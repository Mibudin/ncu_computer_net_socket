#include<stdio.h>
#include<unistd.h>
#include<termios.h>
#include<thread>
#include"screenio.hpp"


namespace gol
{
    void Screenio::initTty()
    {
        backupTty();
        fflush(nullptr);

        ANSIES(ASB);

        // tty.c_lflag |= ~ECHO;  // Enable
        // tty.c_lflag &= ~ECHO;  // Disable

        getTty();
        tty[STDIN_FILENO].c_lflag &= ~ICANON;
        tty[STDIN_FILENO].c_lflag &= ~ECHO;
        tty[STDIN_FILENO].c_cc[VMIN] = 0;
        tty[STDIN_FILENO].c_cc[VTIME] = 0;
        // cfmakeraw(&(tty[STDIN_FILENO]));
        setTty();

        ANSIES(SWT("Game of Life"));

        ANSIES(CUH ED(2) CUP(1,1));

        return;
    }

    void Screenio::deinitTty()
    {
        ANSIES(CUS);

        fflush(nullptr);
        ANSIES(MSB);

        restoreTty();

        return;
    }

    void Screenio::getTty()
    {
        // WHY CAN'T?
        // for(int i = 0; i < 1; i++)
        // {
        //     tcgetattr(i, &tty[i]);
        // }
        tcgetattr(STDIN_FILENO, &tty[STDIN_FILENO]);

        return;
    }

    void Screenio::setTty()
    {
        // WHY CAN'T?
        // for(int i = 0; i < 3; i++)
        // {
        //     tcsetattr(i, TCSANOW, &tty[i]);
        // }
        tcsetattr(STDIN_FILENO, TCSANOW, &tty[STDIN_FILENO]);

        return;
    }

    void Screenio::backupTty()
    {
        // WHY CAN'T?
        // for(int i = 0; i < 3; i++)
        // {
        //     tcgetattr(i, &otty[i]);
        // }
        tcgetattr(STDIN_FILENO, &otty[STDIN_FILENO]);

        return;
    }

    void Screenio::restoreTty()
    {
        // WHY CAN'T?
        // for(int i = 0; i < 3; i++)
        // {
        //     tcsetattr(i, TCSANOW, &otty[i]);
        // }
        tcsetattr(STDIN_FILENO, TCSANOW, &otty[STDIN_FILENO]);

        return;
    }
}
