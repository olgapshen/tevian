#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QWidget>
#include <QDebug>
#include <QFileInfoList>
#include <iostream>

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
  nameFilter << "*.png" << "*.jpeg";
  QFileInfoList list = dir.entryInfoList(nameFilter);

  for ( const auto& i : list  )
  {
      qDebug() << i.canonicalFilePath();
  }

  return true;
}

MainWindow::MainWindow(QWidget *parent) : QDialog(parent) {
  QVBoxLayout *layout = new QVBoxLayout(this);
  QLabel *label = new QLabel(this);
  QPushButton *closeButton = new QPushButton(this);
  QPushButton *loadButton = new QPushButton(this);;

  label->setText("<font color=red>Hello, World</font>");

  loadButton->setText(tr("Open dir"));
  closeButton->setText(tr("Close"));

  layout->addWidget(label);
  layout->addWidget(loadButton);
  layout->addWidget(closeButton);

  connect(loadButton, SIGNAL(clicked()), this, SLOT(loadFromFile()));//SLOT(loadFromFile()));
  connect(closeButton,  SIGNAL(clicked()), this, SLOT(close()));
}