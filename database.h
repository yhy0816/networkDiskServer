#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>

class Database : QObject
{
    Q_OBJECT
public:
    static Database& getInstance();
    ~Database();
    bool connect();
    //
    bool loginHandle(const char* name, const char* pwd);
    bool registHandle(const char* name, const char* pwd);
    void offlinehandle(const char* name);
    int findUserHandle(const char* name);
    QStringList findOnlineUserHandle();
    int friendhandle(const char* curname, const char* tarname);
    void agreeFriendHandle(const char* curname, const char* tarname);
    QStringList getFriendsHandle(const char* curname);
    int removeFriendsHandle(const char *curname, const char *tarname);
private:
    Database();
    Database(const Database&) = delete;
    Database& operator= (const Database&) = delete;
    QSqlDatabase m_dbhandler;
};

#endif // DATABASE_H
