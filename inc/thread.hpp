#pragma once
#ifndef THREAD_HPP
#define THREAD_HPP

#include<thread>
#include<atomic>
#include<mutex>
#include<condition_variable>
#include<future>


namespace gol
{
    enum class ThreadState : unsigned int
    {
        STOPPED = 0, RUNNING, PAUSED
    };

    struct ThreadTask
    {
        int (*process)(void* arg);
        void* arg;
    };

    class Thread
    {
    public:
        Thread(ThreadTask task);
        virtual ~Thread();
        ThreadState getState() const;
        void deletePromise();
        std::future<int>* nextPromise();
        std::future<int>* currentPromise();
        bool getPromiseReady() const;
        void start();
        void stop();
        void resume();
        void storrs();

    private:
        ThreadTask task;
        std::promise<int>** p;
        std::future<int>* f;
        std::thread* th;
        std::mutex mtx;
        std::condition_variable cv;
        std::atomic_bool promiseFlag;
        std::atomic_bool pauseFlag;
        std::atomic_bool stopFlag;
        std::atomic<ThreadState> state;
        void pause();
        void run(std::promise<int>** p);
    };
}

#endif
