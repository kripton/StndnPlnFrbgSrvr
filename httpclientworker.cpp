#include "httpclientworker.h"

httpclientworker::httpclientworker(QObject *parent) :
    QObject(parent)
{
    mutex = new QMutex();
}

void httpclientworker::doWork()
{
    manager = new QNetworkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl("http://www.h-ab.de/fileadmin/dokumente/stundenplan/klassisch/KuD13.html")));
}

void httpclientworker::replyFinished(QNetworkReply *myreply)
{
    // Save the reply's body
    QByteArray replyBody = myreply->readAll();
    QXmlStreamReader xr;
    QXmlStreamReader xr1;
    QString currentId;
    QMap<QString, bool> idMatch;
    QString html = QString("");
    QXmlStreamWriter xw(&html);
    bool match = false;

    // Loop through the reply once to create the map of which Ids match the requested group
    xr1.setNamespaceProcessing(false);
    xr1.addData(replyBody);

    while (!xr1.atEnd()) {
        xr1.readNext();
        match = false;

        if (xr1.name() != "td")
        {
            continue;
        }

        foreach (QXmlStreamAttribute attr, xr1.attributes()) {
            if (attr.name() == "id")
            {
                match = true;
                currentId = attr.value().toString();
                idMatch[currentId] = 0;
                break;
            }
        }

        if (!match)
        {
            continue;
        }

        xr1.readNext();
        while (1) {
            if (xr1.text().contains("KuD 3 Gr. 2")) {
                idMatch[currentId] = 1;
            }
            xr1.readNext();
            if (xr1.name() == "td") break;
        }
        //qDebug() << "Id" << currentId << "Matched" << idMatch[currentId];
    }

    // Now loop through the reply again, writing the output
    xr.setNamespaceProcessing(false);
    xr.addData(replyBody);
    xw.setAutoFormatting(false);

    while (!xr.atEnd()) {
        xr.readNext();
        match = false;

        //qDebug() << "XML elem:" << xr.name() << "TEXT:" << xr.text();

        if (xr.name() != "td")
            //if (1)
        {
            xw.writeCurrentToken(xr);
            continue;
        }

        //xr.name() == "td"

        foreach (QXmlStreamAttribute attr, xr.attributes()) {
            //qDebug() << "ATTR" << attr.name() << "VAL" << attr.value();
            if (attr.name() == "id")
            {
                match = true;
                currentId = attr.value().toString();
            }
        }

        if (!match)
        {
            xw.writeCurrentToken(xr);
            continue;
        }

        // MATCH :D
        // Replaced td start element
        xw.writeStartElement(xr.namespaceUri().toString(), "td");
        xw.writeAttributes(xr.attributes());

        // Additional div start element
        xw.writeStartElement(xr.namespaceUri().toString(), "div");
        //qDebug() << "ID" << currentId << "Match" << idMatch[currentId];
        if (idMatch[currentId] == 1) {
            xw.writeAttribute("style", "background-color:#00FF00");
        //} else if (idMatch[currentId] == 0) {
        //    xw.writeAttribute("style", "background-color:#FF0000");
        }

        xr.readNext();
        while (1) {
            xw.writeCurrentToken(xr);
            xr.readNext();
            if (xr.name() == "td") break;
        }

        //qDebug() << "Matched" << idMatch[currentId];

        // End div
        xw.writeEndElement();

        // End replaced td
        xw.writeEndElement();
    }
    if (xr.hasError()) {
        qDebug() << "XML error:" << xr.errorString();
    }

    classHtml = html;
    mutex->unlock();
}
