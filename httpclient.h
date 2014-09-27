#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtXml/QXmlStreamReader>
#include <QtXml/QXmlStreamWriter>
#include <QMutex>
#include <QThread>

#include <QDebug>

class HttpClient : public QObject
{
    Q_OBJECT
public:
    explicit HttpClient(QObject *parent = 0);
    QString getHtml();

private:
    QNetworkAccessManager *manager;
    QString parseReply(QNetworkReply*);
    QString classHtml;
    QMutex mutex;
    QThread* thread;

private slots:
    void replyFinished(QNetworkReply*);
};

#endif // HTTPCLIENT_H
