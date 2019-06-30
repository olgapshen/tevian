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

  void load(QFileInfoList list);

public:
  Loader(QObject *parent = nullptr);
  ~Loader();

signals:
  void loaded(int imageId, const QImage &image);
  void prepare();
  void counted(int amount);
  void setError(QString error);
  void detect(
    int imageId,
    int fd_min_size,
    int fd_max_size,
    double fd_threshold,
    QImage image
  );

public slots:
  void handleFiles(QStringList dir);
  void handleDir(QString dir);
  void abort();

protected:
  void run() override;

};

#endif // _LOADER_H