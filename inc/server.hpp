#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include<string>
#include<arpa/inet.h>
#include<sys/socket.h>


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

    private:
        int serverSocketFd;
        int clientSocketFd;
        sockaddr_in serverAddr;
        sockaddr_in clientAddr;
        void setAddr();
    };
}

#endif
