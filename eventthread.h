#ifndef EVENTTHREAD_H
#define EVENTTHREAD_H
#include <QThread>
#include <QTimer>
#include <QMutex>
#include "hpusb.h"

class MainWindow;

class EventThread : public QObject
{
    Q_OBJECT
private:
    MainWindow * main;
    hpusb * hpapi;
    QTimer * timer=nullptr;
    mutable QMutex mutex;

public:
    EventThread(MainWindow * parent);
    ~EventThread();
public slots:
     void timerAction();
     void start();
     void exit();
     void stopTimer();

signals:
     void stop();
     void stopped();
};

#endif // EVENTTHREAD_H
