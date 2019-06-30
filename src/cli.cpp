#include <QtCore>
#include <QtGui>
#include <QApplication>
#include <QLabel>
#include <QDialog>
#include <QFileDialog>

#include "maintask.h"

using namespace std;

int main(int argc, char* argv[]) {
  QCoreApplication app(argc, argv);


  QCoreApplication::setApplicationName("tevian");
  QCoreApplication::setApplicationVersion("1.0");

  QCommandLineParser parser;
  parser.setApplicationDescription("Face detector");
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption tokenOption(
    QStringList() << "t" << "token",
    "Token", 
    "string"
  );

  QCommandLineOption thresholdOption(
    QStringList() << "r" << "threshold",
    "Threashold", 
    "double"
  );

  QCommandLineOption fdMinOption(
    QStringList() << "n" << "fdmin",
    "FD Min", 
    "int"
  );

  QCommandLineOption fdMaxOption(
    QStringList() << "x" << "fdmax",
    "FD max", 
    "int"
  );

  QCommandLineOption sourceDirectoryOption(
    QStringList() << "s" << "source",
    "Folder with files", 
    "path"
  );

  QCommandLineOption sourceFilesOption(
    QStringList() << "f" << "file",
    "Single file/files", 
    "path"
  );

  parser.addOption(tokenOption);
  parser.addOption(sourceDirectoryOption);
  parser.addOption(sourceFilesOption);
  parser.addOption(thresholdOption);
  parser.addOption(fdMinOption);
  parser.addOption(fdMaxOption);

  parser.process(app);

  QString token = parser.value(tokenOption);
  double threshold = parser.value(thresholdOption).toDouble();
  int fdMin = parser.value(thresholdOption).toInt();
  int fdMax = parser.value(thresholdOption).toInt();
  QString dir = parser.value(sourceDirectoryOption);
  QStringList files = parser.values(sourceFilesOption);

  MainTask *task = new MainTask(&app);
  
  task->setFdMax(fdMax);
  task->setFdMin(fdMin);
  task->setThreshold(threshold);
  task->setToken(token);
  
  if (!dir.isEmpty())
    task->handleDir(dir);
  else
    task->handleFiles(files);
  
  QObject::connect(task, &MainTask::finished, &app, &QCoreApplication::quit);
  QTimer::singleShot(0, task, &MainTask::start);

  return app.exec();
}