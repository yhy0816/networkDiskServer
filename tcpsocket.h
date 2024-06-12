#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpSocket();

public slots:
    void onReadyRead();
};

#endif // TCPSOCKET_H
