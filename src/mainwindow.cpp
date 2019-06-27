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

using namespace std;

#include "mainwindow.h"

bool MainWindow::loadFromFile()
{
  QString dirName = QFileDialog::getExistingDirectory(
    this,
    tr("Open images directory"),
    "/home/olga/Desktop",
    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
  );

  if (dirName.isEmpty()) return false;

  QDir dir(dirName);
  QStringList nameFilter;
  nameFilter << "*.png" << "*.jpeg" << "*.jpg";
  QFileInfoList list = dir.entryInfoList(nameFilter);
  thread.handleList(list);

  return true;
}

MainWindow::MainWindow(QWidget *parent) : 
  QDialog(parent),
  image(this) 
{
  QLabel *label = new QLabel(this);
  QPushButton *closeButton = new QPushButton(this);
  QPushButton *loadButton = new QPushButton(this);
  label->setText("<font color=red>Hello, World</font>");
  loadButton->setText(tr("Open dir"));
  closeButton->setText(tr("Close"));

  QGridLayout *grid = new QGridLayout(this);

  grid->addWidget(label, 0, 0);
  grid->addWidget(loadButton, 1, 0);
  grid->addWidget(closeButton, 2, 0);
  grid->addWidget(&image, 0, 1);

  //setLayout(grid);
  //window->setLayout(layout);

  thread.startThread();

  connect(loadButton, SIGNAL(clicked()), this, SLOT(loadFromFile()));//SLOT(loadFromFile()));
  connect(closeButton,  SIGNAL(clicked()), this, SLOT(close()));
  connect(&thread, &RenderThread::renderedImage, this, &MainWindow::updatePixmap);
}

void MainWindow::updatePixmap(const QImage &aImage, double scaleFactor)
{
  image.setPixmap(QPixmap::fromImage(aImage));
  // if (!lastDragPos.isNull())
  //     return;

  // pixmap = QPixmap::fromImage(image);
  // pixmapOffset = QPoint();
  // lastDragPos = QPoint();
  // pixmapScale = scaleFactor;
  // update();
}