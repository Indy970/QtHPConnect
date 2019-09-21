#include "eventthread.h"
#include "mainwindow.h"

EventThread::EventThread(MainWindow * parent)
{
    main=parent;
    if (main)
        hpapi=main->getAPI();
}

void EventThread::timerAction()
{
    QMutexLocker locker(&mutex);

 // qDebug()<<"In Eventhandler";
 // QThread::msleep(1);

    if(hpapi) {
        hpapi->eventHandler();
    }
}

void EventThread::start() {
    timer = new QTimer(this);
    timer->setInterval(100);
 //   timer->callOnTimeout(SLOT(timerEvent()),Qt::AutoConnection);
    timer->connect(timer, SIGNAL(timeout()), this, SLOT(timerAction()),Qt::AutoConnection);
    timer->connect(this, SIGNAL(stop()), this, SLOT(stopTimer()));
    timer->start();
    qDebug()<<"EventThread::started Timer";
}

void EventThread::exit() {
    emit stop();
    stop();
}

void EventThread::stopTimer() {
    qDebug()<<"EventThread::stop Timer";
    timer->stop();
    emit stopped();
}

EventThread::~EventThread()
{
    stopTimer();
    if (timer!=nullptr) {
        delete timer;
        timer=nullptr;
    }
    qDebug()<<"delete eventThread";
}
