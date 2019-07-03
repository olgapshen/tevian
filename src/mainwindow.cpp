#include <math.h>

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
#include <QIntValidator>
#include <QRegExpValidator>

#include "mainwindow.h"

const int CANVAS_HEIGHT = 700;
const int CANVAS_WIDTH = 700;
const int STATUS_HEIGHT = 50;

const int DEFAULT_FD_MIN = 50;
const int DEFAULT_FD_MAX = 400;
const int MAX_VALID_FD = 1000;
const double DEFAULT_THRESHOLD = 0.2;

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
  QPushButton *startButton = new QPushButton(this);
  QPushButton *prevButton = new QPushButton(this);
  QPushButton *nextButton = new QPushButton(this);
  QProgressBar *progressBar = new QProgressBar(this);
  QLabel *lblSetFdMin = new QLabel(this);
  QLabel *lblSetFdMax = new QLabel(this);
  QLabel *lblSetThreshold = new QLabel(this);
  QLabel *lblSetToken = new QLabel(this);
  QLineEdit *tokenBox = new QLineEdit(this);
  QLineEdit *fdMinBox = new QLineEdit(this);
  QLineEdit *fdMaxBox = new QLineEdit(this);
  QLineEdit *thresholdBox = new QLineEdit(this);
  QLabel *error = new QLabel(this);

  QRegExp rx("[a-zA-Z0-9_.]{128,}");
  fdMinBox->setValidator(new QIntValidator(0, MAX_VALID_FD, this));
  fdMaxBox->setValidator(new QIntValidator(0, MAX_VALID_FD, this));
  tokenBox->setValidator(new QRegExpValidator(rx, this));

  loadDirButton->setText(tr("Upload dir"));
  loadFileButton->setText(tr("Upload file"));
  startButton->setText(tr("Start"));
  prevButton->setText(tr("Prev"));
  nextButton->setText(tr("Next"));
  loadDirButton->setEnabled(false);
  loadFileButton->setEnabled(false);
  startButton->setEnabled(false);
  closeButton->setText(tr("Close"));
  lblSetToken->setText("Set API Token:");
  lblSetFdMin->setText("Set FD min");
  lblSetFdMax->setText("Set FD max");
  lblSetThreshold->setText("Set Threshold");
  tokenBox->setFixedWidth(CANVAS_WIDTH);
  progressBar->setVisible(false);
  progressBar->setFixedWidth(CANVAS_WIDTH);
  error->setFixedHeight(STATUS_HEIGHT);
  
  grid->setSpacing(10);
  
  QPalette palette = error->palette();
  palette.setColor(error->foregroundRole(), Qt::red);
  error->setPalette(palette);

  grid->addWidget(loadDirButton,    0, 0, 1, 1, Qt::AlignLeft);
  grid->addWidget(loadFileButton,   0, 1, 1, 1, Qt::AlignLeft);
  grid->addWidget(startButton,      0, 2, 1, 1, Qt::AlignLeft);
  grid->addWidget(closeButton,      0, 3, 1, 3, Qt::AlignLeft);
  grid->addWidget(lblSetToken,      1, 0, 1, 6, Qt::AlignLeft);
  grid->addWidget(tokenBox,         2, 0, 1, 6, Qt::AlignLeft);
  
  grid->addWidget(lblSetFdMin,      3, 0, 1, 1, Qt::AlignLeft);
  grid->addWidget(fdMinBox,         3, 1, 1, 1, Qt::AlignLeft);
  grid->addWidget(lblSetFdMax,      3, 2, 1, 1, Qt::AlignLeft);
  grid->addWidget(fdMaxBox,         3, 3, 1, 1, Qt::AlignLeft);
  grid->addWidget(lblSetThreshold,  3, 4, 1, 1, Qt::AlignLeft);
  grid->addWidget(thresholdBox,     3, 5, 1, 1, Qt::AlignLeft);

  grid->addWidget(progressBar,      4, 0, 1, 6, Qt::AlignLeft);
  grid->addWidget(image,            5, 0, 1, 6, Qt::AlignLeft);
  grid->addWidget(prevButton,       6, 0, 1, 3, Qt::AlignLeft);
  grid->addWidget(nextButton,       6, 3, 1, 3, Qt::AlignRight);
  grid->addWidget(error,            7, 0, 1, 6, Qt::AlignLeft);

  grid->setRowStretch(0, 10);
  grid->setRowStretch(1, 10);
  grid->setRowStretch(2, 10);
  grid->setRowStretch(3, 10);
  grid->setRowStretch(4, 0);
  grid->setRowStretch(5, 10);
  grid->setRowStretch(6, 10);

  loader->start();

  connect(
    requester,
    &Requester::ready,
    this, 
    [loadDirButton, loadFileButton, startButton]() { 
      loadFileButton->setEnabled(true);
      loadDirButton->setEnabled(true);
      startButton->setEnabled(true);
    }
  );

  connect(this, &MainWindow::stepOn, [this, progressBar]() {
    mutex.lock();
    currentlyProcessed++;
    progressBar->setValue(currentlyProcessed);
    mutex.unlock();
  });
  
  connect(
    loader,
    &Loader::loaded,
    [this, requester](int imageId, const QString path, const QImage &aImage) {
      //progressBar->setVisible(false);
      images[imageId] = aImage;
    }
  );

  connect(
    tokenBox,
    &QLineEdit::textChanged,
    [tokenBox, requester, this](const QString &value) {
      int pos;
      QString temp = value;
      QValidator::State state = tokenBox->validator()->validate(temp, pos);
      if (QValidator::State::Acceptable == state)
        requester->setToken(value);
      else
        setError("Invalid token!");
    }
  );

  connect(
    fdMinBox,
    &QLineEdit::textChanged,
    [fdMinBox, requester, this](const QString &value) {
      int pos;
      QString temp = value;
      QValidator::State state = fdMinBox->validator()->validate(temp, pos);
      
      if (QValidator::State::Acceptable != state) {
        setError("Invalid FD Min!");
        return;
      }
      
      double fdMin = value.toInt();
      requester->setFdMin(fdMin);
    }
  );

  connect(
    fdMaxBox,
    &QLineEdit::textChanged,
    [fdMaxBox, requester, this](const QString &value) {
      int pos;
      QString temp = value;
      QValidator::State state = fdMaxBox->validator()->validate(temp, pos);
      
      if (QValidator::State::Acceptable != state) {
        setError("Invalid FD Min!");
        return;
      }
      
      double fdMax = value.toInt();
      requester->setFdMax(fdMax);
    }
  );

  connect(
    thresholdBox,
    &QLineEdit::textChanged,
    [thresholdBox, requester, this](const QString &value) {
      double threshold = value.toDouble();
      
      if (threshold >= 0 && threshold <= 1)
        requester->setThreshold(threshold);
      else
        setError("Invalid threshold!");
    }
  );

  connect(startButton, &QPushButton::clicked, [this, progressBar]() {
    setError("");
    progressBar->setVisible(true);
    progressBar->setValue(0);
    currentImageIndex = -1;
    currentlyProcessed = 0;
    data.clear();
    images.clear();
  });
  
  connect(this, &MainWindow::abort, loader, &Loader::abort);
  connect(this, &MainWindow::handleDir, loader, &Loader::handleDir);
  connect(this, &MainWindow::handleFiles, loader, &Loader::handleFiles);
  connect(this, &MainWindow::setError, error, &QLabel::setText);
  connect(this, &MainWindow::setImage, image, &ViewPoint::setImage);
  connect(loadDirButton, &QPushButton::clicked, this, &MainWindow::loadFromDir);
  connect(loadFileButton, &QPushButton::clicked, this, &MainWindow::loadFromFiles);
  connect(startButton, &QPushButton::clicked, loader, &Loader::process);
  connect(prevButton, &QPushButton::clicked, this, &MainWindow::prev);
  connect(nextButton, &QPushButton::clicked, this, &MainWindow::next);
  connect(closeButton, &QPushButton::clicked, this, &MainWindow::close);
  connect(loader, &Loader::counted, progressBar, &QProgressBar::setMaximum);
  connect(loader, &Loader::counted, progressBar, &QProgressBar::setValue);
  connect(loader, &Loader::detect, requester, &Requester::detect);
  connect(requester, &Requester::onError, this, &MainWindow::restError);
  connect(requester, &Requester::onSuccess, this, &MainWindow::restSuccess); 

  fdMinBox->setText(QString::number(DEFAULT_FD_MIN));
  fdMaxBox->setText(QString::number(DEFAULT_FD_MAX));
  thresholdBox->setText(QString::number(DEFAULT_THRESHOLD));
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

// void MainWindow::addPixmap()
// {
//   images[imageId] = aImage;
// }