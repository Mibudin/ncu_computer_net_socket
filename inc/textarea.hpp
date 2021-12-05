#pragma once
#ifndef TEXTAREA_HPP
#define TEXTAREA_HPP

#include"renderee.hpp"
#include"world.hpp"
#include"pane.hpp"


namespace gol
{
    // enum class BarType : int
    // {
    //     TOP, MID, BUT
    // };

    enum class ModeType : int
    {
        SET, RUN
    };


    // class Textarea : public IRenderee
    // {
    // public:
    //     Textarea(World* wld);
    //     void setMode(const ModeType mode);

    // private:
    //     World* wld;
    //     int start;
    //     int width;
    //     ModeType mode;
    //     void renderSpace(const int y);
    //     void renderBar(const int y, const BarType type);
    //     bool needRender() override;
    //     void render() override;
    //     void renderInit() override;
    // };

    class TitlePane : public Pane
    {
    public:
        TitlePane(const int width, const int height,
                  const int x, const int y);

    protected:
        bool needRender() override;
        void renderInit() override;
    };

    class InfoPane : public Pane
    {
    public:
        InfoPane(World* wld,
                 const int width, const int height,
                 const int x, const int y);
        void setMode(const ModeType mode);

    protected:
        void render() override;
        void renderInit() override;

    private:
        World* wld;
        ModeType mode;
    };

    class NetworkPane : public Pane
    {
    public:
        NetworkPane(const int width, const int height,
                    const int x, const int y);
        void setConnect(const std::string ipAddr, const int port);
    
    protected:
        void render() override;
        void renderInit() override;

    private:
        std::string ipAddr;
        int port;
    };
}

#endif
