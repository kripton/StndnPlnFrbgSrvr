#ifndef HTTPCLIENTWORKER_H
#define HTTPCLIENTWORKER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtXml/QXmlStreamReader>
#include <QtXml/QXmlStreamWriter>
#include <QMutex>

#include <QDebug>

class httpclientworker : public QObject
{
    Q_OBJECT
public:
    explicit httpclientworker(QObject *parent = 0);
    QNetworkRequest* request;
    QNetworkReply* reply;
    QMutex* mutex;
    QString classHtml;

private:
    QNetworkAccessManager* manager;

signals:

public slots:
    void doWork();
    void replyFinished(QNetworkReply* myreply);

};

#endif // HTTPCLIENTWORKER_H
