#include <QtCore>
#include <QtGui>
#include <QApplication>
#include <QLabel>
#include <QDialog>
#include <QFileDialog>

#include "mainwindow.h"

#if !QT_CONFIG(wheelevent)
#error wheelevent not found!
#endif 

using namespace std;

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  MainWindow *dialog = new MainWindow();
  dialog->show();
  return app.exec();
}