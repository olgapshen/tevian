//#include <QImage>

#include "renderthread.h"

RenderThread::RenderThread(QObject *parent)
    : QThread(parent)
{
    restart = false;
    abort = false;

    // for (int i = 0; i < ColormapSize; ++i)
    //     colormap[i] = rgbFromWaveLength(380.0 + (i * 400.0 / ColormapSize));
}

RenderThread::~RenderThread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void RenderThread::startThread() {
  if (!isRunning()) {
    start(LowPriority);
  }
}

void RenderThread::handleList(QFileInfoList aList) {
  list = aList;
  //list = &aList;
  condition.wakeOne();
}

// void RenderThread::render(QImage &image) {
//   QMutexLocker locker(&mutex);

//   imageToShow = &image;
//   //isUpdated = true;
//   condition.wakeOne();
//     // this->centerX = centerX;
//     // this->centerY = centerY;
//     // this->scaleFactor = scaleFactor;
//     // this->resultSize = resultSize;

//     // if (!isRunning()) {
//     //     start(LowPriority);
//     // } else {
//     //     restart = true;
//     //     condition.wakeOne();
//     // }
// }

void RenderThread::run()
{
  forever {
    mutex.lock();
    // QSize resultSize = this->resultSize;
    // double scaleFactor = this->scaleFactor;
    // double centerX = this->centerX;
    // double centerY = this->centerY;
    // mutex.unlock();
    
    // int halfWidth = resultSize.width() / 2;
    // int halfHeight = resultSize.height() / 2;
    // QImage image(resultSize, QImage::Format_RGB32);

    // const int NumPasses = 8;
    // int pass = 0;
    
    // while (pass < NumPasses) {
    //   const int MaxIterations = (1 << (2 * pass + 6)) + 32;
    //   const int Limit = 4;
    //   bool allBlack = true;

    //   for (int y = -halfHeight; y < halfHeight; ++y) {
    //     if (restart)
    //       break;
    //     if (abort)
    //       return;

    //     uint *scanLine =
    //       reinterpret_cast<uint *>(image.scanLine(y + halfHeight));
    //     double ay = centerY + (y * scaleFactor);

    //     for (int x = -halfWidth; x < halfWidth; ++x) {
    //       double ax = centerX + (x * scaleFactor);
    //       double a1 = ax;
    //       double b1 = ay;
    //       int numIterations = 0;

    //       do {
    //         ++numIterations;
    //         double a2 = (a1 * a1) - (b1 * b1) + ax;
    //         double b2 = (2 * a1 * b1) + ay;
    //         if ((a2 * a2) + (b2 * b2) > Limit)
    //           break;

    //         ++numIterations;
    //         a1 = (a2 * a2) - (b2 * b2) + ax;
    //         b1 = (2 * a2 * b2) + ay;
    //         if ((a1 * a1) + (b1 * b1) > Limit)
    //           break;
    //       } while (numIterations < MaxIterations);

    //       if (numIterations < MaxIterations) {
    //         *scanLine++ = colormap[numIterations % ColormapSize];
    //         allBlack = false;
    //       } else {
    //         *scanLine++ = qRgb(0, 0, 0);
    //       }
    //     }
    //   }

    //   if (allBlack && pass == 0) {
    //     pass = 4;
    //   } else {
    //     if (!restart)
    //       emit renderedImage(image, scaleFactor);
    //     ++pass;
    // }
    // }
    
    // mutex.lock();
    // if (!restart)
    //     condition.wait(&mutex);
    // restart = false;
    condition.wait(&mutex);

    int i = 1;

    for (const auto& i : list )
    {
      QImage myImage;
      myImage.load(i.canonicalFilePath());
      renderedImage(myImage, 1);
      QThread::sleep(1);

      //update();
      //thread.render(myImage);

      //image->show();
      //this->repaint();
      //qDebug() << i.canonicalFilePath();
    }

    mutex.unlock();
  }
}