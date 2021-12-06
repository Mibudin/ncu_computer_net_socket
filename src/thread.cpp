#include"thread.hpp"


namespace gol
{
    Thread::Thread(ThreadTask task)
    {
        this->task = task;
        p = new std::promise<int>*;
        *p = nullptr;
        f = nullptr;
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

    void Thread::deletePromise()
    {
        promiseFlag = false;
        if(*p) delete (*p);
        if(f) delete f;
    }

    std::future<int>* Thread::nextPromise()
    {
        deletePromise();
        *p = new std::promise<int>();
        f = new std::future<int>((*p)->get_future());

        return f;
    }

    std::future<int>* Thread::currentPromise()
    {
        return f;
    }

    bool Thread::getPromiseReady() const
    {
        return promiseFlag;
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

        pauseFlag = false;
        stopFlag = true;
        cv.notify_all();
        th->join();

        deletePromise();
        delete th;
        th = nullptr;
        state = ThreadState::STOPPED;

        return;
    }

    void Thread::resume()
    {
        if(th == nullptr || !pauseFlag) return;

        if(promiseFlag) deletePromise();
        pauseFlag = false;
        cv.notify_all();
        state = ThreadState::RUNNING;

        return;
    }

    void Thread::storrs()
    {
        if(th == nullptr) start();
        else              resume();
    }

    void Thread::setTask(ThreadTask task)
    {
        if(th != nullptr || !pauseFlag) return;

        this->task = task;

        return;
    }

    void Thread::pause()
    {
        if(th == nullptr) return;

        pauseFlag = true;
        state = ThreadState::PAUSED;

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
                    promiseFlag = true;
                    pause();
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
