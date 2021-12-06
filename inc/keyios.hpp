#pragma once
#ifndef KEYIOS_HPP
#define KEYIOS_HPP

#include<queue>
#include<future>
#include<mutex>
#include<condition_variable>
#include"thread.hpp"
#include"server.hpp"
#include"client.hpp"


// #define CTRL(k) ((k) & 0x1f)


namespace gol
{
    class Keyios
    {
        struct KiosGols
        {
            Keyios* kios;
            GolServer* gols;
        };

        struct KiosGolc
        {
            Keyios* kios;
            GolClient* golc;
        };


    public:
        Keyios(GolServer* gols, GolClient* golc);
        ~Keyios();
        std::mutex waitMutex;
        std::condition_variable cv;
        void startWait();
        bool waitKey();
        bool waitKeyAsync(const std::chrono::steady_clock::time_point time);
        int setLastKey(const int key);
        int getLastKey();
        int blockWaitKey();

    private:
        KiosGols kgs;
        KiosGolc kgc;
        Thread* kinth;
        Thread* kinths;
        Thread* kinthc;
        std::queue<int> lastKey;
        std::mutex lastKeyMutex;
        int getch();
    };
}

#endif
