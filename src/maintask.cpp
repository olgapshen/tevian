#include <maintask.h>

#include <loader.h>
#include <requester.h>

MainTask::MainTask(QObject *parent) : QObject(parent) {
  Loader *loader = new Loader(this);
  Requester *requester = new Requester(this);

  connect(this, &MainTask::handleDir, loader, &Loader::handleDir);
  connect(this, &MainTask::handleFiles, loader, &Loader::handleFiles);
  //connect(loader, &Loader::detect, requester, &Requester::detect);
  //connect(requester, &Requester::onSuccess, )
}

// void MainTask::run() {
//   qDebug() << "run";
//   emit finished();
// }