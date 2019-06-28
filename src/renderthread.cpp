//#include <QImage>

#include "renderthread.h"

RenderThread::RenderThread(QObject *parent)
    : QThread(parent)
{
    restart = false;
    abort = false;
}

RenderThread::~RenderThread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void RenderThread::handleList(QFileInfoList aList) {
  list = aList;
  condition.wakeOne();
}

void RenderThread::run()
{
  forever {
    mutex.lock();
    condition.wait(&mutex);

    int i = 1;

    for (const auto& i : list )
    {
      QImage image;
      image.load(i.canonicalFilePath());

      // request REST
      // Update progress bar

      emit renderedImage(image);
    }

    mutex.unlock();
  }
}