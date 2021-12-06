#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include<arpa/inet.h>
#include<sys/socket.h>
#include<string>
#include"msgpkt.hpp"


namespace gol
{
    class GolClient
    {
    public:
        GolClient();
        bool createSocket();
        bool connectServer();
        bool closeSocket();
        std::string getServerIPAddr();
        int getServerPort();

        bool sendMsgPacket(const MsgPacket* pkt);
        MsgPacket* recvMsgPacket();
        bool checkMode(const ModeType mode);

    private:
        bool connected;
        int clientSocketFd;
        sockaddr_in serverAddr;
        void setAddr();
    };
}

#endif
