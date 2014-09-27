#include <QCoreApplication>

#include "handler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    handler *myHandler = new handler();

    return a.exec();
}
