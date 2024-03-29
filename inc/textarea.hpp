#pragma once
#ifndef TEXTAREA_HPP
#define TEXTAREA_HPP

#include"renderee.hpp"
#include"world.hpp"
#include"pane.hpp"
#include"config.hpp"


namespace gol
{
    // enum class BarType : int
    // {
    //     TOP, MID, BUT
    // };

    // enum class ModeType : int
    // {
    //     SET, RUN
    // };


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
        // void setMode(const ModeType mode);

    protected:
        void render() override;
        void renderInit() override;

    private:
        World* wld;
        // ModeType mode;
    };

    enum class NetworkState : int
    {
        INIT = 0, CONNECTED,
        SEND_SET,   RECV_SET,   SET,
        SEND_RUN,   RECV_RUN,   RUN,
        SEND_CLOSE, RECV_CLOSE, CLOSE,
    };

    class NetworkPane : public Pane
    {
    public:
        NetworkPane(const int width, const int height,
                    const int x, const int y);
        void setConnect(const std::string ipAddr, const int port);
        void setNetworkState(const NetworkState state);
        void emergRender();
    
    protected:
        void render() override;
        void renderInit() override;

    private:
        std::string ipAddr;
        int port;
        NetworkState state;
    };
}

#endif
