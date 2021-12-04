#pragma once
#ifndef WORLD_HPP
#define WORLD_HPP

#include<vector>
#include"cell.hpp"
#include"renderee.hpp"
#include"pane.hpp"


namespace gol
{
    // Forward declarations
    class Cell;
    enum class CellStatus : int;


    typedef std::vector<Cell*> WorldMapStreet;
    typedef std::vector<WorldMapStreet> WorldMap;


    class World
    {
        friend class WorldPane;

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
        // void updateAllCells();
        // void updateACell(const int i);
        // void updateACell(const int x, const int y);
        void setMap();
        void fillMap();
        // bool needRender() override;
        // void render() override;
        // void renderInit() override;
    };

    class WorldPane : public Pane
    {
    public:
        WorldPane(World* wld, const int x, const int y);

    protected:
        void render() override;

    private:
        World* wld;
    };
}

#endif
