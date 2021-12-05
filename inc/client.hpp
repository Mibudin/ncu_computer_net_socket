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
        std::string getClientIPAddr();
        int getClientPort();
        bool sendMsgPacket(const MsgPacket* pkt);
        MsgPacket* recvMsgPacket();

    private:
        int clientSocketFd;
        sockaddr_in serverAddr;
        void setAddr();
    };
}

#endif
