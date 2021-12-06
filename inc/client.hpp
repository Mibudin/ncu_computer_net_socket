#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include<arpa/inet.h>
#include<sys/socket.h>
#include<string>
#include<future>
#include"msgpkt.hpp"
#include"thread.hpp"


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

        MsgPacket lastSendPkt;
        MsgPacket lastRecvPkt;
        bool sendMsgPacket(const MsgPacket* pkt);
        MsgPacket* recvMsgPacket();
        std::future<int>* asyncRecvMsgPacket();
        bool checkMode(const ModeType mode);
        bool sendKey(const int key);
        int recvKey();

    private:
        bool connected;
        Thread* th;
        int clientSocketFd;
        sockaddr_in serverAddr;
        void setAddr();
    };
}

#endif
