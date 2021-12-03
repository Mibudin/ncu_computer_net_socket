#pragma once
#ifndef KEYIO_H
#define KEYIO_H

#include<future>


#define CTRL(k) ((k) & 0x1f)


namespace gol
{
    class Keyio
    {
    public:
        void startWait();
        bool waitKeyAsync(const std::chrono::steady_clock::time_point time);
        int getLastKey();
        int blockWaitKey();

    private:
        std::future<int> kin;
        int lastKey;
        int kbhit();
        int getch();
    };
}

#endif
