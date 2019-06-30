#include <maintask.h>

MainTask::MainTask(QObject *parent) : QObject(parent) {}

void MainTask::run() {
  emit finished();
}