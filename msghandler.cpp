#include "msghandler.h"
#include "database.h"
#include "logger.h"

MsgHandler::MsgHandler()
{

}



PDU* MsgHandler::handleMsg(PDU* msgPDU)
{
    PDU* resPDU = nullptr;
    switch(msgPDU->msgType) {
        case EnMsgType::REGIST_MSG : {

           resPDU = RegistMsg(msgPDU);
           break;
        }

        case EnMsgType::LOGIN_MSG : {
            resPDU = LoginMsg(msgPDU);
            break;

        }
        case EnMsgType::FIND_FRIEND_MSG : {

            resPDU = FindFriendMsg(msgPDU);
            break;
        }

        case EnMsgType::GET_ONLINE_USERS_MSG : {

            resPDU = OnlineUsersMsg();
            break;
        }
        default :{
            INFO << "未知消息";
        }
    }
    return resPDU;
}


PDU * MsgHandler::RegistMsg(PDU *pdu)
{
    bool ret = Database::getInstance().registHandle(pdu->data, pdu->data + 32);
    PDU* respone_pdu = makePDU(0);
    respone_pdu->msgType = EnMsgType::REGIST_RESPOND;
    memcpy(respone_pdu->data, &ret, sizeof(ret));
    return respone_pdu;
}

PDU *MsgHandler::LoginMsg(PDU *pdu)
{
    bool ret = Database::getInstance().loginHandle(pdu->data, pdu->data + 32);
    PDU* respone_pdu = makePDU(0);
    respone_pdu->msgType = EnMsgType::LOGIN_RESPOND;
//    if(ret) sock->set_name(pdu->data);
    memcpy(respone_pdu->data, &ret, sizeof(ret));
    return respone_pdu;
}

PDU *MsgHandler::FindFriendMsg(PDU *pdu)
{
    int ret = Database::getInstance().findUserHandle(pdu->data);
    PDU* respone_pdu = makePDU(0);
    respone_pdu->msgType = EnMsgType::FIND_FRIEND_RESPONE;
    memcpy(respone_pdu->data, &ret, sizeof(ret));
    return respone_pdu;
}

PDU *MsgHandler::OnlineUsersMsg()
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

