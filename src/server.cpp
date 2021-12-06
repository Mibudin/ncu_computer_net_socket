#include<stdlib.h>
#include<string.h>
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
        connected = false;

        serverSocketFd = -1;
        clientSocketFd = -1;

        th = new Thread({nullptr, nullptr});
    
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
        connected = true;

        return true;
    }

    bool GolServer::closeSocket()
    {
        if(!connected) return false;

        if(close(serverSocketFd) < 0) return false;
        serverSocketFd = -1;
        connected = false;

        return true;
    }

    bool GolServer::closeClientSocket()
    {
        if(close(clientSocketFd) < 0) return false;
        clientSocketFd = -1;

        return true;
    }

    std::string GolServer::getClientIPAddr()
    {
        char ipAddr[20];
        inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, ipAddr, sizeof(ipAddr));
        printf("> %s\n", ipAddr);

        return std::string(ipAddr);
    }
    
    int GolServer::getClientPort()
    {
        return ntohs(clientAddr.sin_port);
    }

    // MsgPacket GolServer::getLastSendPkt()
    // {
    //     return lastSendPkt;
    // }

    // MsgPacket GolServer::getLastRecvPkt()
    // {
    //     return lastRecvPkt;
    // }

    bool GolServer::sendMsgPacket(const MsgPacket* pkt)
    {
        MsgPacket* newPkt = (MsgPacket*)malloc(sizeof(MsgPacket));
        memcpy(newPkt, pkt, sizeof(MsgPacket));
        int sendSuc = send(clientSocketFd, newPkt, sizeof(MsgPacket), 0);
        free(newPkt);

        if(sendSuc == -1) return false;
        else              return true;
    }

    MsgPacket* GolServer::recvMsgPacket()
    {
        MsgPacket* pkt = (MsgPacket*)malloc(sizeof(MsgPacket));
        int recvSuc = recv(clientSocketFd, pkt, sizeof(MsgPacket), 0);

        if(recvSuc == -1)
        {
            free(pkt);
            return nullptr;
        }
        else return pkt;
    }

    std::future<int>* GolServer::asyncSendMsgPacket(const MsgPacket* pkt)
    {
        lastSendPkt = *pkt;
        th->setTask({
            [](void* ths){
                return (int)((GolServer*)ths)->sendMsgPacket(
                    &(((GolServer*)ths)->lastSendPkt));},
            this
        });
        std::future<int>* f = th->nextPromise();
        th->storrs();

        return f;
    }

    std::future<int>* GolServer::asyncRecvMsgPacket()
    {
        th->setTask({
            [](void* ths){
                ((GolServer*)ths)->lastRecvPkt = *(((GolServer*)ths)->recvMsgPacket());
                return 0;},
            this
        });
        std::future<int>* f = th->nextPromise();
        th->storrs();

        return f;
    }

    bool GolServer::sendKey(const int key)
    {
        return sendMsgPacket((MsgPacket*)(new MsgPacket_Key(
            {gol::MsgType::MODE, key})));
    }

    bool GolServer::sendMode(const ModeType mode)
    {
        if(!connected) return false;
        return sendMsgPacket((MsgPacket*)(new MsgPacket_Mode(
            {gol::MsgType::MODE, mode})));
    }

    void GolServer::setAddr()
    {
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(cfg->server_port);
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        return;
    }
}
