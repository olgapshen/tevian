#ifndef _VIEW_POINT_H_
#define _VIEW_POINT_H_

#include <QLabel>
#include <QImage>
#include <QMutex>

class ViewPoint : public QLabel {
  Q_OBJECT

private:
  const int Height;
  const int Width;

  QMutex mutexRender;
  QMutex mutexResize;
  QMutex mutexMove;
  QPoint lastDragPos;
  //QPoint pixmapOffset;
  // double curScale;
  // int x0, y0, x1, y1;

  // qreal h11;
  // qreal h22;
  // qreal h12;
  // qreal h13;
  // qreal h21;
  // qreal h23;
  // qreal h31;
  // qreal h32;

  int x0;
  int y0;
  int x1;
  int y1; 
  //double scale;
  
  QImage image;

  //void resize(double scaleFactor);
  //void scroll(int deltaX, int deltaY);
  void render();
  void scale(double scale);
  void move(QPoint point);

protected:
  //void paintEvent(QPaintEvent *event) override;
  //void resizeEvent(QResizeEvent *event) override;
  //void keyPressEvent(QKeyEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  //void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
#if QT_CONFIG(wheelevent)
  void wheelEvent(QWheelEvent *event) override;
#endif  

public:
  explicit ViewPoint(QWidget *parent, int height, int width);
  void setImage(const QImage &aImage);

};

#endif // _VIEW_POINT_H_