#include "eventthread.h"
#include <mainwindow.h>
#include <QDebug>

EventThread::EventThread(MainWindow * parent):QThread (parent)
{

    main=parent;
    if (main)
        hpapi=main->getAPI();

    if (hpapi==nullptr)
        qDebug()<<"hpusb not started";
}

void EventThread::run()  {

    qDebug()<<"Event run";
//    emit startTimer();
    hpapi->eventHandler();
    qDebug()<<"Event end";
    //exec();
}


