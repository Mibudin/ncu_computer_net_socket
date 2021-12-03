#include<stdio.h>
#include<unistd.h>
#include<iostream>
#include<limits>
#include<future>
#include<termios.h>
#include"keyio.hpp"


namespace gol
{
    void Keyio::startWait()
    {
        kin = std::async(std::launch::async, &gol::Keyio::blockWaitKey, this);

        return;
    }

    bool Keyio::waitKeyAsync(const std::chrono::steady_clock::time_point time)
    {
        bool ready = kin.wait_until(time) == std::future_status::ready;
        // if(ready) lastKey = kin.get();

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

        while(!kbhit());

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
