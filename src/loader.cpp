#include "loader.h"

const int FD_MIN = 50;
const int FD_MAX = 1000;
const double THRESHOLD = 0;

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

void Loader::abort() {
  aborted = true;
}

void Loader::load(QFileInfoList aList) {
  list = aList;
  aborted = false;
  condition.wakeOne();
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
      emit detect(imageId, FD_MIN, FD_MAX, THRESHOLD, image);
      emit renderedImage(imageId, image);

      // Update progress bar

      imageId++;
      break;
    }

    mutex.unlock();
  }
}