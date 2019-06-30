#ifndef _LOADER_H
#define _LOADER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QSize>
#include <QImage>
#include <QFileInfoList>

#include <requester.h>

class Loader : public QThread
{
  Q_OBJECT

private:
  QFileInfoList list;
  QMutex mutex;
  QWaitCondition condition;
  bool aborted;

public:
  Loader(QObject *parent = nullptr);
  ~Loader();

  void load(QFileInfoList list);
  void abort();

signals:
  void renderedImage(int imageId, const QImage &image);
  void detect(
    int imageId,
    int fd_min_size,
    int fd_max_size,
    double fd_threshold,
    QImage image
  );

protected:
  void run() override;

};

#endif // _LOADER_H