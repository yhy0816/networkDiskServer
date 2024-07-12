#include "msghandler.h"
#include "database.h"
#include "logger.h"
#include "tcpserver.h"

MsgHandler::MsgHandler()
{

}



PDU* MsgHandler::handleMsg(PDU* msgPDU)
{
    PDU* resPDU = nullptr;
    switch(msgPDU->msgType) {
        case EnMsgType::REGIST_MSG : {

           resPDU = registMsg(msgPDU);
           break;
        }

        case EnMsgType::LOGIN_MSG : {
            resPDU = loginMsg(msgPDU);
            break;

        }
        case EnMsgType::FIND_FRIEND_MSG : {

            resPDU = findFriendMsg(msgPDU);
            break;
        }

        case EnMsgType::GET_ONLINE_USERS_MSG : {

            resPDU = onlineUsersMsg();
            break;
        }
        case EnMsgType::ADD_FRIEND_MSG : {

            resPDU = addFriendMsg(msgPDU);
            break;
        }
        case EnMsgType::AGREE_FRIEND_MSG : {

            resPDU = agreeFriendMsg(msgPDU);
            break;
        }
        case EnMsgType::GET_FRIENDS_MSG : {

            resPDU = getFriendsMsg(msgPDU);
            break;
        }
        case EnMsgType::REMOVE_FRIENDS_MSG : {

            resPDU = removeFriendsMsg(msgPDU);
            break;
        }
        case EnMsgType::CHAT_MSG : {

            ChatMsg(msgPDU);
            break;
        }
        default :{
            INFO << "未知消息 " << (int)msgPDU->msgType;
        }
    }
    return resPDU;
}


PDU * MsgHandler::registMsg(PDU *pdu)
{
    bool ret = Database::getInstance().registHandle(pdu->data, pdu->data + 32);
    PDU* respone_pdu = makePDU(0);
    respone_pdu->msgType = EnMsgType::REGIST_RESPOND;
    memcpy(respone_pdu->data, &ret, sizeof(ret));
    return respone_pdu;
}

PDU *MsgHandler::loginMsg(PDU *pdu)
{
    bool ret = Database::getInstance().loginHandle(pdu->data, pdu->data + 32);
    PDU* respone_pdu = makePDU(0);
    respone_pdu->msgType = EnMsgType::LOGIN_RESPOND;
//    if(ret) sock->set_name(pdu->data);
    memcpy(respone_pdu->data, &ret, sizeof(ret));
    return respone_pdu;
}

PDU *MsgHandler::findFriendMsg(PDU *pdu)
{
    int ret = Database::getInstance().findUserHandle(pdu->data);
    PDU* respone_pdu = makePDU(0);
    respone_pdu->msgType = EnMsgType::FIND_FRIEND_RESPONE;

    memcpy(respone_pdu->data, &ret, sizeof(ret));
    memcpy(respone_pdu->data + 32, pdu->data, 32);
    return respone_pdu;
}

PDU *MsgHandler::onlineUsersMsg()
{
    QStringList ret = Database::getInstance().findOnlineUserHandle();
    PDU* respone_pdu = makePDU(ret.size() * 32);
    respone_pdu->msgType = EnMsgType::GET_ONLINE_USERS_RESPONE;
    for(int i = 0; i < ret.size(); i++) {
        std::string cur_s = ret.at(i).toStdString();
        memcpy(respone_pdu->msg + i * 32, cur_s.c_str(), cur_s.size());
    }
    return respone_pdu;
}

PDU *MsgHandler::addFriendMsg(PDU *pdu)
{
    int ret = Database::getInstance().friendhandle(pdu->data, pdu->data + 32);
    if(ret == 0) {
        TcpServer::getInstance().sendTo(pdu->data + 32, pdu);
    }
    PDU* resPDU = makePDU(0);
    resPDU->msgType = EnMsgType::ADD_FRIEND_RESPONE;
    memcpy(resPDU->data, &ret, sizeof(ret));
    return resPDU;
}

PDU *MsgHandler::agreeFriendMsg(PDU *pdu)
{
    Database::getInstance().agreeFriendHandle(pdu->data, pdu->data + 32);
    PDU* resPDU = makePDU(0);
    resPDU->msgType = EnMsgType::AGREE_FRIEND_RESPONE;
    memcpy(resPDU->data, pdu->data, 64);
    TcpServer::getInstance().sendTo(resPDU->data, resPDU);
    return resPDU;
}

PDU *MsgHandler::getFriendsMsg(PDU *pdu)
{
    QStringList list = Database::getInstance().getFriendsHandle(pdu->data);

    PDU* resPDU = makePDU(list.size() * 32);
    resPDU->msgType = EnMsgType::GET_FRIENDS_RESPONE;
    for(int i = 0; i < list.size(); i++) {
        std::string name = list.at(i).toStdString();
        memcpy(resPDU->msg + i * 32, name.c_str(), name.size());
    }
    return resPDU;
}

PDU *MsgHandler::removeFriendsMsg(PDU *pdu)
{
    int ret = Database::getInstance().removeFriendsHandle(pdu->data, pdu->data + 32);
    PDU* resPDU = makePDU(0);
    resPDU->msgType = EnMsgType::REMOVE_FRIENDS_RESPONE;
    memcpy(resPDU->data, &ret, sizeof(ret));
    return resPDU;
}

void MsgHandler::ChatMsg(PDU *pdu)
{
    TcpServer::getInstance().sendTo(pdu->data + 32, pdu);
}

