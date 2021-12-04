#pragma once
#ifndef PANE_HPP
#define PANE_HPP

#include"renderee.hpp"


namespace gol
{
    class Pane : public IRenderee
    {
    public:
        Pane(const int width, const int height,
             const int x, const int y);
        void setAdjacency(const bool top, const bool bottom,
                          const bool left, const bool right);
        void renderDividerH(const int row);
        void renderDividerV(const int col);

    protected:
        int pos[2];
        int size[2];
        virtual bool needRender() override;
        virtual void render() override;
        virtual void renderInit() override;

    private:
        int adj[4];
        void renderCorner();
        void renderEdge();
        void renderBorder();
    };
}

#endif
