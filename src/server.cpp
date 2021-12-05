#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string>
#include"server.hpp"
#include"config.hpp"


namespace gol
{
    GolServer::GolServer()
    {
        serverSocketFd = -1;
        clientSocketFd = -1;
    
        setAddr();
    }

    bool GolServer::createSocket()
    {
        serverSocketFd = socket(PF_INET, SOCK_STREAM, 0);
        if(serverSocketFd == -1) return false;

        int bindSuc = bind(serverSocketFd, (const sockaddr*)&serverAddr, sizeof(serverAddr));
        if(bindSuc < 0)
        {
            close(serverSocketFd);
            serverSocketFd = -1;
            return false;
        }

        return true;
    }

    bool GolServer::listenSocket()
    {
        if(serverSocketFd < 0) return false;

        if(listen(serverSocketFd, 1) == -1)
        {
            close(serverSocketFd);
            serverSocketFd = -1;
            return false;
        }

        return true;
    }

    bool GolServer::acceptClient()
    {
        size_t clientAddrLen = sizeof(clientAddr);
        clientSocketFd = accept(serverSocketFd, (sockaddr*)&clientAddr, &clientAddrLen);
        if(clientSocketFd == -1) return false;

        return true;
    }

    bool GolServer::closeSocket()
    {
        if(close(serverSocketFd) < 0) return false;
        serverSocketFd = -1;

        return true;
    }

    void GolServer::setAddr()
    {
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(cfg->server_port);
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        return;
    }
}
