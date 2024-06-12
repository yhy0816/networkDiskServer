#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QList>
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
    QList<QTcpSocket*> m_tcpSocketList;

};

#endif // TCPSERVER_H
