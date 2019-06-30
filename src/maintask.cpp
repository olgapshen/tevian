#include <maintask.h>

#include <iostream>
#include <loader.h>
#include <requester.h>

#include <QDebug>
#include <QJsonDocument>

using namespace std;

MainTask::MainTask(QObject *parent) : QObject(parent) {
  Loader *loader = new Loader(this);
  Requester *requester = new Requester(this);
  processed = 0;
  
  // loader->start();
  // loader->abort();
  // connect(this, &MainTask::start, this, &MainTask::finished);
  // return;
  loader->start();

  connect(loader, &Loader::counted, [this](int aAmount) { 
    cout << "Counted: " << to_string(aAmount) << endl;
    amount = aAmount; 
  });

  connect(this, &MainTask::stop, [loader]() { 
    loader->abort();
    //loader->quit();
    //loader->wait();
  });

  connect(loader, &Loader::finished, [this]() { 
    emit finished(); 
  });

  connect(this, &MainTask::handleDir, loader, &Loader::handleDir);
  connect(this, &MainTask::handleFiles, loader, &Loader::handleFiles);
  connect(this, &MainTask::start, loader, &Loader::process);
  connect(loader, &Loader::loaded, this, &MainTask::loaded);
  connect(loader, &Loader::setError, this, &MainTask::setError);
  connect(loader, &Loader::detect, requester, &Requester::detect);
  connect(this, &MainTask::setFdMax, requester, &Requester::setFdMax);
  connect(this, &MainTask::setFdMin, requester, &Requester::setFdMin);
  connect(this, &MainTask::setThreshold, requester, &Requester::setThreshold);
  connect(this, &MainTask::setToken, requester, &Requester::setToken);
  connect(requester, &Requester::onSuccess, this, &MainTask::restSuccess);
  connect(requester, &Requester::onError, this, &MainTask::restFailed);
}

void MainTask::setError(QString error) {
  qDebug() << error;
  qDebug() << "on error finished";
  emit stop();
}

void MainTask::onStep() {
  processed++;
  
  if (processed == amount) {
    qDebug() << "successfull finished";
    emit stop();
  } 
}

void MainTask::printResponse(int imageId, const QJsonObject& response) {
  cout << paths[imageId].toStdString() << endl;
  QJsonDocument doc(response);
  QString strJson(doc.toJson(QJsonDocument::Indented));
  cout << strJson.toStdString() << endl;
}

void MainTask::loaded(int imageId, const QString path, const QImage &image) {
  paths[imageId] = path;
}

void MainTask::restSuccess(int imageId, const QJsonObject& response) {
  printResponse(imageId, response);
  onStep();
}

void MainTask::restFailed(int imageId, const QJsonObject& response) {
  printResponse(imageId, response);
  qDebug() << "on error finished";
  emit stop();
}