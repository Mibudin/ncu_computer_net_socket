#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string>
#include"client.hpp"
#include"config.hpp"


namespace gol
{
    GolClient::GolClient()
    {
        clientSocketFd = -1;

        setAddr();
    }

    bool GolClient::createSocket()
    {
        clientSocketFd = socket(PF_INET, SOCK_STREAM, 0);
        if(clientSocketFd == -1) return false;

        return true;
    }

    bool GolClient::connectServer()
    {
        int conSuc = connect(clientSocketFd, (sockaddr*)&serverAddr, sizeof(serverAddr));
        if(conSuc == -1)
        {
            close(clientSocketFd);
            clientSocketFd = -1;
            return false;
        }

        return true;
    }

    bool GolClient::closeSocket()
    {
        if(close(clientSocketFd) < 0) return false;
        clientSocketFd = -1;

        return true;
    }

    void GolClient::setAddr()
    {
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(cfg->server_port);

        inet_pton(AF_INET, gol::cfg->server_ip_addr.c_str(),
            &serverAddr.sin_addr.s_addr);

        return;
    }
}
