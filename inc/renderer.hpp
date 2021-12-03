#pragma once
#ifndef RENDERER_HPP
#define RENDERER_HPP

#include<vector>
#include"renderee.hpp"


namespace gol
{
    // Forward Declarations
    class IRenderee;

    class Renderer
    {
    public:
        void renderAll();
        void renderInit();
        void addRenderee(IRenderee* renderee);

    private:
        std::vector<IRenderee*> renderees;
    };
}

#endif
