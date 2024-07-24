#include "server.h"
#include "logger.h"
#include <QFile>

Server::Server(QObject *parent) : QObject(parent)
{
    loadConf();
    TcpServer::getInstance().listen(QHostAddress(m_listenIp), m_listenPort);
}



Server &Server::getInstance()
{
    static Server server;
    return server;
}

void Server::loadConf() {
    QFile confFile(":/server.conf");
    if(!confFile.open(QIODevice::ReadOnly)) {
        INFO << "打开配置文件失败!!";
    }
    QString sport;
    QTextStream stream(&confFile);
    stream >> m_listenIp >> sport >> m_rootPath;

    m_listenPort = sport.toUShort();

    INFO << "listen " << m_listenIp << ":" << m_listenPort;
    confFile.close();
}
