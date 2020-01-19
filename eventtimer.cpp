#include "eventtimer.h"
#include "mainwindow.h"

EventTimer::EventTimer(MainWindow * main):QObject()
{

    if (main)
        hpapi=main->getAPI();

    if (hpapi==nullptr)
        qDebug()<<"hpusb not started";

}

void EventTimer::timerAction()
{
  QMutexLocker locker(&mutex);

 // qDebug()<<"In Eventhandler";
 // QThread::msleep(1);

    if(hpapi!=nullptr) {
        hpapi->eventHandler();
    }
    emit tick();
}

void EventTimer::start() {
    timer = new QTimer(this);
    timer->setInterval(100);
 //   timer->callOnTimeout(SLOT(timerEvent()),Qt::AutoConnection);
    timer->connect(timer, SIGNAL(timeout()), this, SLOT(timerAction()));
 //   timer->connect(timer, SIGNAL(timeout()), hpapi, SLOT(eventHandler()),Qt::DirectConnection);

//    timer->connect(this, SIGNAL(tick()), this, SLOT(tickEvent()),Qt::AutoConnection);

    timer->connect(this, SIGNAL(stop()), this, SLOT(stopTimer()));
    timer->start();
    qDebug()<<"EventTimer::started Timer";
}

void EventTimer::exit() {
    emit stop();
    stop();
}

void EventTimer::stopTimer() {
    qDebug()<<"EventTimer::stop Timer";
    timer->stop();
    emit stopped();
}

void EventTimer::tickEvent() {
    qDebug()<<"EventTimer::tickEvent";
}

EventTimer::~EventTimer()
{
    stopTimer();
    if (timer!=nullptr) {
        delete timer;
        timer=nullptr;
    }
    qDebug()<<"delete EventTimer";
}
