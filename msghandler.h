#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "protocol.h"



class MsgHandler
{
public:
    MsgHandler();
    PDU* handleMsg(PDU* msgPDU);
    PDU* registMsg(PDU* pdu);
    PDU* loginMsg(PDU* pdu);
    PDU* findFriendMsg(PDU* pdu);
    PDU* onlineUsersMsg();
    PDU* addFriendMsg(PDU *pdu);
    PDU* agreeFriendMsg(PDU *pdu);
    PDU* getFriendsMsg(PDU *pdu);
    PDU* removeFriendsMsg(PDU *pdu);
    void ChatMsg(PDU *pdu);
};

#endif // MSGHANDLER_H
