#pragma once
#ifndef RENDEREE_H
#define RENDEREE_H

#include"renderer.hpp"


namespace gol
{
    class IRenderee
    {
        friend class Renderer;
    public:
        ;

    private:
        virtual bool needRender() = 0;
        virtual void render() = 0;
        virtual void renderInit() = 0;
    };
}

#endif
