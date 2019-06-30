#ifndef _MY_DIALOG_H_
#define _MY_DIALOG_H_

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QPoint>
#include <QMutex>

#include <map>
#include <vector>

#include "loader.h"
#include "viewpoint.h"
#include "requester.h"
#include "facedata.h"

class MainWindow : public QDialog {
    Q_OBJECT

private:
  QMutex mutex;
  int currentImageIndex;
  int currentlyProcessed;

  std::map<int, QImage> images;
  std::map<int, std::vector<FaceData>> data;
  
  void init();
  void display(int index);

signals:
  //void ready();
  void reset();
  void abort();
  void stepOn();
  void setError(const QString &status);
  void handleFiles(QStringList dir);
  void handleDir(QString dir);
  void setImage(std::vector<FaceData> &imageData, const QImage &aImage);

private slots:
  //void setToken(const QString &token);
  void prev();
  void next();
  bool loadFromFiles();
  bool loadFromDir();
  void addPixmap(int imageId, const QImage &image);
  void restSuccess(int imageId, const QJsonObject& response);
  void restError(int imageId, const QJsonObject& response);

public:
    MainWindow(QWidget *parent = 0);
};

#endif // _MY_DIALOG_H_