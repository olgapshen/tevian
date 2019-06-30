#include "requester.h"

#include <QUrlQuery>
#include <QDebug>
#include <QThread>

using namespace std;

const QString Host = "backend.facecloud.tevian.ru";

const QString Requester::httpsTemplate = "https://%1/%2";
const QString Requester::KEY_QNETWORK_REPLY_ERROR = "QNetworkReplyError";
const QString Requester::KEY_CONTENT_NOT_FOUND = "ContentNotFoundError";

Requester::Requester(QObject *parent) : QObject(parent)
{
  manager = new QNetworkAccessManager(this);
}

QString Requester::getToken() const { return token; }
void Requester::setToken(const QString &value) { 
  token = value;
  emit ready();
}

void Requester::detect(
  int imageId,
  int fd_min_size,
  int fd_max_size,
  double fd_threshold,
  QImage image
) {
  QUrlQuery query;

  query.addQueryItem("fd_min_size", QString::number(fd_min_size));
  query.addQueryItem("fd_max_size", QString::number(fd_max_size));
  query.addQueryItem("fd_threshold", QString::number(fd_threshold));
  query.addQueryItem("demographics", "true");

  QNetworkRequest request = createRequest("api/v1/detect", query);
  QNetworkReply *reply;

  //QImage img_enrll;
  QByteArray arr;
  QBuffer buffer(&arr);
  buffer.open(QIODevice::WriteOnly);
  image.save(&buffer, "JPEG");

  reply = manager->post(request, arr);

  connect(
    reply, 
    &QNetworkReply::finished, 
    this,
    [this, imageId, reply]() 
    {
      QJsonObject obj = parseReply(reply);

      if (onFinishRequest(reply)) {
        onSuccess(imageId, obj);
      } else {
        handleQtNetworkErrors(reply, obj);
        onError(imageId, obj);
      }

      reply->close();
      reply->deleteLater();
    }
  );
}

QNetworkRequest Requester::createRequest(const QString &apiStr, const QUrlQuery &query)
{
  QNetworkRequest request;
  QString strUrl = httpsTemplate.arg(Host).arg(apiStr);

  QUrl url(strUrl);
  url.setQuery(query);
  request.setUrl(url);

  request.setRawHeader("Content-Type","image/jpeg");
  request.setRawHeader("accept","application/json");
  request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toUtf8());    

  return request;
}

QJsonObject Requester::parseReply(QNetworkReply *reply)
{
  QJsonObject jsonObj;
  QJsonDocument jsonDoc;
  QJsonParseError parseError;
  auto replyText = reply->readAll();
  jsonDoc = QJsonDocument::fromJson(replyText, &parseError);

  if(parseError.error != QJsonParseError::NoError){
    qDebug() << replyText;
    qWarning() << "Json parse error: " << parseError.errorString();
  }else{
    if(jsonDoc.isObject())
      jsonObj  = jsonDoc.object();
    else if (jsonDoc.isArray())
      jsonObj["non_field_errors"] = jsonDoc.array();
  }
  
  return jsonObj;
}

bool Requester::onFinishRequest(QNetworkReply *reply)
{
  auto replyError = reply->error();

  if (replyError == QNetworkReply::NoError ) {
    int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if ((code >=200) && (code < 300)) {
      return true;
    }
  }

  return false;
}

void Requester::handleQtNetworkErrors(QNetworkReply *reply, QJsonObject &obj)
{
    auto replyError = reply->error();
    if (!(replyError == QNetworkReply::NoError ||
          replyError == QNetworkReply::ContentNotFoundError ||
          replyError == QNetworkReply::ContentAccessDenied ||
          replyError == QNetworkReply::ProtocolInvalidOperationError
          ) ) {
        qDebug() << reply->error();
        obj[KEY_QNETWORK_REPLY_ERROR] = reply->errorString();
    } else if (replyError == QNetworkReply::ContentNotFoundError)
        obj[KEY_CONTENT_NOT_FOUND] = reply->errorString();
}