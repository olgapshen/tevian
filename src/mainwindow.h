#ifndef _MY_DIALOG_H_
#define _MY_DIALOG_H_

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QPoint>
#include <vector>

#include "renderthread.h"

class MainWindow : public QDialog {
    Q_OBJECT

private:
  RenderThread thread;
  QLabel image;
  QPoint lastDragPos;
  QPoint pixmapOffset;
  double curScale;
  int x0, y0, x1, y1;

  int current_image_index;
  std::vector<QImage> images;

  void handleDir(QString dir);
  void render();

protected:
  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;
  //void keyPressEvent(QKeyEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
#if QT_CONFIG(wheelevent)
  void wheelEvent(QWheelEvent *event) override;
#endif

private slots:
    void addPixmap(const QImage &image);
    //void zoom(double zoomFactor);

public slots:
  bool loadFromFile();

public:
    MainWindow(QWidget *parent = 0);
};

#endif // _MY_DIALOG_H_