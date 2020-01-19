#ifndef EVENTTIMER_H
#define EVENTTIMER_H
#include <QThread>
#include <QTimer>
#include <QMutex>
#include "hpusb.h"

class MainWindow;

class EventTimer : public QObject
{
    Q_OBJECT

private:
    MainWindow * main;
    hpusb * hpapi=nullptr;
    QTimer * timer=nullptr;
    mutable QMutex mutex;

public:
    EventTimer(MainWindow * main);
    ~EventTimer();
public slots:
     void timerAction();
     void start();
     void exit();
     void stopTimer();
     void tickEvent();

signals:
     void stop();
     void stopped();
     void tick();
};

#endif // EVENTTHREAD_H
