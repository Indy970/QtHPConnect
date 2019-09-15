#include "eventthread.h"

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
    timer = new QTimer(this);
    timer->setInterval(100);
    timer->connect(timer, SIGNAL(timeout()), this, SLOT(timerEvent()));
    timer->connect(this, SIGNAL(stop()), this, SLOT(stopTimer()));
    timer->start();
}

void EventThread::exit() {
    stop();
}

void EventThread::stopTimer() {
    timer->stop();
}

EventThread::~EventThread()
{

    if (timer!=nullptr) {
        delete timer;
        timer=nullptr;
    }
    qDebug()<<"delete eventThread";
}
