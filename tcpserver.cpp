#include "tcpserver.h"
#include "logger.h"
#include "tcpsocket.h"

void TcpServer::incomingConnection(qintptr handle)
{
    INFO << "新客户端连接";
    TcpSocket *newClient = new TcpSocket();
    newClient->setSocketDescriptor(handle);
    connect(newClient, &TcpSocket::clientClose, this, &TcpServer::removeSocket);
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

void TcpServer::removeSocket(TcpSocket *socket)
{
    m_tcpSocketList.removeOne(socket);
    socket->deleteLater();
    socket = nullptr;
//    for(int i = 0; i < m_tcpSocketList.size(); i++) {
//        INFO << m_tcpSocketList.at(i)->get_userName();
//    }
}
