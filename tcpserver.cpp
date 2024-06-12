#include "tcpserver.h"
#include "logger.h"
#include "tcpsocket.h"
void TcpServer::incomingConnection(qintptr handle)
{
    INFO << "新客户端连接";
    TcpSocket *newClient = new TcpSocket();
    newClient->setSocketDescriptor(handle);
    m_tcpSocketList.append(newClient);
}

TcpServer &TcpServer::getInstance()
{
    static TcpServer tcpServer;
    return tcpServer;
}

TcpServer::TcpServer()
{

}
