#pragma once
#ifndef KEYIO_HPP
#define KEYIO_HPP

#include<future>
#include"thread.hpp"


#define CTRL(k) ((k) & 0x1f)


namespace gol
{
    class Keyio
    {
    public:
        Keyio();
        ~Keyio();
        void startWait();
        bool waitKeyAsync(const std::chrono::steady_clock::time_point time);
        int getLastKey();
        int blockWaitKey();

    private:
        // std::future<int> kin;
        Thread* kinth;
        // std::future<int>* kinf;
        int lastKey;
        int kbhit();
        int getch();
    };
}

#endif
