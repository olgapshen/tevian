#ifndef _MY_DIALOG_H_
#define _MY_DIALOG_H_

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QPoint>
#include <map>
#include <vector>

#include "loader.h"
#include "viewpoint.h"
#include "requester.h"
#include "facedata.h"

class MainWindow : public QDialog {
    Q_OBJECT

private:
  Loader loader;
  Requester requester;

  int current_image_index;
  
  std::map<int, QImage> images;
  std::map<int, std::vector<FaceData>> data;
  std::map<int, bool> found;

  ViewPoint image;
  void handleDir(QString dir);

private slots:
  void addPixmap(int imageId, const QImage &image);
  void restSuccess(int imageId, const QJsonObject& response);
  void restError(int imageId, const QJsonObject& response);

public slots:
  bool loadFromFile();

public:
    MainWindow(QWidget *parent = 0);
};

#endif // _MY_DIALOG_H_