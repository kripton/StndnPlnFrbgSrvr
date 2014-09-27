#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include "../qhttpserver/src/qhttpserver.h"
#include "../qhttpserver/src/qhttprequest.h"
#include "../qhttpserver/src/qhttpresponse.h"
#include "httpclientworker.h"
#include <QThread>

class handler : public QObject
{
    Q_OBJECT
public:
    explicit handler(QObject *parent = 0);

private:
    QHttpServer *server;
    httpclientworker *client;

signals:
    void startRequest();

public slots:
    void handle(QHttpRequest* request, QHttpResponse* response);
};

#endif // HANDLER_H
