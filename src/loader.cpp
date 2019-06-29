#include "loader.h"

const int FD_MIN = 50;
const int FD_MAX = 1000;
const double THRESHOLD = 0;

Loader::Loader(QObject *parent) : QThread(parent)
{
  restart = false;
  abort = false;
}

Loader::~Loader()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void Loader::load(QFileInfoList aList) {
  list = aList;
  condition.wakeOne();
}

void Loader::run()
{
  forever {
    mutex.lock();
    condition.wait(&mutex);

    //int i = 0;
    int imageId = 0;
    for (const auto& i : list)
    {
      QImage image;
      image.load(i.canonicalFilePath());
      emit detect(imageId, FD_MIN, FD_MAX, THRESHOLD, image);
      emit renderedImage(imageId, image);

      // Update progress bar

      imageId++;
      break;
    }

    //condition.wait(&mutex);
    mutex.unlock();
  }

  int i = 0;
}