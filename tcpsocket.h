#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpSocket();
    QString get_userName() const {return m_userName;};
private:
    QString m_userName;

public slots:
    void onReadyRead();
    void onClientClose();

signals:

    void clientClose(TcpSocket* sock);
};

#endif // TCPSOCKET_H
