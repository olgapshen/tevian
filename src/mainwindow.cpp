#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QWidget>
#include <QDebug>
#include <QImage>
#include <QFileInfoList>
#include <iostream>
#include <QThread>
#include <QGridLayout>
#include <QWheelEvent>
#include <QPainter>
#include <QKeyEvent>
#include <math.h>

#include "mainwindow.h"

const double DefaultCenterX = -0.637011f;
const double DefaultCenterY = -0.0395159f;
const double DefaultScale = 0.00403897f;

const double ZoomInFactor = 0.8f;
const double ZoomOutFactor = 1 / ZoomInFactor;
const int ScrollStep = 20;

const int Width = 400;
const int Height = 400;

using namespace std;

MainWindow::MainWindow(QWidget *parent) : 
  QDialog(parent),
  image(this)
{
  x0, y0 = 0;
  x1 = Width;
  y1 = Height;
  curScale = 1;
  current_image_index = -1;

  image.setFixedWidth(Width);
  image.setFixedHeight(Height);

  QPushButton *closeButton = new QPushButton(this);
  QPushButton *loadButton = new QPushButton(this);

  loadButton->setText(tr("Open dir"));
  closeButton->setText(tr("Close"));

  QGridLayout *grid = new QGridLayout(this);

  grid->addWidget(loadButton, 0, 0);
  grid->addWidget(closeButton, 1, 0);
  grid->addWidget(&image, 0, 1);

  thread.start();

  connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadFromFile);
  connect(closeButton, &QPushButton::clicked, this, &MainWindow::close);
  connect(&thread, &RenderThread::renderedImage, this, &MainWindow::addPixmap);

  // TODO: remove
  handleDir("/home/olga/Desktop/images");
}

void MainWindow::handleDir(QString path) {
  QDir dir(path);
  QStringList nameFilter;
  nameFilter << "*.png" << "*.jpeg" << "*.jpg";
  QFileInfoList list = dir.entryInfoList(nameFilter);
  thread.handleList(list);
}

bool MainWindow::loadFromFile()
{
  QString dirName = QFileDialog::getExistingDirectory(
    this,
    tr("Open images directory"),
    "",
    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
  );

  if (dirName.isEmpty()) return false;
  else {
    handleDir(dirName);
    return true;
  }
}

void MainWindow::paintEvent(QPaintEvent * /* event */)
{
}

void MainWindow::resizeEvent(QResizeEvent * /* event */)
{
  //thread.render(centerX, centerY, curScale, size());
}

void MainWindow::zoom(double zoomFactor)
{
    curScale *= zoomFactor;
    // update();
    // thread.render(centerX, centerY, curScale, size());
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        lastDragPos = event->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        pixmapOffset += event->pos() - lastDragPos;
        lastDragPos = event->pos();
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    // if (event->button() == Qt::LeftButton) {
    //     pixmapOffset += event->pos() - lastDragPos;
    //     lastDragPos = QPoint();

    //     int deltaX = (width() - pixmap.width()) / 2 - pixmapOffset.x();
    //     int deltaY = (height() - pixmap.height()) / 2 - pixmapOffset.y();
    //     scroll(deltaX, deltaY);
    // }
}

void MainWindow::render() 
{
  QImage current_image = images[current_image_index];
  QRect rect(x0, y0, x1, y1);
  QImage scaled = current_image.scaledToHeight(Height);//, Qt::KeepAspectRatio);
  QImage croped = current_image.copy(rect);
  image.setPixmap(QPixmap::fromImage(current_image));
}

void MainWindow::addPixmap(const QImage &aImage)
{
  QImage copy = aImage;
  images.push_back(copy);
  if (-1 == current_image_index) {
    current_image_index = 0;
    render();
  }
}

#if QT_CONFIG(wheelevent)
void MainWindow::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    double numSteps = numDegrees / 15.0f;
    zoom(pow(ZoomInFactor, numSteps));
}
#endif