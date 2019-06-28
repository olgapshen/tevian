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
  // x0 = 0;
  // y0 = 0;
  // x1 = Width;
  // y1 = Height;
  // curScale = 1;

  // h11 = 1;
  // h22 = 1;

  x0 = 0;
  y0 = 0;
  x1 = Width;
  y1 = Height;
  //scale = 1;

  // h12 = 0;
  // h13 = 0;
  // h21 = 0;
  // h23 = 0;
  // h31 = 0;
  // h32 = 0;

  setFixedWidth(Width);
  setFixedHeight(Height);
  setScaledContents(true);
}

void ViewPoint::setImage(const QImage &aImage) {
  image = aImage;
  scale(1);
}

// void ViewPoint::paintEvent(QPaintEvent * /* event */)
// {
// }

#if QT_CONFIG(wheelevent)
void ViewPoint::wheelEvent(QWheelEvent *event)
{
  mutexResize.lock();
    // event->
    // int numDegrees = event->delta() / 8;
    // double numSteps = numDegrees / 15.0f;
    // resize(pow(ZoomFactor, numSteps));


  const int degrees = event->delta()  / 8;
  int steps = degrees / 15;
  //qDebug() << steps;
  //double scaleFactor = 0.9; //How fast we zoom
  double currentScale = pow(ZoomFactor, steps);
  //qDebug() << currentScale;
  if (currentScale > 0.5 && currentScale < 1.5) scale(currentScale);

  // const qreal minFactor = 1.0;
  // const qreal maxFactor = 10.0;
  
  // if(steps > 0)
  // {
  //   h11 = (h11 >= maxFactor) ? h11 : (h11 + scaleFactor);
  //   h22 = (h22 >= maxFactor) ? h22 : (h22 + scaleFactor);
  // }
  // else
  // {
  //   h11 = (h11 <= minFactor) ? minFactor : (h11 - scaleFactor);
  //   h22 = (h22 <= minFactor) ? minFactor : (h22 - scaleFactor);
  // } 
  
  // qDebug() << h11;
  // qDebug() << h22;

  mutexResize.unlock();
}
#endif

void ViewPoint::mousePressEvent(QMouseEvent *event)
{
    mutexMove.lock();
    if (event->button() == Qt::LeftButton) {
      //pixmapOffset = lastDragPos;
      lastDragPos = event->pos();
    }
    mutexMove.unlock();
}

void ViewPoint::mouseReleaseEvent(QMouseEvent *event)
{
  mutexMove.lock();
  if (event->button() == Qt::LeftButton) {
    QPoint point = lastDragPos - event->pos();
    lastDragPos = QPoint();

    // qDebug() << pixmapOffset.x();
    // qDebug() << pixmapOffset.y();

    // x0 += pixmapOffset.x();
    // y0 += pixmapOffset.y();

    // //QImage current = images[]

    // int deltaX = (width() - image.width()) / 2 - pixmapOffset.x();
    // int deltaY = (height() - image.height()) / 2 - pixmapOffset.y();

    // if (x0 + deltaX < 0 || x1 + deltaX > Width) return;
    // if (y0 + deltaY < 0 || y1 + deltaY > Height) return;

    // x0 += deltaX;
    // x1 += deltaX;

    // y0 += deltaY;
    // y1 += deltaY;

    //render();
    //scroll(deltaX, deltaY);
    move(point);
  }
  mutexMove.unlock();
}

// void MainWindow::mouseMoveEvent(QMouseEvent *event)
// {
//     if (event->buttons() & Qt::LeftButton) {
//         pixmapOffset += event->pos() - lastDragPos;
//         lastDragPos = event->pos();
//         update();
//     }
// }

void ViewPoint::render() {
  mutexRender.lock();
  QRect rect(x0, y0, x1, y1);
  QPixmap pixmap = QPixmap::fromImage(image);
  
  int init_height = pixmap.height();
  int init_width = pixmap.width();
  if (init_height > init_width) init_height = init_width;
  else init_width = init_height;

  QPixmap init_crop = pixmap.copy(0, 0, init_width, init_height);
  QPixmap init_scale = init_crop.scaled(Height, Width, Qt::KeepAspectRatioByExpanding);
  QPixmap crop = init_scale.copy(rect);
  
  setPixmap(crop);
  mutexRender.unlock();
}

void ViewPoint::move(QPoint point)
{
  qDebug() << "point.x" << point.x();
  qDebug() << "point.y" << point.y();

  if (
    x0 + point.x() < 0 || 
    y0 + point.y() < 0 ||
    x1 + point.x() > Width || 
    y1 + point.y() > Height
  ) {
    return;
  } else {
    x0 += point.x();
    y0 += point.y();
    x1 += point.x();
    y1 += point.y();
  }

  render();
}

void ViewPoint::scale(double scale)
{
  int currWidth = x1 - x0;
  int currHeight = y1 - y0;

  if (currWidth * scale > Width || currHeight * scale > Height) {
    currWidth = Width;
    currHeight = Height;
  } else {
    currWidth *= scale;
    currHeight *= scale;
  }

  x1 = x0 + currWidth;
  y1 = y0 + currHeight;

  qDebug() << "currWidth" << currWidth;
  qDebug() << "currHeight" << currHeight;

  render();
}

// void ViewPoint::resize(double scaleFactor){
//   double scale = curScale * scaleFactor;
//   int preX = x1 * scale;
//   int preY = x1 * scale;

//   if (scale > 2 || scale < 0.5) return;
//   if (preX >= Height || preX >= Width) return; 
//   if (preY <= 0 || preY <= 0) return; 

//   x1 = preX;
//   y1 = preY;

//   if (x1 == 0) {
//     int r = 0;
//   }
  
//   curScale = scale;
//   // x1 = scaleX;
//   // y1 = scaleY;
//   render();
// }

// void ViewPoint::scroll(int deltaX, int deltaY)
// {  
//   x0 += deltaX;
//   x1 += deltaX;

//   y0 += deltaY;
//   y1 += deltaY;

//   render();
// }