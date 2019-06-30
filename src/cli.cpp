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
  // parser.addPositionalArgument("source", "Folder with files");
  // parser.addPositionalArgument("file", "Separate file");


  //const QStringList args = parser.positionalArguments();
  // source is args.at(0), destination is args.at(1)

  QCommandLineOption tokenOption(
    QStringList() << "t" << "token",
    "Token", 
    "string"
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

  parser.process(app);

  QString token = parser.value(tokenOption);
  QString dir = parser.value(sourceDirectoryOption);
  QStringList files = parser.values(sourceFilesOption);

  qDebug() << dir;
  qDebug() << files;

  MainTask *task = new MainTask(&app);  
  QObject::connect(task, SIGNAL(finished()), &app, SLOT(quit()));
}