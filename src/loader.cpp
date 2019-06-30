#include "loader.h"

#include <QDir>

Loader::Loader(QObject *parent) : QThread(parent)
{
  aborted = false;
}

Loader::~Loader()
{
    mutex.lock();
    aborted = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void Loader::abort() { aborted = true; }

void Loader::process() { 
  aborted = false; 
  condition.wakeOne();
}

void Loader::load(QFileInfoList aList) {
  int amount = aList.count();
  if (amount == 0)
    emit setError("No input");
  else {
    //emit prepare();
    emit counted(amount);
    list = aList;
  }
}

void Loader::handleFiles(QStringList files) {  
  QFileInfoList list;

  for (QString path : files) {
    QFileInfo info(path);
    if (info.isFile() && info.isReadable())
      list.append(info);
  }

  load(list);
}

void Loader::handleDir(QString path) {
  QDir dir(path);
  QStringList nameFilter;
  nameFilter << "*.png" << "*.jpeg" << "*.jpg";
  QFileInfoList list = dir.entryInfoList(nameFilter);
  load(list);
}

void Loader::run()
{
  forever {
    mutex.lock();
    condition.wait(&mutex);

    int imageId = 0;
    for (const auto& i : list)
    {
      if (aborted) break;

      QImage image;
      QString path = i.canonicalFilePath();
      image.load(path);
      emit loaded(imageId, path, image);
      emit detect(imageId, image);
      imageId++;
    }

    mutex.unlock();
  }
}