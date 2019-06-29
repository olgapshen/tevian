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

const QString Token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJqdGkiOiJmNjRhNTE3MC0xYzFhLTQwZTMtYTA4ZC0xN2U3YmEzN2M0YWUiLCJzdWIiOjMyLCJpYXQiOjE1NjE4MTUxNDUsIm5iZiI6MTU2MTgxNTE0NSwidHlwZSI6ImFjY2VzcyIsImZyZXNoIjpmYWxzZX0.0myIt5LdLQdd0SSAdH_DhuDkfZ6fv4ATK1tJKU0QiXc";

const int Height = 400;
const int Width = 400;

using namespace std;

MainWindow::MainWindow(QWidget *parent) : 
  QDialog(parent),
  loader(this),
  requester(this),
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

  loader.start();

  connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadFromFile);
  connect(closeButton, &QPushButton::clicked, this, &MainWindow::close);
  connect(&loader, &Loader::renderedImage, this, &MainWindow::addPixmap);
  connect(&requester, &Requester::onError, this, &MainWindow::restError);
  connect(&requester, &Requester::onSuccess, this, &MainWindow::restSuccess);
  connect(&loader, &Loader::detect, &requester, &Requester::detect);

  requester.setToken(Token);

  // TODO: remove
  QThread::sleep(1);
  handleDir("/home/olga/Desktop/images");
}

void MainWindow::handleDir(QString path) {
  current_image_index = -1;
  found.clear();
  images.clear();

  QDir dir(path);
  QStringList nameFilter;
  nameFilter << "*.png" << "*.jpeg" << "*.jpg";
  QFileInfoList list = dir.entryInfoList(nameFilter);
  loader.load(list);
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

void MainWindow::restSuccess(int imageId, const QJsonObject& response) {
  //int age = 
  QJsonArray arr = response["data"].toArray();
  
  for (const auto& i : arr)
  {
    QJsonObject bbox = i.toObject()["bbox"].toObject();
    QJsonObject demographics = i.toObject()["demographics"].toObject();

    int x = bbox["x"].toInt();
    int y = bbox["y"].toInt();
    int height = bbox["height"].toInt();
    int width = bbox["width"].toInt();
    double age = demographics["age"].toObject()["mean"].toDouble();
    bool isMale = demographics["age"].toObject()["gender"].toBool();

    FaceData face;
    face.x0 = x;
    face.y0 = y;
    face.x1 = x + width;
    face.y1 = y + width;
    face.age = age;
    face.isMale = isMale;

    data[imageId].push_back(face);
    found[imageId] = true;
  }
}

void MainWindow::restError(int imageId, const QJsonObject& response) {
  found[imageId] = false;
}

void MainWindow::addPixmap(int imageId, const QImage &aImage)
{
  images[imageId] = aImage;

  if (-1 == current_image_index) {
    current_image_index = 0;
    image.setImage(aImage);
  }
}