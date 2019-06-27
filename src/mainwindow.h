#ifndef _MY_DIALOG_H_
#define _MY_DIALOG_H_

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

#include "renderthread.h"

class MainWindow : public QDialog {
    Q_OBJECT

private:
  RenderThread thread;
  QLabel image;

private slots:
    void updatePixmap(const QImage &image, double scaleFactor);
    //void zoom(double zoomFactor);

public slots:
  bool loadFromFile();

public:
    MainWindow(QWidget *parent = 0);
};

#endif // _MY_DIALOG_H_