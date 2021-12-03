#pragma once
#ifndef TEXTAREA_H
#define TEXTAREA_H

#include"renderee.hpp"
#include"world.hpp"


namespace gol
{
    enum BarType : int
    {
        TOP, MID, BUT
    };

    enum ModeType : int
    {
        SET, RUN
    };


    class Textarea : public IRenderee
    {
    public:
        Textarea(World* wld);
        void setMode(const ModeType mode);

    private:
        World* wld;
        int start;
        int width;
        ModeType mode;
        void renderSpace(const int y);
        void renderBar(const int y, const BarType type);
        bool needRender();
        void render();
        void renderInit();
    };
}

#endif
