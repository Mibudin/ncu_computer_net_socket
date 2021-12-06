#pragma once
#ifndef MSGPKT_HPP
#define MSGPKT_HPP

#include"cell.hpp"
#include"textarea.hpp"


namespace gol
{
    enum class MsgType : int
    {
        TEMP = 0, CELL, KEY, MODE
    };

    struct MsgPacket
    {
        MsgType type = MsgType::TEMP;
        char _rest[12];
    };

    struct MsgPacket_Cell
    {
        MsgType type = MsgType::CELL;
        int x;
        int y;
        CellStatus status;
        char _rest[0];
    };

    struct MsgPacket_Key
    {
        MsgType type = MsgType::KEY;
        int key;
        char _rest[8];
    };

    struct MsgPacket_Mode
    {
        MsgType type = MsgType::MODE;
        ModeType mode;
        char _rest[8];
    };

    // union MsgPacket
    // {
    //     MsgPacket_Temp temp;
    //     MsgPacket_Cell cell;
    //     MsgPacket_Key  key;
    //     MsgPacket_Mode mode;
    // };

    // void _()
    // {
    //     sizeof(MsgPacket);
    //     sizeof(MsgPacket_Cell);
    //     sizeof(MsgPacket_Key);
    //     sizeof(MsgPacket_Mode);
    //     // sizeof(MsgPacket);
    // }
}

#endif
