#include"thread.hpp"


namespace gol
{
    Thread::Thread(ThreadTask task)
    {
        this->task = task;
        p = new std::promise<int>*;
        *p = nullptr;
        th = nullptr;
        state = ThreadState::STOPPED;
        promiseFlag = false;
        pauseFlag = false;
        stopFlag = false;
    }

    Thread::~Thread()
    {
        stop();
    }

    ThreadState Thread::getState() const
    {
        return state;
    }

    std::future<int>* Thread::nextPromise()  // TODO
    {
        promiseFlag = false;
        if(*p != nullptr) delete (*p);
        *p = new std::promise<int>();

        return &((*p)->get_future());
    }

    void Thread::start()
    {
        if(th != nullptr) return;
        
        th = new std::thread(&Thread::run, this, std::move(p));
        state = ThreadState::RUNNING;
        promiseFlag = false;
        pauseFlag = false;
        stopFlag = false;

        return;
    }

    void Thread::stop()
    {
        if(th == nullptr) return;

        promiseFlag = false;
        pauseFlag = false;
        stopFlag = true;
        cv.notify_all();
        th->join();

        if(*p) delete (*p);
        delete th;
        *p = nullptr;
        th = nullptr;
        state = ThreadState::STOPPED;

        return;
    }

    void Thread::pause()
    {
        if(th == nullptr) return;

        pauseFlag = true;
        state = ThreadState::PAUSED;

        return;
    }

    void Thread::resume()
    {
        if(th == nullptr || !pauseFlag) return;

        pauseFlag = false;
        cv.notify_all();
        state = ThreadState::RUNNING;

        return;
    }

    void Thread::run(std::promise<int>** p)
    {
        while(!stopFlag)
        {
            if(task.process != nullptr)
            {
                int value = task.process(task.arg);
                if(*p != nullptr)
                {
                    (*p)->set_value(value);
                    // TODO: Pause for future
                    // TODO: Join
                }
            }

            if(pauseFlag)
            {
                std::unique_lock<std::mutex> locker(mtx);
                while(pauseFlag)
                {
                    cv.wait(locker);
                }
                locker.unlock();
            }
        }
        pauseFlag = false;
        stopFlag = false;

        return;
    }
}
