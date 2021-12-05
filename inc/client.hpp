#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include<arpa/inet.h>
#include<sys/socket.h>
#include<string>


namespace gol
{
    class GolClient
    {
    public:
        GolClient();
        bool createSocket();
        bool connectServer();
        bool closeSocket();

    private:
        int clientSocketFd;
        sockaddr_in serverAddr;
        void setAddr();
    };
}

#endif
