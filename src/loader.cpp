#include "loader.h"

#include <QDir>

const int FD_MIN = 50;
const int FD_MAX = 500;
const double THRESHOLD = 0.2;

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

void Loader::load(QFileInfoList aList) {
  int amount = aList.count();
  if (amount == 0)
    emit setError("No input");
  else {
    emit prepare();
    emit counted(amount);
    list = aList;
    aborted = false;
    condition.wakeOne();
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
      image.load(i.canonicalFilePath());
      emit loaded(imageId, image);
      emit detect(imageId, FD_MIN, FD_MAX, THRESHOLD, image);
      imageId++;
    }

    mutex.unlock();
  }
}