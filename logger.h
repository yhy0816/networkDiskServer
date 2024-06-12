#ifndef LOGGER_H
#define LOGGER_H
#include <QDebug>
#include <QDateTime>
#define INFO (qDebug() << QDateTime::currentDateTime().toString() <<  __FILE__ << ":" << __LINE__)

#endif // LOGGER_H
