#include "eventthread.h"
#include <QTimer>
#include "mainwindow.h"

EventThread::EventThread(MainWindow * parent)
{
    main=parent;
    if (main)
        hpapi=main->getAPI();
}

void EventThread::timerEvent()
{
    if(hpapi) {
//        qDebug()<<"In Eventhandler";
        hpapi->eventHandler();
    }
}

void EventThread::start() {
    QTimer* timer = new QTimer(this);
    timer->setInterval(100);
    timer->connect(timer, SIGNAL(timeout()), this, SLOT(timerEvent()));
    timer->start();
}
