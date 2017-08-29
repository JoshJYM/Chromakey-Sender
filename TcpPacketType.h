#ifndef TCPPACKETTYPE_H
#define TCPPACKETTYPE_H

#include "widget.h"

#define MAX_SERVER_NUMBER 100

#define PACKET_SIZE 256
#define HEADER_SIZE 8

#define TCP_CONNECTION_PORT      1500
#define TCP_CONNECTION_ADDRESS   "172.16.100.166"
//#define TCP_CONNECTION_ADDRESS   "61.43.139.140"

#define TCP_CHAT_CONNECTION_PORT      2500
#define TCP_CHAT_CONNECTION_ADDRESS   "172.16.100.166"
//#define TCP_CHAT_CONNECTION_ADDRESS   "61.43.139.140"
enum PacketOrder
{
    Order_Header_A,
    Order_Header_W,
    Order_Header_T,
    Order_Header_V,
    Order_Header_NULL,
    Order_MajorCate,
    Order_MinorCate,
    Order_Reserved,
    Order_Data
};
enum MajorCate
{
    Major_Login,
    Major_BroadcastSettring,
    Major_BroadcastView,
    Major_Chatting
};
enum LoginCate
{
    Login_Connect,
    Login_Request,
    Login_Success,
    Login_Fail,
    Login_Fail_Dupli
};
enum BroadSetCate
{
    BroadSet_Ok,
    BroadSet_Error,
    BroadSet_Start,
    BroadSet_Stop,
    BroadSet_Request_All,
    BroadSet_All,
    BroadSet_Modify,
    BroadSet_MusicInfo,
    BroadSet_AdvertiseInfo
};
enum ChattingVate
{
    Chatting_Connect,
    Chatting_Error,
    Chatting_Join,
    Chatting_Exit,
    Chatting_Chat,
    Chatting_Kick,
    Chatting_Ban,
    Chatting_Kicked,
    Chatting_Banned
};

#endif // TCPPACKETTYPE_H
