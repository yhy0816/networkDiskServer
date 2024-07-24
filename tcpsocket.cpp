#include "tcpsocket.h"
#include "protocol.h"
#include "logger.h"
#include "database.h"


TcpSocket::TcpSocket()
{
    connect(this, &QTcpSocket::readyRead, this, &TcpSocket::onReadyRead);
    connect(this, &QTcpSocket::disconnected, this, &TcpSocket::onClientClose);

}

PDU *TcpSocket::readPDU()
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
    return pdu;
}

void TcpSocket::sendPDU(PDU *pdu)
{
    if(pdu == nullptr) return;
    INFO << "send: ";
    m_msgHandler.printPDU(pdu);
    this->write(reinterpret_cast<char*>(pdu), pdu->totalLen);
    free(pdu);
}

void TcpSocket::onReadyRead()
{
    INFO << "可读消息长度 " << this->bytesAvailable();

    QByteArray data = this->readAll();

    buffer.append(data);

    while(buffer.size() >= static_cast<int>(sizeof(PDU))) {
        INFO << "loop" ;
        PDU* pdu = reinterpret_cast<PDU*>(buffer.data());
        int totalLen = pdu->totalLen;
        if(totalLen > buffer.size()) {
            break;
        }
        PDU* msgPDU = makePDU(pdu->msgLen);
        memcpy(msgPDU, pdu, pdu->totalLen);
        m_msgHandler.printPDU(msgPDU);
        PDU* resPDU = m_msgHandler.handleMsg(msgPDU);
        if(resPDU && resPDU->msgType == EnMsgType::LOGIN_RESPOND) {
            if(resPDU->data[0] == 1)this->set_name(msgPDU->data); // 如果登录成功
        }

        INFO << "send:  " << (int)resPDU->msgType;
        sendPDU(resPDU);

        buffer.remove(0, totalLen);
    }

//    PDU* msgPDU = readPDU();
//    PDU* resPDU = m_msgHandler.handleMsg(msgPDU);
//    if(resPDU && resPDU->msgType == EnMsgType::LOGIN_RESPOND) {
//        if(resPDU->data[0] == 1)this->set_name(msgPDU->data); // 如果登录成功
//    }

//    sendPDU(resPDU);
//    free(resPDU);
}

void TcpSocket::onClientClose()
{
    Database::getInstance().offlinehandle(m_userName.toStdString().c_str());
    emit clientClose(this);
}


