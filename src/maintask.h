#ifndef _MAIN_TASK_H
#define _MAIN_TASK_H

#include <QtCore>

#include <map>

class MainTask : public QObject {
    Q_OBJECT

private:
  int amount;
  int processed;
  std::map<int, QString> paths;

  void onStep();
  void printResponse(int imageId, const QJsonObject& response);

public:
  MainTask(QObject *parent = 0);

private slots:
  void counted(int amount);
  void loaded(int imageId, const QString path, const QImage &image);
  void restSuccess(int imageId, const QJsonObject& response);
  void restFailed(int imageId, const QJsonObject& response);
  void setError(QString error);

signals:
  void handleFiles(QStringList dir);
  void handleDir(QString dir);
  void setToken(const QString &token);
  void setFdMin(int min);
  void setFdMax(int max);
  void setThreshold(double threshold);
  void start();
  void stop();
  void finished();
};

#endif // _MAIN_TASK_H