#include "database.h"
#include "logger.h"
#include <QSqlError>
#include <QSqlQuery>
#include "logger.h"

Database::Database()
{
    m_dbhandler = QSqlDatabase::addDatabase("QMYSQL");
}

Database &Database::getInstance()
{
    static Database ins;
    return ins;
}

bool Database::connect()
{
    m_dbhandler.setHostName("localhost");
    m_dbhandler.setPort(3306);
    m_dbhandler.setUserName("root");
    m_dbhandler.setPassword("123456");
    m_dbhandler.setDatabaseName("networkdisk");

    if(!m_dbhandler.open()) {
        INFO << m_dbhandler.lastError();
        return false;
    }
    INFO << "database connect successed!";
    return true;
}

bool Database::loginHandle(const char *name, const char *pwd)
{
    if(name == nullptr || pwd == nullptr) return false;
    QString sql = QString("select id from user where name = '%1' and pwd = '%2'").arg(name).arg(pwd);
    QSqlQuery query; // 通过获取程序的默认数据库来实现的
    INFO << "query " << sql;
    if(!query.exec(sql) || !query.next()) { // 查询失败或者说没有记录
        return false;
    }
    sql = QString("update user set online = 1 where name = '%1'").arg(name).arg(pwd);
    if(!query.exec(sql)) { // 更新在线状态失败
        return false;
    }

    return true;

}

bool Database::registHandle(const char* name, const char* pwd)
{
    if(name == nullptr || pwd == nullptr) return false;
    QString sql = QString("select id from user where name = '%1'").arg(name);
    QSqlQuery query; // 通过获取程序的默认数据库来实现的
    INFO << "query " << sql;
    if(!query.exec(sql) || query.next()) { // 查询失败或用户已存在

        return false;

    }
    sql = QString("insert into user(name, pwd) values('%1', '%2')").arg(name);
    INFO << "query " << sql;

    return query.exec(sql);
}

void Database::offlinehandle(const char *name)
{
    if(name == nullptr) return;
    QString sql = QString("update user set online = 0 where name = '%1'").arg(name);
    QSqlQuery query;
    INFO << "query " << sql;
    query.exec(sql);
}

int Database::findUserHandle(const char *name)
{
    if(name == nullptr) return -1;
    QString sql = QString("select online from user where name = '%1'").arg(name);
    QSqlQuery query;
    INFO << "query " << sql;
    if(!query.exec(sql) || !query.next()) return -1; // 不存在这个用户
    return query.value(0).toInt();
}

QStringList Database::findOnlineUserHandle()
{
    QString sql = QString("select name from user where online = 1");
    QSqlQuery query;
    if(query.exec(sql) == false) return {};
    INFO << "query " << sql;
    QStringList list;
    while(query.next()) {
        list.append(query.value(0).toString());
    }
    return list;
}

Database::~Database()
{
    m_dbhandler.close();
}
