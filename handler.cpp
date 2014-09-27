#include "handler.h"

handler::handler(QObject *parent) :
    QObject(parent)
{
    server = new QHttpServer(this);
    connect(server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)), this, SLOT(handle(QHttpRequest*, QHttpResponse*)));

    // let's go
    server->listen(8300);
}

void handler::handle(QHttpRequest *request, QHttpResponse *response)
{
    QThread* thread = new QThread();
    client = new httpclientworker(NULL);
    client->moveToThread(thread);
    thread->start();
    client->mutex->lock();
    connect(this, SIGNAL(startRequest()), client, SLOT(doWork()), Qt::QueuedConnection);
    emit(startRequest());
    client->mutex->lock();

    response->setHeader("Content-Length", QString("%1").arg(client->classHtml.length()));
    response->writeHead(200); // everything is OK
    response->write(client->classHtml.toUtf8());
    response->end();

    client->deleteLater();
    client = NULL;
}
