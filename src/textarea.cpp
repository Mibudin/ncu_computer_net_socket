#include<stdio.h>
#include"config.hpp"
#include"textarea.hpp"
#include"screenio.hpp"


namespace gol
{
    Textarea::Textarea(World* wld)
    {
        this->wld = wld;

        this->start = (WORLD_WIDTH << 1) + 7;
        this->width = SCREEN_WIDTH - ((WORLD_WIDTH << 1) + 2 + 8);

        this->mode = SET;
    }

    void Textarea::setMode(const ModeType mode)
    {
        this->mode = mode;

        return;
    }

    void Textarea::renderSpace(const int y)
    {
        printf(CUP(%d, %d), y, start);
        printf(DEC_VFHN CHA(%d) DEC_VFHN, start + width + 1);

        return;
    }

    void Textarea::renderBar(const int y, const BarType type)
    {
        printf(CUP(%d, %d), y, start);
        switch(type)
        {
            case TOP: printf(DEC_VBHR CUF(%d) DEC_VBHL, width); break;
            case MID: printf(DEC_VFHR CUF(%d) DEC_VFHL, width); break;
            case BUT: printf(DEC_VTHR CUF(%d) DEC_VTHL, width); break;
        }

        printf(CHA(%d), start + 1);
        for(int i = 0; i < width; i++)
        {
            printf(DEC_VNHF);
        }

        return;
    }

    bool Textarea::needRender()
    {
        return true;
    }

    void Textarea::render()
    {
        // Phase
        printf(CUP(6, %d) ECH(12), start + 2);
        switch(mode)
        {
            case SET: printf(SGR_CLOR(49, 96) SGR_UDLN "Map " SGR_BDBR "Set" SGR_NOBB " Mode" SGR_DEFT); break;
            case RUN: printf(SGR_CLOR(49, 96) SGR_UDLN "Map " SGR_BDBR "Run" SGR_NOBB " Mode" SGR_DEFT); break;
        }
        // Turn
        printf(CUP(8, %d) ECH(6), start + 8);
        printf(SGR_CLOR(49, 93) "%d" SGR_DEFT, wld->getTurn());
        // Turn Period
        printf(CUP(10, %d) ECH(6), start + 8);
        printf(SGR_CLOR(49, 93) "%d" SGR_DEFT, turnPeriod);
        // Cell Counts
        int lives = wld->getLiveCount();
        printf(CUP(12, %d) ECH(3), start + 11);
        printf(SGR_CLOR(49, 92) "%d" SGR_DEFT, lives);
        printf(CUP(13, %d) ECH(3), start + 11);
        printf(SGR_CLOR(49, 91) "%d" SGR_DEFT, WORLD_WIDTH * WORLD_HEIGHT - lives);
        // Control Guide
        switch(mode)
        {
            case SET:
                printf(CUP(15, %d) ECH(12), start + 2);
                printf(SGR_CLOR(49, 95) SGR_BDBR "W" SGR_DEFT ", "
                       SGR_CLOR(49, 95) SGR_BDBR "A" SGR_DEFT ", "
                       SGR_CLOR(49, 95) SGR_BDBR "S" SGR_DEFT ", "
                       SGR_CLOR(49, 95) SGR_BDBR "D" SGR_DEFT ":");
                printf(CUP(16, %d) ECH(12), start + 2);
                printf(" Move cursor");
                printf(CUP(17, %d) ECH(12), start + 2);
                printf(SGR_CLOR(49, 95) SGR_BDBR "[" SGR_DEFT ": Set dead");
                printf(CUP(18, %d) ECH(12), start + 2);
                printf(SGR_CLOR(49, 95) SGR_BDBR "]" SGR_DEFT ": Set live");
                printf(CUP(19, %d) ECH(12), start + 2);
                printf(SGR_CLOR(49, 95) SGR_BDBR ";" SGR_DEFT ": Slower");
                printf(CUP(20, %d) ECH(12), start + 2);
                printf(SGR_CLOR(49, 95) SGR_BDBR "'" SGR_DEFT ": Faster");
                printf(CUP(21, %d) ECH(12), start + 2);
                printf(SGR_CLOR(49, 95) SGR_BDBR "\\" SGR_DEFT ": Run map");
                printf(CUP(22, %d) ECH(12), start + 2);
                printf(SGR_CLOR(49, 95) SGR_BDBR "P" SGR_DEFT ": Exit game");
                break;
            case RUN:
                printf(CUP(15, %d) ECH(12), start + 2);
                printf(SGR_CLOR(49, 95) SGR_BDBR ";" SGR_DEFT ": Slower");
                printf(CUP(16, %d) ECH(12), start + 2);
                printf(SGR_CLOR(49, 95) SGR_BDBR "'" SGR_DEFT ": Faster");
                printf(CUP(17, %d) ECH(12), start + 2);
                printf(SGR_CLOR(49, 95) SGR_BDBR "\\" SGR_DEFT ": Set map");
                printf(CUP(18, %d) ECH(12), start + 2);
                printf(SGR_CLOR(49, 95) SGR_BDBR "P" SGR_DEFT ": Exit game");
                printf(CUP(19, %d) ECH(12), start + 2);
                printf(CUP(20, %d) ECH(12), start + 2);
                printf(CUP(21, %d) ECH(12), start + 2);
                printf(CUP(22, %d) ECH(12), start + 2);
                break;
        }

        ANSIES("");

        return;
    }

    void Textarea::renderInit()
    {
        ANSIES(DEC(0));

        // Title
        renderBar(2, TOP);
        renderSpace(3);
        renderBar(4, BUT);
        // Phase
        renderBar(5, TOP);
        renderSpace(6);
        // Turn
        renderBar(7, MID);
        renderSpace(8);
        // Turn Period
        renderBar(9, MID);
        renderSpace(10);
        // Cell Counts
        renderBar(11, MID);
        renderSpace(12);
        renderSpace(13);
        // Control Guide
        renderBar(14, MID);
        renderSpace(15);
        renderSpace(16);
        renderSpace(17);
        renderSpace(18);
        renderSpace(19);
        renderSpace(20);
        renderSpace(21);
        renderSpace(22);
        renderBar(23, BUT);

        ANSIES(DEC(B));

        // Title
        printf(CUP(3, %d), start + 2);
        printf(SGR_CLOR(49, 93) SGR_BDBR "Game of Life" SGR_DEFT);
        // Phase
        // ...
        // Turn
        printf(CUP(8, %d), start + 2);
        printf(SGR_CLOR(49, 94) SGR_BDBR "Turn" SGR_DEFT ":");
        // TurnPeriod
        printf(CUP(10, %d), start + 2);
        printf(SGR_CLOR(49, 94) SGR_BDBR "T.P." SGR_DEFT ":");
        // Cell Counts
        printf(CUP(12, %d), start + 2);
        printf(SGR_CLOR(49, 94) SGR_BDBR "Live C." SGR_DEFT ":");
        printf(CUP(13, %d), start + 2);
        printf(SGR_CLOR(49, 94) SGR_BDBR "Dead C." SGR_DEFT ":");
        // Control Guide
        // ...

        this->render();

        return;
    }
}
