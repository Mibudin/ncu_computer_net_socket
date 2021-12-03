#pragma once
#ifndef RENDEREE_HPP
#define RENDEREE_HPP

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
