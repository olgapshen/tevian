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
#include <QProgressBar>
#include <math.h>

#include "mainwindow.h"

const int CANVAS_HEIGHT = 700;
const int CANVAS_WIDTH = 700;
const int STATUS_HEIGHT = 50;

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QDialog(parent)
{
  currentImageIndex = -1;
  currentlyProcessed = 0;
  
  Loader *loader = new Loader(this);
  Requester *requester = new Requester(this);
  ViewPoint *image = new ViewPoint(this, CANVAS_HEIGHT, CANVAS_WIDTH);

  QGridLayout *grid = new QGridLayout(this);
  QPushButton *closeButton = new QPushButton(this);
  QPushButton *loadDirButton = new QPushButton(this);
  QPushButton *loadFileButton = new QPushButton(this);
  QPushButton *prevButton = new QPushButton(this);
  QPushButton *nextButton = new QPushButton(this);
  QProgressBar *progressBar = new QProgressBar(this);
  QLineEdit *tokenBox = new QLineEdit(this);
  QLabel *lblSetToken = new QLabel(this);
  QLabel *error = new QLabel(this);

  loadDirButton->setText(tr("Upload dir"));
  loadFileButton->setText(tr("Upload file"));
  prevButton->setText(tr("Prev"));
  nextButton->setText(tr("Next"));
  loadDirButton->setEnabled(false);
  loadFileButton->setEnabled(false);
  closeButton->setText(tr("Close"));
  lblSetToken->setText("Set API Token:");
  tokenBox->setFixedWidth(CANVAS_WIDTH);
  progressBar->setVisible(false);
  progressBar->setFixedWidth(CANVAS_WIDTH);
  error->setFixedHeight(STATUS_HEIGHT);
  
  grid->setSpacing(10);
  
  QPalette palette = error->palette();
  palette.setColor(error->foregroundRole(), Qt::red);
  error->setPalette(palette);

  grid->addWidget(loadDirButton, 0, 0, Qt::AlignLeft);
  grid->addWidget(loadFileButton, 0, 1, Qt::AlignLeft);
  grid->addWidget(closeButton, 0, 1, 1, 4, Qt::AlignLeft);
  grid->addWidget(lblSetToken, 1, 0, 1, 6, Qt::AlignLeft);
  grid->addWidget(tokenBox, 2, 0, 1, 6, Qt::AlignLeft);
  grid->addWidget(progressBar, 3, 0, 1, 6, Qt::AlignLeft);
  grid->addWidget(image, 4, 0, 1, 6, Qt::AlignLeft);
  grid->addWidget(prevButton, 5, 0, 1, 3, Qt::AlignLeft);
  grid->addWidget(nextButton, 5, 3, 1, 3, Qt::AlignRight);
  grid->addWidget(error, 6, 0, 1, 6, Qt::AlignLeft);

  grid->setRowStretch(0, 10);
  grid->setRowStretch(1, 10);
  grid->setRowStretch(2, 10);
  grid->setRowStretch(3, 10);
  grid->setRowStretch(4, 0);
  grid->setRowStretch(5, 10);
  grid->setRowStretch(6, 10);

  loader->start();

  connect(requester, &Requester::ready, loadDirButton, [loadDirButton, loadFileButton]() { 
    loadFileButton->setEnabled(true);
    loadDirButton->setEnabled(true);
  });

  connect(this, &MainWindow::reset, progressBar, [progressBar]() {
    progressBar->setVisible(true);
    progressBar->reset();
  });

  connect(this, &MainWindow::stepOn, progressBar, [this, progressBar]() {
    mutex.lock();
    currentlyProcessed++;
    progressBar->setValue(currentlyProcessed);
    mutex.unlock();
  });

  connect(this, &MainWindow::abort, loader, &Loader::abort);
  connect(this, &MainWindow::handleDir, loader, &Loader::handleDir);
  connect(this, &MainWindow::handleFiles, loader, &Loader::handleFiles);
  connect(this, &MainWindow::setError, error, &QLabel::setText);
  connect(this, &MainWindow::setImage, image, &ViewPoint::setImage);
  connect(tokenBox, &QLineEdit::textChanged, requester, &Requester::setToken);
  connect(loadDirButton, &QPushButton::clicked, this, &MainWindow::loadFromDir);
  connect(loadFileButton, &QPushButton::clicked, this, &MainWindow::loadFromFiles);
  connect(prevButton, &QPushButton::clicked, this, &MainWindow::prev);
  connect(nextButton, &QPushButton::clicked, this, &MainWindow::next);
  connect(closeButton, &QPushButton::clicked, this, &MainWindow::close);
  connect(loader, &Loader::prepare, this, &MainWindow::init);
  connect(loader, &Loader::counted, progressBar, &QProgressBar::setMaximum);
  connect(loader, &Loader::loaded, this, &MainWindow::addPixmap);
  connect(requester, &Requester::onError, this, &MainWindow::restError);
  connect(requester, &Requester::onSuccess, this, &MainWindow::restSuccess);
  connect(loader, &Loader::detect, requester, &Requester::detect);
}

void MainWindow::display(int index) {
  setImage(data[index], images[index]);
}

void MainWindow::prev() {
  if (0 == currentImageIndex) return;
  currentImageIndex--;
  display(currentImageIndex);
}

void MainWindow::next() {
  if (currentImageIndex == images.size() - 1) return;
  currentImageIndex++;
  display(currentImageIndex);
}

void MainWindow::init() {
  setError("");
  reset();
  currentImageIndex = -1;
  currentlyProcessed = 0;
  data.clear();
  images.clear();
}

bool MainWindow::loadFromFiles()
{
  QStringList files = QFileDialog::getOpenFileNames(
    this,
    tr("Select files"),
    ""
  );

  handleFiles(files);
}

bool MainWindow::loadFromDir()
{  
  QString dirName = QFileDialog::getExistingDirectory(
    this,
    tr("Open images directory"),
    "",
    QFileDialog::ShowDirsOnly
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
    //found[imageId] = true;
  }

  mutex.lock();
  if (-1 == currentImageIndex) {
    currentImageIndex = imageId;
    display(imageId);
  }
  mutex.unlock();

  stepOn();
}

void MainWindow::restError(int imageId, const QJsonObject& response) {
  abort();
  //found[imageId] = false;
  QString message = response["message"].toString();
  setError(message);
  stepOn();
}

void MainWindow::addPixmap(int imageId, const QImage &aImage)
{
  images[imageId] = aImage;
}