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


