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
    sql = QString("insert into user(name, pwd) values('%1', '%2')").arg(name).arg(pwd);
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

int Database::friendhandle(const char *curname, const char *tarname)
{
    if(curname == nullptr || tarname == nullptr) {
        return -1;
    }

    // 先查询 tarname 是否在线
    QString sql = QString("select online from user where name = '%1'").arg(tarname);
    INFO << "query " << sql;
    QSqlQuery query;
    // 查询失败或
    if(query.exec(sql) == false) return -1;
    // 用户不存在
    if(query.next() == false) return 1;
    // 用户不在线
    if(query.value(0).toInt() == 0) return 2;

    // 查询两人是不是已经有好友关系
    sql = QString("select b.id from friend b "
                  "join user a on a.id = b.userid join user c on c.id = b.friendid "
                  "where a.name = '%1' and c.name = '%2' or a.name = '%2' and c.name = '%1'")
                .arg(curname).arg(tarname);
    INFO << "query " << sql;
    if(query.exec(sql) == false) return -1;
    // 有结果说明已经有好友关系
    if(query.next()) return 3;

    return 0;


}


void Database::agreeFriendHandle(const char *curname, const char *tarname)
{
    if(curname == nullptr || tarname == nullptr) {
        return;
    }
    QString sql = QString("insert into friend(userid, friendid) "
                          "select a.id, b.id from user a, user b "
                          "where a.name = '%1' and b.name = '%2'").
                            arg(curname).arg(tarname);
    QSqlQuery query;

    query.exec(sql);
    INFO << "query " << sql;
}

QStringList Database::getFriendsHandle(const char *curname)
{
    if(curname == nullptr) return {};
    QString sql = QString(
         R"sql(
            select name from user where id in (
                select friendid from friend where userid =  (select id from user where name = '%1')
                union
                select userid from friend where friendid =  (select id from user where name = '%1')
            ) and online = 1
           )sql").arg(curname);
    QSqlQuery query;
    INFO << "query " << sql;
    if(query.exec(sql) == false)  return {};

    QStringList list;
    while(query.next()) {
        list.append(query.value(0).toString());
    }
    return list;
}

int Database::removeFriendsHandle(const char *curname, const char *tarname)
{
    if(curname == nullptr || tarname == nullptr) return -1;
    QSqlQuery query;

    // 查询两人是不是已经有好友关系
    QString sql = QString("select b.id from friend b "
                  "join user a on a.id = b.userid join user c on c.id = b.friendid "
                  "where a.name = '%1' and c.name = '%2' or a.name = '%2' and c.name = '%1'")
                .arg(curname).arg(tarname);
    INFO << "query " << sql;
    if(query.exec(sql) == false) return -1;
    if(query.next() == false) return 0;
    sql = QString(
         R"sql(
                 delete from friend where
                     userid = (select id from user where name = '%1')
                     and
                     friendid = (select id from user where name = '%2')
                    OR
                     friendid = (select id from user where name = '%1')
                     and
                     userid = (select id from user where name = '%2')
           )sql").arg(curname).arg(tarname);


    INFO << "query " << sql;
    if(query.exec(sql) == false)  return -1;

    return 1;
}

Database::~Database()
{
    m_dbhandler.close();
}

