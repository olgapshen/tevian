#ifndef _MY_DIALOG_H_
#define _MY_DIALOG_H_

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class MyDialog : public QDialog {
    Q_OBJECT
public:
    MyDialog(QWidget *parent = 0);
};

#endif // _MY_DIALOG_H_