#include<string.h>
#include<stdlib.h>
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
        connected = false;

        clientSocketFd = -1;

        th = new Thread({nullptr, nullptr});

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
            // closeSocket();
            return false;
        }
        connected = true;

        return true;
    }

    bool GolClient::closeSocket()
    {
        if(!connected) return false;

        if(close(clientSocketFd) < 0) return false;
        clientSocketFd = -1;
        connected = false;

        return true;
    }

    std::string GolClient::getServerIPAddr()
    {
        return cfg->server_ip_addr;
    }
    
    int GolClient::getServerPort()
    {
        return cfg->server_port;
    }

    bool GolClient::sendMsgPacket(const MsgPacket* pkt)
    {
        MsgPacket* newPkt = (MsgPacket*)malloc(sizeof(MsgPacket));
        memcpy(newPkt, pkt, sizeof(MsgPacket));
        int sendSuc = send(clientSocketFd, newPkt, sizeof(MsgPacket), 0);
        free(newPkt);

        if(sendSuc == -1) return false;
        else              return true;
    }

    MsgPacket* GolClient::recvMsgPacket()
    {
        MsgPacket* pkt = (MsgPacket*)malloc(sizeof(MsgPacket));
        int recvSuc = recv(clientSocketFd, pkt, sizeof(MsgPacket), 0);

        if(recvSuc == -1)
        {
            free(pkt);
            return nullptr;
        }
        else
        {
            memcpy(&lastRecvPkt, pkt, sizeof(MsgPacket));
            return pkt;
        }
    }

    std::future<int>* GolClient::asyncRecvMsgPacket()
    {
        th->setTask({
            [](void* ths){
                ((GolClient*)ths)->lastRecvPkt = *(((GolClient*)ths)->recvMsgPacket());
                return 0;},
            this
        });
        std::future<int>* f = th->nextPromise();
        th->storrs();

        return f;
    }

    bool GolClient::checkMode(const ModeType mode)
    {
        if(!connected) return false;

        gol::MsgPacket_Mode* pkt;
        while(true)
        {
            pkt = (gol::MsgPacket_Mode*)(recvMsgPacket());
            if(pkt->type == gol::MsgType::MODE
                && pkt->mode == mode) break;
        }

        return true;
    }

    bool GolClient::sendKey(const int key)
    {
        while(!sendMsgPacket((MsgPacket*)(new MsgPacket_Key(
            {gol::MsgType::KEY, key}))));
        return true;
    }

    int GolClient::recvKey()
    {
        MsgPacket* pkt;
        while(true)
        {
            pkt = recvMsgPacket();
            if(pkt == nullptr) return -1;
            if(pkt->type == MsgType::KEY)
            {
                int key = ((MsgPacket_Key*)pkt)->key;
                free(pkt);
                return key;
            }
        }
        return -1;
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
