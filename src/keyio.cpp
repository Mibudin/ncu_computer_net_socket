#include<stdio.h>
#include<unistd.h>
#include<iostream>
#include<limits>
#include<future>
#include<termios.h>
#include"keyio.hpp"
#include"thread.hpp"


namespace gol
{
    Keyio::Keyio()
    {
        kinth == nullptr;
        // kinf == nullptr;
    }

    Keyio::~Keyio()
    {
        if(kinth) delete kinth;
    }

    void Keyio::startWait()
    {
        // kin = std::async(std::launch::async, &gol::Keyio::blockWaitKey, this);

        if(kinth != nullptr && kinth->getState() == ThreadState::STOPPED)
        {
            delete kinth;
            // delete kinf;
            kinth = nullptr;
            // kinf = nullptr;
        }
        if(kinth == nullptr)
        {
            kinth = new Thread({
                [](void* ths){return ((Keyio*)ths)->blockWaitKey();}, this});
            // kinf = kinth->nextPromise();
            kinth->nextPromise();
            kinth->start();
        }
        else if(kinth->getState() == ThreadState::PAUSED)
        {
            // kinf = kinth->nextPromise();
            kinth->nextPromise();
            kinth->resume();
        }

        return;
    }

    bool Keyio::waitKeyAsync(const std::chrono::steady_clock::time_point time)
    {
        // bool ready = kin.wait_until(time) == std::future_status::ready;
        // if(ready) lastKey = kin.get();

        bool ready = kinth->currentPromise()->wait_until(time)
                     == std::future_status::ready;

        return ready;
    }

    int Keyio::getLastKey()
    {
        return lastKey;
    }

    int Keyio::blockWaitKey()
    {
        // std::cin.sync();
        // fflush(stdin);
        // std::cin.ignorse(std::numeric_limits<std::streamsize>::max());
        // return (lastKey = getchar());

        tcflush(0, TCIFLUSH);

        // While setting the VMIN in TTY info as 1,
        // there is no need to use kbhit in a busy-wait loop anymore.
        // Ref: https://stackoverflow.com/a/18806671
        // while(!kbhit());

        lastKey = getch();

        return lastKey;
    }

    int Keyio::kbhit()
    {
        struct timeval tv = {0L, 0L};
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);

        return select(1, &fds, NULL, NULL, &tv);
    }

    int Keyio::getch()
    {
        int r;
        unsigned char c;

        r = read(STDIN_FILENO, &c, sizeof(c));

        if(r < 0) return r;
        else      return c;
    }
}
