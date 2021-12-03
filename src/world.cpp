#include<stdio.h>
#include<thread>
#include"cell.hpp"
#include"world.hpp"
#include"screenio.hpp"


namespace gol
{
    World::World(const int width, const int height)
    {
        turn = 0;
        size[0] = width;
        size[1] = height;
        liveCount[0] = 0;
        liveCount[1] = 0;

        setMap();
    }

    void World::deinit()
    {
        WorldMapStreet* wms;
        for(int i = 0; i < size[1]; i++)
        {
            WorldMapStreet* wms = &(map[i]);
            for(int j = 0; j < size[0]; j++)
            {
                free((*wms)[j]);
            }
        }

        return;
    }

    int World::goTurn()
    {
        // updateAllMap();

        // return ++turn;

        updateAllCells();

        countAllCells((turn & 1) ^ 1);

        return turn + 1;
    }

    int World::nextTurn()
    {
        return ++turn;
    }

    int World::backTurn()
    {
        return --turn;
    }

    int World::getTurn()
    {
        return turn;
    }

    int* World::getSize()
    {
        return size;
    }

    int World::getLiveCount()
    {
        return liveCount[turn & 1];
    }

    Cell* World::getCell(const int x, const int y)
    {
        return map[y][x];
    }

    void World::setSampleMap()
    {
        getCell(2, 1)->setStatus(gol::CellStatus::LIVE, 0);
        getCell(3, 2)->setStatus(gol::CellStatus::LIVE, 0);
        getCell(1, 3)->setStatus(gol::CellStatus::LIVE, 0);
        getCell(2, 3)->setStatus(gol::CellStatus::LIVE, 0);
        getCell(3, 3)->setStatus(gol::CellStatus::LIVE, 0);

        countAllCells(turn & 1);

        return;
    }

    void World::countAllCells(const int t)
    {
        // int t = turn & 1;
        // int t2 = t ^ 1;

        liveCount[t] = 0;

        WorldMapStreet* wms;
        for(int i = 0; i < size[1]; i++)
        {
            WorldMapStreet* wms = &(map[i]);
            for(int j = 0; j < size[0]; j++)
            {
                if((*wms)[j]->getStatus()[t] == LIVE) liveCount[t]++;
            }
        }

        return;
    }

    void World::updateAllMap()
    {
        WorldMapStreet* wms;
        for(int i = 0; i < size[1]; i++)
        {
            WorldMapStreet* wms = &(map[i]);
            for(int j = 0; j < size[0]; j++)
            {
                (*wms)[j]->interact(&map, size, j, i, turn);
            }
        }

        return;
    }

    void World::updateAllCells()
    {
        for(int i = 0; i < size[0] * size[1]; i++)
        {
            updateACell(i);
        }

        return;
    }

    void World::updateACell(const int i)
    {
        updateACell(i % size[0], i / size[0]);

        return;
    }

    void World::updateACell(const int x, const int y)
    {
        map[y][x]->interact(&map, size, x, y, turn);

        return;
    }

    void World::setMap()
    {
        map = WorldMap(size[1], WorldMapStreet(size[0], nullptr));
        fillMap();

        return;
    }

    void World::fillMap()
    {
        WorldMapStreet* wms;
        for(int i = 0; i < size[1]; i++)
        {
            WorldMapStreet* wms = &(map[i]);
            for(int j = 0; j < size[0]; j++)
            {
                (*wms)[j] = new Cell();
            }
        }

        return;
    }

    bool World::needRender()
    {
        return true;
    }

    void World::render()
    {
        // ANSIES(SCP CUP(3, 5));

        WorldMapStreet* wms;
        for(int i = 0; i < size[1]; i++)
        {
            WorldMapStreet* wms = &(map[i]);
            for(int j = 0; j < size[0]; j++)
            {
                // switch((*wms)[j]->getStatus()[turn & 1])
                // {
                //     case DEAD: printf(". "); break;
                //     case LIVE: printf("O "); break;
                // }
                (*wms)[j]->render(j, i, turn);
            }
            // ANSIES("\n" CHA(5));
        }

        return;
    }

    void World::renderInit()
    {
        ANSIES(DEC(0) CUP(2, 4));
        printf(DEC_VBHR);
        for(int i = 0; i < (size[0] << 1) + 1; i++)
        {
            printf(DEC_VNHF);
        }
        printf(DEC_VBHL);

        ANSIES(CUP(23, 4));
        printf(DEC_VTHR);
        for(int i = 0; i < (size[0] << 1) + 1; i++)
        {
            printf(DEC_VNHF);
        }
        printf(DEC_VTHL);

        ANSIES(CUP(3, 4));
        for(int i = 0; i < size[1]; i++)
        {
            printf(DEC_VFHN CUB(1) CUD(1));
        }

        printf(CUP(3, %d), 4 + (size[0] << 1) + 2);
        for(int i = 0; i < size[1]; i++)
        {
            printf(DEC_VFHN CUB(1) CUD(1));
        }

        ANSIES(DEC(B));

        return;
    }
}
