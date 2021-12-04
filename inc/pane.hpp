#pragma once
#ifndef PANE_HPP
#define PANE_HPP

#include"renderee.hpp"


namespace gol
{
    class Pane : public IRenderee
    {
    public:
        Pane();

    private:
        bool needRender();
        void render();
        void renderInit();
    };
}

#endif
