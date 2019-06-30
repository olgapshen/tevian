#ifndef _MAIN_TASK_H
#define _MAIN_TASK_H

#include <QtCore>

class MainTask : public QObject {
    Q_OBJECT
public:
    MainTask(QObject *parent = 0);// : QObject(parent) {}

public slots:
    void run();

signals:
    void finished();
};

#endif // _MAIN_TASK_H