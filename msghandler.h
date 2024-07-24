#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "protocol.h"

#include <QString>



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
    PDU* makeDir(PDU *pdu);
    PDU* getFiles(PDU *pdu);
    PDU* delDir(PDU *pdu);
    PDU* delFile(PDU *pdu);
    PDU* renameFile(PDU *pdu);
    PDU* moveFile(PDU *pdu);
    void printPDU(PDU *pdu);

private:
    QString mergePath(PDU* pdu);
};

#endif // MSGHANDLER_H
