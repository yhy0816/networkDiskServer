#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QList>
#include "tcpsocket.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    void incomingConnection(qintptr handle) override;
    static TcpServer& getInstance();



private:
    TcpServer();
    TcpServer(const TcpServer&) = delete;
    TcpServer& operator= (const TcpServer&) = delete;
    QList<TcpSocket*> m_tcpSocketList;

public slots:
    void removeSocket(TcpSocket* socket);
};

#endif // TCPSERVER_H
