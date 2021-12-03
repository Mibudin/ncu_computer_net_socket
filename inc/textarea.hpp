#pragma once
#ifndef TEXTAREA_HPP
#define TEXTAREA_HPP

#include"renderee.hpp"
#include"world.hpp"


namespace gol
{
    enum class BarType : int
    {
        TOP, MID, BUT
    };

    enum class ModeType : int
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
