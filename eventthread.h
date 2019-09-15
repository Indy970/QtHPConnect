#ifndef EVENTTHREAD_H
#define EVENTTHREAD_H
#include <QThread>
#include <QTimer>
#include "hpusb.h"

class MainWindow;

class EventThread : public QObject
{
    Q_OBJECT
private:
    MainWindow * main;
    hpusb * hpapi;
    QTimer * timer=nullptr;
public:
    EventThread(MainWindow * parent);
    ~EventThread();
public slots:
     void timerEvent();
     void start();
     void exit();
     void stopTimer();

signals:
     void stop();
};

#endif // EVENTTHREAD_H
