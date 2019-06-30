#ifndef _VIEW_POINT_H_
#define _VIEW_POINT_H_

#include <QLabel>
#include <QImage>
#include <QMutex>

#include <facedata.h>

class ViewPoint : public QWidget {
  Q_OBJECT

private:
  const int Height;
  const int Width;

  QMutex mutexRender;
  QMutex mutexResize;
  QMutex mutexMove;
  QPoint lastDragPos;

  int x0;
  int y0;
  int x1;
  int y1;
  double imgscale;
  
  QPixmap image;
  std::vector<FaceData> data;

  //void render();
  void scale(double scale);
  void move(QPoint point);

protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
#if QT_CONFIG(wheelevent)
  void wheelEvent(QWheelEvent *event) override;
#endif  

public:
  explicit ViewPoint(QWidget *parent, int height, int width);
  void setImage(std::vector<FaceData> &data, const QImage &aImage);

};

#endif // _VIEW_POINT_H_