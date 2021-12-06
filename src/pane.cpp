#include<stdio.h>
#include<vector>
#include"pane.hpp"
#include"screenio.hpp"
#include"config.hpp"


namespace gol
{
    Pane::Pane(const int width, const int height, const int x, const int y)
    {
        pos[0] = x; pos[1] = y;
        size[0] = width; size[1] = height;
        setAdjacency(false, false, false, false);
    }

    void Pane::setAdjacency(const bool top, const bool bottom,
                            const bool left, const bool right)
    {
        adj[0] = top;  adj[1] = bottom;
        adj[2] = left; adj[3] = right;

        return;
    }

    void Pane::renderCorner()
    {
        ANSIES(DEC(0));

        printf(CUP(%d, %d) "%s", pos[1], pos[0],
            adj[0] ? (adj[2] ? DEC_VFHF : DEC_VFHR)
                   : (adj[2] ? DEC_VBHF : DEC_VBHR));
        printf(CUP(%d, %d) "%s", pos[1], pos[0] + size[0] - 1,
            adj[0] ? (adj[3] ? DEC_VFHF : DEC_VFHL)
                   : (adj[3] ? DEC_VBHF : DEC_VBHL));
        printf(CUP(%d, %d) "%s", pos[1] + size[1] - 1, pos[0],
            adj[1] ? (adj[2] ? DEC_VFHF : DEC_VFHR)
                   : (adj[2] ? DEC_VTHF : DEC_VTHR));
        printf(CUP(%d, %d) "%s", pos[1] + size[1] - 1, pos[0] + size[0] - 1,
            adj[1] ? (adj[3] ? DEC_VFHF : DEC_VFHL)
                   : (adj[3] ? DEC_VTHF : DEC_VTHL));

        ANSIES(DEC(B));

        return;
    }

    void Pane::renderEdge()
    {
        ANSIES(DEC(0));

        printf(CUP(%d, %d), pos[1], pos[0] + 1);
        for(int i = 0; i < size[0] - 2; i++) printf(DEC_VNHF);
        printf(CUP(%d, %d), pos[1] + size[1] - 1, pos[0] + 1);
        for(int i = 0; i < size[0] - 2; i++) printf(DEC_VNHF);
        printf(CUP(%d, %d), pos[1] + 1, pos[0]);
        for(int i = 0; i < size[1] - 2; i++) printf(DEC_VFHN CUB(1) CUD(1));
        printf(CUP(%d, %d), pos[1] + 1, pos[0] + size[0] - 1);
        if(pos[0] + size[0] - 1 == cfg->screenWidth)
             for(int i = 0; i < size[1] - 2; i++) printf(DEC_VFHN CUD(1));
        else for(int i = 0; i < size[1] - 2; i++) printf(DEC_VFHN CUB(1) CUD(1));

        ANSIES(DEC(B));

        return;
    }

    void Pane::renderDividerH(const int row)
    {
        if(row < 1 || row > size[1] - 2) return;

        ANSIES(DEC(0));
        printf(CUP(%d, %d), pos[1] + row, pos[0]);
        ANSIES(DEC_VFHR);
        for(int i = 0; i < size[0] - 2; i++) printf(DEC_VNHF);
        ANSIES(DEC_VFHL);
        ANSIES(DEC(B));

        return;
    }

    void Pane::renderDividerV(const int col)
    {
        if(col < 1 || col > size[0] - 2) return;

        ANSIES(DEC(0));
        printf(CUP(%d, %d), pos[1], pos[0] + col);
        ANSIES(DEC_VBHF CUB(1) CUD(1));
        for(int i = 0; i < size[1] - 2; i++) printf(DEC_VFHN CUB(1) CUD(1));
        ANSIES(DEC_VTHF);
        ANSIES(DEC(B));

        return;
    }

    void Pane::renderBorder()
    {
        renderCorner();
        renderEdge();

        return;
    }

    bool Pane::needRender()
    {
        return true;
    }

    void Pane::render()
    {
        return;
    }

    void Pane::renderInit()
    {
        renderBorder();

        return;
    }
}
