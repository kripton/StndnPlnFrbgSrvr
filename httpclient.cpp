#include "httpclient.h"

HttpClient::HttpClient(QObject *parent) :
    QObject(parent)
{
}

QString HttpClient::getHtml()
{
    QThread* thread = new QThread;
    manager = new QNetworkAccessManager();
    manager->moveToThread(thread);
    qDebug() << "Connection" << connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    thread->start();

    manager->get(QNetworkRequest(QUrl("http://www.h-ab.de/fileadmin/dokumente/stundenplan/klassisch/KuD13.html")));
    return classHtml;
}

void HttpClient::replyFinished(QNetworkReply* reply)
{
    QXmlStreamReader xr;
    QString html = QString("");
    QXmlStreamWriter xw(&html);
    xr.setNamespaceProcessing(false);
    xr.setDevice(reply);

    bool match = false;
    bool match2 = false;
    QList<QString> between;

    while (!xr.atEnd()) {
          xr.readNext();
          match = false;

          //qDebug() << "XML elem:" << xr.name() << "TEXT:" << xr.text();

          if (xr.name() != "td")
          {
              xw.writeCurrentToken(xr);
              continue;
          }

          foreach (QXmlStreamAttribute attr, xr.attributes()) {
              //qDebug() << "ATTR" << attr.name() << "VAL" << attr.value();
              if (attr.name() == "id")
              {
                  match = true;
              }
          }

          if (!match)
          {
              xw.writeCurrentToken(xr);
              continue;
          }

          // MATCH :D
          between = QList<QString>();

          xw.writeStartElement("td");
          xw.writeAttributes(xr.attributes());

          // Text handling
          match2 = false;
          xr.readNext();
          qDebug() << "NextText:" << xr.text();
          between.append(xr.text().toString());
          while (xr.name() != "td")
          {
              xr.readNext();
              qDebug() << "LoopText:" << xr.text() << "Name:" << xr.name();
              if (!xr.text().isEmpty())
              {
                  between.append(xr.text().toString());
                  if (xr.text().contains("KuD 3 Gr. 2")) {
                      match2 = true;
                  }
              } /*else if (xr.tokenType() == QXmlStreamReader::EntityReference) {
                  if (xr.name() == "auml") between.append("ä");
                  else if (xr.name() == "uuml") between.append("ü");
                  else if (xr.name() == "ouml") between.append("ö");
                      break;

                  case "ouml":
                      between.append("ö");
                      break;

                  case "uuml":
                      between.append("ü");
                      break;
                  }
              }*/
          }
          if (match2) {
              xw.writeStartElement("div");
              xw.writeAttribute("style", "background-color:#00FF00");
          }
          foreach (QString str, between)
          {
              xw.writeCharacters(str);
              xw.writeEmptyElement("br");
          }
          if (match2) {
              xw.writeEndElement();
          }
          xw.writeEndElement();
    }
    if (xr.hasError()) {
        qDebug() << "XML error:" << xr.errorString();
    }

    qDebug() << html;
    classHtml = html;
    mutex.unlock();
}
