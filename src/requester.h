#ifndef _REQUESTER_H
#define _REQUESTER_H

#include <QObject>
#include <QBuffer>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QImage>
#include <functional>

class Requester : public QObject
{
    Q_OBJECT
public:
  static const QString KEY_QNETWORK_REPLY_ERROR;
  static const QString KEY_CONTENT_NOT_FOUND;

  explicit Requester(QObject *parent = 0);

private:
  static const QString httpsTemplate;
  QString token;
  int fdMin;
  int fdMax;
  double threshold;

  QNetworkRequest createRequest(const QString &apiStr, const QUrlQuery &query);
  QJsonObject parseReply(QNetworkReply *reply);
  bool onFinishRequest(QNetworkReply *reply);
  void handleQtNetworkErrors(QNetworkReply *reply, QJsonObject &obj);
  QNetworkAccessManager *manager;

signals:
  void ready();
  void onSuccess(int imageId, const QJsonObject& response);
  void onError(int imageId, const QJsonObject& response);

public slots:
  void setToken(const QString &token);
  void setFdMin(int min);
  void setFdMax(int max);
  void setThreshold(double threshold);
  void detect(
    int imageId,
    // int fd_min_size,
    // int fd_max_size,
    // double fd_threshold,
    QImage image
  );
};

#endif // _NETWORK_REQUEST_H