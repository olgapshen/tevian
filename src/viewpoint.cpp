#include "viewpoint.h"

#include <QPainter>
#include <QTextItem>
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
}

void ViewPoint::setImage(std::vector<FaceData> &imageData, const QImage &aImage) {
  QPixmap pixmap = QPixmap::fromImage(aImage);
  data.insert(data.begin(), imageData.begin(), imageData.end());

  int init_height = pixmap.height();
  int init_width = pixmap.width();
  if (init_height > init_width) init_width = init_height;
  else init_height = init_width;

  QPixmap canvas(init_width, init_height);
  canvas.fill(Qt::transparent);
  QRect rectSource(0, 0, pixmap.width(), pixmap.height());
  QPainter painter(&canvas);
  painter.drawPixmap(rectSource, pixmap);

  image = canvas;

  x0 = 0;
  y0 = 0;
  x1 = Width;
  y1 = Height;
  imgscale = 1;

  scale(1);
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
    move(point);
  }
  mutexMove.unlock();
}

void ViewPoint::paintEvent(QPaintEvent * /* event */)
{
  QPainter painter(this);
  QRect rect(x0, y0, x1, y1);

  if (rect.width() == 0) return;

  painter.drawPixmap(rect, image);

  double scale = (double)image.width() / (double)rect.width();

  for (const auto& i : data)
  {
    FaceData transformed;
    i.transform(scale, x0, y0, transformed);

    QString age("age: ");
    age.append(QString::number(i.age));
    QString sex("sex: ");
    sex.append(i.isMale ? "male" : "female");

    QPoint agePoint(transformed.x, transformed.y + transformed.height + 20);
    QPoint sexPoint(transformed.x, agePoint.y() + 20);

    painter.drawRect(transformed.x, transformed.y, transformed.height, transformed.width);
    painter.drawText(agePoint, age);
    painter.drawText(sexPoint, sex);
  }
}

void ViewPoint::scale(double scale)
{
  if (imgscale / scale > 3) return;
  
  int currWidth = x1 - x0;
  int currHeight = y1 - y0;
  int imageHeight = Height;
  int imageWidth = Width;

  if (currWidth / scale < imageWidth || currHeight / scale < imageHeight) {
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

  // qDebug() << "imgscale" << imgscale;
  // qDebug() << "x0" << x0;
  // qDebug() << "y0" << y0;
  // qDebug() << "x1" << x1;
  // qDebug() << "y1" << y1;
  // qDebug() << "width" << x1 - x0;
  // qDebug() << "height" << y1 - y0;

  //render();
  update();
}

void ViewPoint::move(QPoint point)
{
  int currentWidth = x1 - x0;
  int currentHeight = y1 - y0;
  int marginHorizontal = (currentWidth - Width) / 2;
  int marginVertical = (currentHeight - Height) / 2;
  int right = Height + marginHorizontal;
  int bottom = Width + marginVertical;

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

  // qDebug() << "x0" << x0;
  // qDebug() << "y0" << y0;
  // qDebug() << "x1" << x1;
  // qDebug() << "y1" << y1;
  // qDebug() << "width" << x1 - x0;
  // qDebug() << "height" << y1 - y0;

  //render();
  update();
}