#ifndef _MY_DIALOG_H_
#define _MY_DIALOG_H_

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class MainWindow : public QDialog {
    Q_OBJECT

public Q_SLOTS:
  bool loadFromFile();

public:
    MainWindow(QWidget *parent = 0);
};

#endif // _MY_DIALOG_H_