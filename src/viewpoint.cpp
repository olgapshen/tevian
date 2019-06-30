#include "viewpoint.h"

#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <math.h>

const double ZoomFactor = 0.9f;

ViewPoint::ViewPoint(QWidget *parent, int height, int width) : 
  QWidget(parent) ,
  Height(height),
  Width(width)
{
  setFixedWidth(Width);
  setFixedHeight(Height);
  //setScaledContents(true);

}

void ViewPoint::setImage(const QImage &aImage) {
  QPixmap pixmap = QPixmap::fromImage(aImage);

  int init_height = pixmap.height();
  int init_width = pixmap.width();
  if (init_height > init_width) init_width = init_height;
  else init_height = init_width;

  QPixmap canvas(init_width, init_height);
  canvas.fill(Qt::transparent);
  //QRect rectDest(0, 0, init_width, init_height);
  QRect rectSource(0, 0, pixmap.width(), pixmap.height());
  QPainter painter(&canvas);
  //painter.drawPixmap(rectDest, canvas);
  painter.drawPixmap(rectSource, pixmap);

  image = canvas;

  x0 = 0;
  y0 = 0;
  x1 = Width; //image.width();
  y1 = Height;// image.height();
  imgscale = 1;

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
    QPoint point = event->pos() - lastDragPos;
    lastDragPos = QPoint();
    //image.scroll(-100, 100, image.rect());
    move(point);
  }
  mutexMove.unlock();
}

void ViewPoint::paintEvent(QPaintEvent * /* event */)
{
  QPainter painter(this);
  QRect rect(x0, y0, x1, y1);
  painter.drawPixmap(rect, image);
}


void ViewPoint::render() {
  mutexRender.lock();

  // int tx0 = x0 * image.width() / Width;
  // int ty0 = y0 * image.height() / Height;
  // int tx1 = x1 * image.width() / Width;
  // int ty1 = y1 * image.height() / Height;
  // QRect rect(x0, y0, x1, y1);

  // //QPixmap pixmap = QPixmap::fromImage(image);

  // //QPixmap init_crop = pixmap.copy(0, 0, init_width, init_height);
  // //QPixmap init_scale = pixmap.scaled(Height, Width, Qt::KeepAspectRatio);
  // QPixmap crop = image.copy(rect);
  // //qDebug() << crop.width();
  // QPixmap scaled = crop.scaled(Height, Width);
  
  //setPixmap(scaled);
  update();
  mutexRender.unlock();
}

void ViewPoint::scale(double scale)
{
  if (imgscale / scale > 3) return;
  
  int currWidth = x1 - x0;
  int currHeight = y1 - y0;
  int imageHeight = Height; //image.height();
  int imageWidth = Width; //image.width();

  if (currWidth / scale < imageWidth || currHeight / scale < imageHeight) {
    //currWidth = Width;
    //currHeight = Height;
    x0 = y0 = 0;
    x1 = imageWidth;
    y1 = imageHeight;
    imgscale = 1;
  } else {
    currWidth /= scale;
    currHeight /= scale;
    imgscale /= scale;
    x1 = x0 + currWidth;
    y1 = y0 + currHeight;
  }

  qDebug() << "imgscale" << imgscale;
  qDebug() << "x0" << x0;
  qDebug() << "y0" << y0;
  qDebug() << "x1" << x1;
  qDebug() << "y1" << y1;
  qDebug() << "width" << x1 - x0;
  qDebug() << "height" << y1 - y0;

  render();
}

void ViewPoint::move(QPoint point)
{
  //int moveX = point.x() / (image.width() / Width);

  //int w = image.width();
  int currentWidth = x1 - x0;
  int currentHeight = y1 - y0;
  int marginHorizontal = (currentWidth - Width) / 2;
  int marginVertical = (currentHeight - Height) / 2;
  int right = Height + marginHorizontal;
  int bottom = Width + marginVertical;
  
  // int imageWidth = Width;// x1 - x0;// Width;// image.width();
  // int imageHeight = Height;// y1 - y0;// Height;// image.height();

  qDebug() << "point.x" << point.x();
  qDebug() << "point.y" << point.y();

  if (x0 + point.x() > 0) {
    x1 += -x0;
    x0 = 0;
  } else if (x1 + point.x() < right) {
    x0 -= x1 - right;
    x1 = right;
  } else {
    x0 += point.x();
    x1 += point.x();
  }

  if (y0 + point.y() > 0) {
    y1 += -y0;
    y0 = 0;
  } else if (y1 + point.y() < bottom) {
    y0 -= y1 - bottom;
    y1 = bottom;
  } else {
    y0 += point.y();
    y1 += point.y();
  }

  qDebug() << "x0" << x0;
  qDebug() << "y0" << y0;
  qDebug() << "x1" << x1;
  qDebug() << "y1" << y1;
  qDebug() << "width" << x1 - x0;
  qDebug() << "height" << y1 - y0;

  render();
}