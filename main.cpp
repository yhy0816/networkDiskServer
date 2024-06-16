#include <QCoreApplication>
#include "server.h"
#include "database.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Database::getInstance().connect();
    Server s;
    return a.exec();
}
