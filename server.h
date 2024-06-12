#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include "tcpserver.h"
class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void loadConf();
signals:
private:
    QString m_listenIp;
    uint16_t m_listenPort;


};

#endif // SERVER_H
