#ifndef _RENDER_THREAD_H
#define _RENDER_THREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QSize>
#include <QImage>
#include <QFileInfoList>

class RenderThread : public QThread
{
  Q_OBJECT

public:
  RenderThread(QObject *parent = nullptr);
  ~RenderThread();

  void handleList(QFileInfoList list);
  //void startThread();
  //void render(QImage &image);

signals:
  void renderedImage(const QImage &image);

protected:
  void run() override;

private:
  QFileInfoList list;
  //uint rgbFromWaveLength(double wave);
  //QImage *imageToShow;
  //bool isUpdated;

  QMutex mutex;
  QWaitCondition condition;
  // double centerX;
  // double centerY;
  // double scaleFactor;
  // QSize resultSize;
  bool restart;
  bool abort;

  //enum { ColormapSize = 512 };
  //uint colormap[ColormapSize];
};

#endif // _RENDER_THREAD_H