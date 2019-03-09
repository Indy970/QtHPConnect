#ifndef EVENTTHREAD_H
#define EVENTTHREAD_H
#include <QThread>
#include "hpusb.h"

class MainWindow;

class EventThread : public QObject
{
    Q_OBJECT
private:
    MainWindow * main;
    hpusb * hpapi;
public:
    EventThread(MainWindow * parent);

public slots:
     void timerEvent();
     void start();

};

#endif // EVENTTHREAD_H
