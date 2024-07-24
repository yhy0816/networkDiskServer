#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include "tcpserver.h"
class Server : public QObject
{
    Q_OBJECT
public:
    static Server& getInstance();
    void loadConf();
    QString getRootpath() const {return m_rootPath;}
signals:
private:
    explicit Server(QObject *parent = nullptr);
    QString m_listenIp;
    uint16_t m_listenPort;
    QString m_rootPath;

};

#endif // SERVER_H
