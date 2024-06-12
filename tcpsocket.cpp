#include "tcpsocket.h"
#include "protocol.h"
#include "logger.h"

TcpSocket::TcpSocket()
{
    connect(this, &QTcpSocket::readyRead, this, &TcpSocket::onReadyRead);
}

void TcpSocket::onReadyRead()
{
    INFO << "消息长度 " << this->bytesAvailable();
    uint totalLen = 0;
    this->read(reinterpret_cast<char*>(&totalLen), sizeof(totalLen)); // 先读取消息总长度
    uint MsgLen = totalLen - sizeof(PDU); // 计算出实际消息长度
    PDU *pdu = makePDU(MsgLen); // 创建 pdu 用于保存协议消息
    // 接受剩余协议内容

    this->read(reinterpret_cast<char*>(pdu) + sizeof(totalLen), totalLen - sizeof(totalLen));

    INFO << "总长度: " << totalLen ;
    INFO << "消息类型: " << (int)pdu->msgType;
    INFO << "消息长度" << pdu->msgLen;
    INFO << "消息内容" << pdu->msg;

    free(pdu);
}


