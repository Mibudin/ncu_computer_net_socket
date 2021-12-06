#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include<string>
#include<future>
#include<arpa/inet.h>
#include<sys/socket.h>
#include"msgpkt.hpp"
#include"thread.hpp"


namespace gol
{
    class GolServer
    {
    public:
        GolServer();
        bool createSocket();
        bool listenSocket();
        bool acceptClient();
        bool closeSocket();
        bool closeClientSocket();
        std::string getClientIPAddr();
        int getClientPort();
        // MsgPacket getLastSendPkt();
        // MsgPacket getLastRecvPkt();

        MsgPacket lastSendPkt;
        MsgPacket lastRecvPkt;
        bool sendMsgPacket(const MsgPacket* pkt);
        MsgPacket* recvMsgPacket();
        std::future<int>* asyncSendMsgPacket(const MsgPacket* pkt);
        std::future<int>* asyncRecvMsgPacket();
        bool sendKey(const int key);
        bool sendMode(const ModeType mode);
        bool sendCell(const int x, const int y, const CellStatus status);
        bool sendTP(const int tp);
        int recvKey();

    private:
        bool connected;
        Thread* th;
        int serverSocketFd;
        int clientSocketFd;
        sockaddr_in serverAddr;
        sockaddr_in clientAddr;
        void setAddr();
    };
}

#endif
