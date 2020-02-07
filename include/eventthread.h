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
