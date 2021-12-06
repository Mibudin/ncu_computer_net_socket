#include<stdio.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include"keyios.hpp"


namespace gol
{
    Keyios::Keyios(GolServer* gols, GolClient* golc)
    {
        kinth = nullptr;
        kinths = nullptr;
        kinthc = nullptr;

        kgs = {this, gols};
        kgc = {this, golc};
    }

    Keyios::~Keyios()
    {
        if(kinth)  delete kinth;
        if(kinths) delete kinths;
        if(kinthc) delete kinthc;
    }

    void Keyios::startWait()
    {
        if(kinth != nullptr && kinth->getState() == ThreadState::STOPPED)
        {
            delete kinth;
            kinth = nullptr;
        }
        if(kinth == nullptr)
        {
            kinth = new Thread({
                [](void* ths){
                    std::unique_lock<std::mutex> lock(((Keyios*)ths)->waitMutex);
                    ((Keyios*)ths)->cv.wait(lock);
                    return 0;
                }, this});
            kinth->nextPromise();
            kinth->start();
        }
        else if(kinth->getState() == ThreadState::PAUSED)
        {
            kinth->nextPromise();
            kinth->resume();
        }

        if(kinths != nullptr && kinths->getState() == ThreadState::STOPPED)
        {
            delete kinths;
            kinths = nullptr;
        }
        if(kinths == nullptr)
        {
            if(kgc.golc)
            {
                kinths = new Thread({
                    [](void* kgc){
                        int key = ((KiosGolc*)kgc)->kios->setLastKey(
                            ((KiosGolc*)kgc)->golc->recvKey());
                        ((KiosGolc*)kgc)->kios->cv.notify_all();
                        return key;
                    }, &kgc});
                kinths->nextPromise();
                kinths->start();
            }
            else
            {
                kinths = new Thread({
                    [](void* kgs){
                        int key = ((KiosGols*)kgs)->kios->blockWaitKey();
                        ((KiosGols*)kgs)->gols->sendKey(key);
                        ((KiosGols*)kgs)->kios->cv.notify_all();
                        return key;
                    }, &kgs});
                kinths->nextPromise();
                kinths->start();
            }
        }
        else if(kinths->getState() == ThreadState::PAUSED)
        {
            kinths->nextPromise();
            kinths->resume();
        }

        if(kinthc != nullptr && kinthc->getState() == ThreadState::STOPPED)
        {
            delete kinthc;
            kinthc = nullptr;
        }
        if(kinthc == nullptr)
        {
            if(kgs.gols)
            {
                kinthc = new Thread({
                    [](void* kgs){
                        int key = ((KiosGols*)kgs)->kios->setLastKey(
                            ((KiosGols*)kgs)->gols->recvKey());
                        ((KiosGols*)kgs)->kios->cv.notify_all();
                        return key;
                    }, &kgs});
                kinthc->nextPromise();
                kinthc->start();
            }
            else
            {
                // kinthc = new Thread({
                //     [](void* kgc){
                //         int key = ((KiosGolc*)kgc)->kios->blockWaitKey();
                //         ((KiosGolc*)kgc)->golc->sendKey(key);
                //         ((KiosGolc*)kgc)->kios->cv.notify_all();
                //         return key;
                //     }, &kgc});
                // kinthc->nextPromise();
                // kinthc->start();
            }
        }
        else if(kinthc->getState() == ThreadState::PAUSED)
        {
            kinthc->nextPromise();
            kinthc->resume();
        }

        return;
    }

    bool Keyios::waitKey()
    {  // TODO
        if(!lastKey.empty()) return true;

        // https://stackoverflow.com/questions/43614634/stdthread-how-to-wait-join-for-any-of-the-given-threads-to-complete
        // std::unique_lock<std::mutex> lock(waitMutex);
        // cv.wait(lock);

        while(kinth->currentPromise()->wait_for(std::chrono::milliseconds(10))
              != std::future_status::ready)
        {
            if(!lastKey.empty()) return true;
        }

        return true;
    }

    bool Keyios::waitKeyAsync(const std::chrono::steady_clock::time_point time)
    {
        if(!lastKey.empty()) return true;

        // std::unique_lock<std::mutex> lock(waitMutex);
        // bool ready = cv.wait_until(lock, time) == std::cv_status::no_timeout;
        // bool ready = kgc.golc ? kinthc->currentPromise()->wait_until(time)
        //                         == std::future_status::ready
        //                       : kinths->currentPromise()->wait_until(time)
        //                         == std::future_status::ready;

        bool ready = kinth->currentPromise()->wait_until(time)
                     == std::future_status::ready;

        return ready;
    }

    int Keyios::setLastKey(const int key)
    {
        std::lock_guard<std::mutex> lock(lastKeyMutex);

        if(key >= 0) lastKey.push(key);

        return key;
    }

    int Keyios::getLastKey()
    {
        std::lock_guard<std::mutex> lock(lastKeyMutex);

        int key = -1;
        if(!lastKey.empty())
        {
            memcpy(&key, &lastKey.front(), sizeof(int));
            lastKey.pop();
        }

        return key;
    }

    int Keyios::blockWaitKey()
    {
        tcflush(0, TCIFLUSH);

        int key = getch();
        setLastKey(key);

        return key;
    }

    int Keyios::getch()
    {
        int r;
        unsigned char c;

        r = read(STDIN_FILENO, &c, sizeof(c));

        if(r < 0) return r;
        else      return c;
    }
}
