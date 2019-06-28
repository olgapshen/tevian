#ifndef _MY_DIALOG_H_
#define _MY_DIALOG_H_

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QPoint>
#include <vector>

#include "renderthread.h"
#include "viewpoint.h"

class MainWindow : public QDialog {
    Q_OBJECT

private:
  RenderThread thread;

  int current_image_index;
  std::vector<QImage> images;

  ViewPoint image;

  void handleDir(QString dir);

private slots:
    void addPixmap(const QImage &image);
    //void zoom(double zoomFactor);

public slots:
  bool loadFromFile();

public:
    MainWindow(QWidget *parent = 0);
};

#endif // _MY_DIALOG_H_