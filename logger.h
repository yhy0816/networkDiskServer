#ifndef LOGGER_H
#define LOGGER_H
#include <QDebug>
#include <QDateTime>
#define INFO (qDebug() << QDateTime::currentDateTime().toString("yyyy年MM月dd日 hh:mm:ss") <<  __FILE__ << ":" << __LINE__)

#endif // LOGGER_H
