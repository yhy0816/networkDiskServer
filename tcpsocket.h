#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include "msghandler.h"
#include "protocol.h"

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TcpSocket();
    QString get_userName() const {return m_userName;}
    PDU* readPDU();
    void sendPDU(PDU* pdu);
    void set_name(const QString& name) {this->m_userName = name;}
private:
    QString m_userName;
    MsgHandler m_msgHandler;


public slots:
    void onReadyRead();
    void onClientClose();
signals:

    void clientClose(TcpSocket* sock);
};

#endif // TCPSOCKET_H
