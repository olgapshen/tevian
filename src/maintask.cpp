#include <maintask.h>

MainTask::MainTask(QObject *parent) : QObject(parent) {}

void MainTask::run() {
  qDebug() << "run";
  emit finished();
}