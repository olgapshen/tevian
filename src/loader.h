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
  void loaded(int imageId, const QString path, const QImage &image);
  void detect(int imageId, QImage image);
  void counted(int amount);
  //void finished();
  void setError(QString error);

public slots:
  void handleFiles(QStringList dir);
  void handleDir(QString dir);
  void process();
  void abort();

protected:
  void run() override;

};

#endif // _LOADER_H