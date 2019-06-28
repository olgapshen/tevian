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

const int Height = 400;
const int Width = 400;

using namespace std;

MainWindow::MainWindow(QWidget *parent) : 
  QDialog(parent),
  image(this, Height, Width)
{
  current_image_index = -1;

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

void MainWindow::addPixmap(const QImage &aImage)
{
  //QImage copy = aImage;
  images.push_back(aImage);
  if (-1 == current_image_index) {
    current_image_index = 0;
    image.setImage(aImage);// render();
    //image.setPixmap(QPixmap::fromImage(images[current_image_index]));
  }
}