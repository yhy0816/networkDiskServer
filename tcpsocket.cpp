#include "tcpsocket.h"
#include "protocol.h"
#include "logger.h"
#include "database.h"


TcpSocket::TcpSocket()
{
    connect(this, &QTcpSocket::readyRead, this, &TcpSocket::onReadyRead);
    connect(this, &QTcpSocket::disconnected, this, &TcpSocket::onClientClose);

}

void TcpSocket::onReadyRead()
{
    INFO << "可读消息长度 " << this->bytesAvailable();
    uint totalLen = 0;
    this->read(reinterpret_cast<char*>(&totalLen), sizeof(totalLen)); // 先读取消息总长度
    uint MsgLen = totalLen - sizeof(PDU); // 计算出实际消息长度
    PDU *pdu = makePDU(MsgLen); // 创建 pdu 用于保存协议消息
    // 接受剩余协议内容

    this->read(reinterpret_cast<char*>(pdu) + sizeof(totalLen), totalLen - sizeof(totalLen));

    INFO << "总长度: " << totalLen ;
    INFO << "消息类型: " << (int)pdu->msgType;
    INFO << "数据1" << pdu->data;
    INFO << "数据2" << pdu->data + 32;
    INFO << "消息长度" << pdu->msgLen;
    INFO << "消息内容" << pdu->msg;

    switch(pdu->msgType) {
        case EnMsgType::REGIST_MSG : {
            bool ret = Database::getInstance().registHandle(pdu->data, pdu->data + 32);
            PDU* respone_pdu = makePDU(0);
            respone_pdu->msgType = EnMsgType::REGIST_RESPOND;
            memcpy(respone_pdu->data, &ret, sizeof(ret));
            this->write(reinterpret_cast<char*>(respone_pdu), respone_pdu->totalLen);
            free(respone_pdu);
            break;
        }

        case EnMsgType::LOGIN_MSG : {
            bool ret = Database::getInstance().loginHandle(pdu->data, pdu->data + 32);
            PDU* respone_pdu = makePDU(0);
            respone_pdu->msgType = EnMsgType::LOGIN_RESPOND;
            if(ret) m_userName = pdu->data;
            memcpy(respone_pdu->data, &ret, sizeof(ret));
            this->write(reinterpret_cast<char*>(respone_pdu), respone_pdu->totalLen);
            free(respone_pdu);
            break;
        }
        case EnMsgType::FIND_FRIEND_MSG : {
            int ret = Database::getInstance().findUserHandle(pdu->data);
            PDU* respone_pdu = makePDU(0);
            respone_pdu->msgType = EnMsgType::FIND_FRIEND_RESPONE;
            memcpy(respone_pdu->data, &ret, sizeof(ret));
            this->write(reinterpret_cast<char*>(respone_pdu), respone_pdu->totalLen);
            free(respone_pdu);
            break;
        }

        case EnMsgType::GET_ONLINE_USERS_MSG : {
            QStringList ret = Database::getInstance().findOnlineUserHandle();
            PDU* respone_pdu = makePDU(ret.size() * 32);
            respone_pdu->msgType = EnMsgType::GET_ONLINE_USERS_RESPONE;
            for(int i = 0; i < ret.size(); i++) {
                std::string cur_s = ret.at(i).toStdString();
                memcpy(respone_pdu->msg + i * 32, cur_s.c_str(), cur_s.size());
            }
            this->write(reinterpret_cast<char*>(respone_pdu), respone_pdu->totalLen);
            free(respone_pdu);
            break;
        }
        default :{
            INFO << "未知消息";
        }


    }

    free(pdu);
}

void TcpSocket::onClientClose()
{
    Database::getInstance().offlinehandle(m_userName.toStdString().c_str());
    emit clientClose(this);
}


