#include<stdio.h>
#include<thread>
#include"cell.hpp"
#include"world.hpp"
#include"screenio.hpp"


namespace gol
{
    Cell::Cell()
    {
        status[0] = DEAD;
        status[1] = DEAD;
    }

    CellStatus* Cell::getStatus()
    {
        return status;
    }

    void Cell::setStatus(CellStatus _status, const int t)
    {
        status[t] = _status;

        return;
    }

    CellStatus Cell::interact(const WorldMap* map, const int size[2],
        const int x, const int y, const int turn)
    {
        int t = turn & 1;
        int t2 = t ^ 1;
        status[t2] = liveOrDie(checkEnv(map, size, x, y, t), t);

        return status[t2];
    }

    void Cell::render(const int x, const int y, const int turn)
    {
        printf(CUP(%d, %d), y + 3, (x << 1) + 5 + 1);
        switch(status[turn & 1])
        {
            case DEAD: printf(SGR_CLOR(49, 31) ". " SGR_DEFT); break;
            case LIVE: printf(SGR_CLOR(49, 92) SGR_BDBR "O " SGR_DEFT); break;
        }
        fflush(nullptr);

        return;
    }

    int Cell::checkEnv(const WorldMap* map, const int size[2],
        const int x, const int y, const int t)
    {
        int liveCellCount = 0;
        int p, q;
        for(int i = 0; i < 8; i++)
        {
            p = x + neighbors[i][0];
            q = y + neighbors[i][1];

                 if(p < 0)        p += size[0];
            else if(p >= size[0]) p -= size[0];
                 if(q < 0)        q += size[1];
            else if(q >= size[1]) q -= size[1];

            liveCellCount += (*map)[q][p]->getStatus()[t] == LIVE ? 1 : 0;  // LIVE = 1
        }
        // if(liveCellCount != 0) printf("> %d, %d > %d\n", x, y, liveCellCount);
        
        return liveCellCount;
    }

    CellStatus Cell::liveOrDie(const int liveCellCount, const int t)
    {
        switch(status[t])
        {
            case DEAD:
                if(liveCellCount == 3) return LIVE;
                else                   return DEAD;
                break;
            case LIVE:
                     if(liveCellCount < 2) return DEAD;
                else if(liveCellCount > 3) return DEAD;
                else                       return LIVE;
                break;
        }
    }
}
