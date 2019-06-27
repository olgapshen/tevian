#include <QtCore>
#include <QtGui>
#include <QApplication>
#include <QLabel>
#include <QDialog>

#include "mydialog.h"

using namespace std;

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MyDialog *dialog = new MyDialog;
    dialog->show();
    return app.exec();
}