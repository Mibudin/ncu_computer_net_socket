#pragma once
#ifndef WORLD_H
#define WORLD_H

#include<vector>
#include"cell.hpp"
#include"renderee.hpp"


namespace gol
{
    // Forward declarations
    class Cell;
    enum CellStatus : int;


    typedef std::vector<Cell*> WorldMapStreet;
    typedef std::vector<WorldMapStreet> WorldMap;


    class World : public IRenderee
    {
    public:
        World(const int width, const int height);
        void deinit();
        int goTurn();
        int nextTurn();
        int backTurn();
        int getTurn();
        int* getSize();
        int getLiveCount();
        Cell* getCell(const int x, const int y);
        void setSampleMap();
        void countAllCells(const int t);

    private:
        int turn;
        int size[2];
        int liveCount[2];
        WorldMap map;  // map[y][x]
        void updateAllMap();
        void updateAllCells();
        void updateACell(const int i);
        void updateACell(const int x, const int y);
        void setMap();
        void fillMap();
        bool needRender();
        void render();
        void renderInit();
    };
}

#endif
