#ifndef EVENTTHREAD_H
#define EVENTTHREAD_H

#include <QThread>
#include <hpusb.h>

class MainWindow;

class EventThread : public QThread
{
    Q_OBJECT

private:
    MainWindow * main;
    hpusb * hpapi=nullptr;
    mutable QMutex mutex;

public:
    EventThread(MainWindow * parent);
    void run() override;

signals:
    void startTimer();

};

#endif // EVENTTHREAD_H
