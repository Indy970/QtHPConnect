/*
 * This file is part of the QtHPConnect distribution (https://github.com/Indy970/QtHPConnect.git).
 * Copyright (c) 2020 Ian Gebbie.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 or later.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
