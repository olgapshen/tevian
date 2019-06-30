#ifndef _MAIN_TASK_H
#define _MAIN_TASK_H

#include <QtCore>

class MainTask : public QObject {
    Q_OBJECT

private:
  int amount;

  void restSuccess(int imageId, const QJsonObject& response);
  void restFailed(int imageId, const QJsonObject& response);

public:
  MainTask(QObject *parent = 0);// : QObject(parent) {}

signals:
  void handleFiles(QStringList dir);
  void handleDir(QString dir);
  void finished();
};

#endif // _MAIN_TASK_H