#include "viewpoint.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <math.h>

const double ZoomFactor = 0.9f;

ViewPoint::ViewPoint(QWidget *parent, int height, int width) : 
  QLabel(parent) ,
  Height(height),
  Width(width)
{
  x0 = 0;
  y0 = 0;
  x1 = Width;
  y1 = Height;
  imgscale = 1;

  setFixedWidth(Width);
  setFixedHeight(Height);
  //setScaledContents(true);
}

void ViewPoint::setImage(const QImage &aImage) {
  image = aImage;
  scale(1);
  //render();
}

#if QT_CONFIG(wheelevent)
void ViewPoint::wheelEvent(QWheelEvent *event)
{
  mutexResize.lock();

  const int degrees = event->delta()  / 8;
  int steps = degrees / 15;
  scale(pow(ZoomFactor, steps));

  mutexResize.unlock();
}
#endif

void ViewPoint::mousePressEvent(QMouseEvent *event)
{
    mutexMove.lock();
    if (event->button() == Qt::LeftButton)
      lastDragPos = event->pos();
    mutexMove.unlock();
}

void ViewPoint::mouseReleaseEvent(QMouseEvent *event)
{
  mutexMove.lock();
  if (event->button() == Qt::LeftButton) {
    QPoint point = lastDragPos - event->pos();
    lastDragPos = QPoint();
    move(point);
  }
  mutexMove.unlock();
}

void ViewPoint::render() {
  mutexRender.lock();

  int tx0 = x0 * image.width() / Width;
  int ty0 = y0 * image.height() / Height;
  int tx1 = x1 * image.width() / Width;
  int ty1 = y1 * image.height() / Height;
  QRect rect(tx0, ty0, tx1, ty1);

  QPixmap pixmap = QPixmap::fromImage(image);
  
  // int init_height = pixmap.height();
  // int init_width = pixmap.width();
  // if (init_height > init_width) init_height = init_width;
  // else init_width = init_height;

  //QPixmap init_crop = pixmap.copy(0, 0, init_width, init_height);
  //QPixmap init_scale = pixmap.scaled(Height, Width, Qt::KeepAspectRatio);
  QPixmap crop = pixmap.copy(rect);
  //qDebug() << crop.width();
  QPixmap scaled = crop.scaled(Height, Width, Qt::KeepAspectRatio);
  
  setPixmap(scaled);
  mutexRender.unlock();
}

void ViewPoint::scale(double scale)
{
  if (imgscale * scale < 0.3) return;
  
  int currWidth = x1 - x0;
  int currHeight = y1 - y0;
  //int imageHeight = image.height();
  //int imageWidth = image.width();

  if (currWidth * scale > Width || currHeight * scale > Height) {
    //currWidth = Width;
    //currHeight = Height;
    x0 = y0 = 0;
    x1 = Width;
    y1 = Height;
    imgscale = 1;
  } else {
    currWidth *= scale;
    currHeight *= scale;
    x1 = x0 + currWidth;
    y1 = y0 + currHeight;
    imgscale *= scale;
  }

  render();
}

void ViewPoint::move(QPoint point)
{
  qDebug() << "point.x" << point.x();
  qDebug() << "point.y" << point.y();

  int moveX = point.x() / (image.width() / Width);

  if (x0 + point.x() < 0) {
    x1 -= x0;
    x0 = 0;
  } else if (x1 + point.x() > Width) {
    x0 += Width - x1;
    x1 = Width;
  } else {
    x0 += point.x();
    x1 += point.x();
  }

  if (y0 + point.y() < 0) {
    y1 -= y0;
    y0 = 0;
  } else if (y1 + point.y() > Height) {
    y0 += Height - y1;
    y1 = Height;
  } else {
    y0 += point.y();
    y1 += point.y();
  }

  render();
}