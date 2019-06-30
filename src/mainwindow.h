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
  ViewPoint image;

  int current_image_index;
  //bool error_state;

  std::map<int, QImage> images;
  std::map<int, std::vector<FaceData>> data;
  std::map<int, bool> found;

  void handleDir(QString dir);

signals:
  void setError(const QString &status);
  void ready(bool ready);
  
private slots:
  void setToken(const QString &token);
  bool loadFromFile();
  void addPixmap(int imageId, const QImage &image);
  void restSuccess(int imageId, const QJsonObject& response);
  void restError(int imageId, const QJsonObject& response);

public:
    MainWindow(QWidget *parent = 0);
};

#endif // _MY_DIALOG_H_