#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "protocol.h"



class MsgHandler
{
public:
    MsgHandler();
    PDU* handleMsg(PDU* msgPDU);
    PDU* RegistMsg(PDU* pdu);
    PDU* LoginMsg(PDU* pdu);
    PDU* FindFriendMsg(PDU* pdu);
    PDU* OnlineUsersMsg();
};

#endif // MSGHANDLER_H
