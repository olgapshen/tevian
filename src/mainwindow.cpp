#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QWidget>
#include <QDebug>
#include <QImage>
#include <QFileInfoList>
#include <QThread>
#include <QGridLayout>
#include <QWheelEvent>
#include <QPainter>
#include <QKeyEvent>
#include <QLineEdit>
#include <math.h>

#include "mainwindow.h"

const int CANVAS_HEIGHT = 700;
const int CANVAS_WIDTH = 700;
const int STATUS_HEIGHT = 50;

using namespace std;

MainWindow::MainWindow(QWidget *parent) : 
  QDialog(parent),
  loader(this),
  requester(this),
  image(this, CANVAS_HEIGHT, CANVAS_WIDTH)
{
  current_image_index = -1;
  
  QGridLayout *grid = new QGridLayout(this);
  QPushButton *closeButton = new QPushButton(this);
  QPushButton *loadButton = new QPushButton(this);
  QLineEdit *tokenBox = new QLineEdit(this);
  QLabel *lblSetToken = new QLabel(this);
  QLabel *error = new QLabel(this);

  loadButton->setText(tr("Upload"));
  closeButton->setText(tr("Close"));
  lblSetToken->setText("Set API Token:");
  loadButton->setEnabled(false);
  error->setFixedHeight(STATUS_HEIGHT);
  tokenBox->setFixedWidth(CANVAS_WIDTH);
  grid->setSpacing(10);
  
  QPalette palette = error->palette();
  palette.setColor(error->foregroundRole(), Qt::red);
  error->setPalette(palette);

  grid->addWidget(loadButton, 0, 0, Qt::AlignLeft);
  grid->addWidget(closeButton, 0, 1, 1, 5, Qt::AlignLeft);
  grid->addWidget(lblSetToken, 1, 0, 1, 6, Qt::AlignLeft);
  grid->addWidget(tokenBox, 2, 0, 1, 6, Qt::AlignLeft);
  grid->addWidget(&image, 3, 0, 1, 6, Qt::AlignLeft);
  grid->addWidget(error, 4, 0, 1, 6, Qt::AlignLeft);

  grid->setRowStretch(0, 10);
  grid->setRowStretch(1, 10);
  grid->setRowStretch(2, 10);
  grid->setRowStretch(3, 0);
  grid->setRowStretch(4, 0);

  loader.start();

  connect(this, &MainWindow::ready, loadButton, &QLabel::setEnabled);
  connect(this, &MainWindow::setError, error, &QLabel::setText);
  connect(tokenBox, &QLineEdit::textChanged, this, &MainWindow::setToken);
  connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadFromFile);
  connect(closeButton, &QPushButton::clicked, this, &MainWindow::close);
  connect(&loader, &Loader::renderedImage, this, &MainWindow::addPixmap);
  connect(&requester, &Requester::onError, this, &MainWindow::restError);
  connect(&requester, &Requester::onSuccess, this, &MainWindow::restSuccess);
  connect(&loader, &Loader::detect, &requester, &Requester::detect);

  // TODO: remove
  QThread::sleep(1);
  setToken("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJqdGkiOiJmNjRhNTE3MC0xYzFhLTQwZTMtYTA4ZC0xN2U3YmEzN2M0YWUiLCJzdWIiOjMyLCJpYXQiOjE1NjE4MTUxNDUsIm5iZiI6MTU2MTgxNTE0NSwidHlwZSI6ImFjY2VzcyIsImZyZXNoIjpmYWxzZX0.0myIt5LdLQdd0SSAdH_DhuDkfZ6fv4ATK1tJKU0QiXc");
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

void MainWindow::setToken(const QString &token) {
  requester.setToken(token);
  ready(true);
}

bool MainWindow::loadFromFile()
{
  setError("");
  
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
    face.x = x;
    face.y = y;
    face.width = width;
    face.height = height;
    face.age = age;
    face.isMale = isMale;

    data[imageId].push_back(face);
    found[imageId] = true;
  }

  if (-1 == current_image_index) {
    current_image_index = 0;
    //QImage *image = &images[imageId];
    image.setImage(data[imageId], images[imageId]);
  }
}

void MainWindow::restError(int imageId, const QJsonObject& response) {
  loader.abort();
  found[imageId] = false;
  QString message = response["message"].toString();
  setError(message);
}

void MainWindow::addPixmap(int imageId, const QImage &aImage)
{
  images[imageId] = aImage;
}