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

#include "hp_mdiwindow.h"
#include <QDebug>
#include <QMenu>
#include <QToolBar>
#include <QToolButton>
#include <QBoxLayout>

hp_MdiWindow::hp_MdiWindow(QWidget * parent)
    :QMdiSubWindow(parent)
{
 //   setMinimumSize(500,400);
 //   setMaximumSize(1000,1000);
    setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    setup();
}

void hp_MdiWindow::setup() {


}

void hp_MdiWindow::eventSave() {
    qDebug()<<"Save pressed";
}

bool hp_MdiWindow::save() {

    qDebug()<<"hp_MdiWindow::save";
    return false;
}

bool hp_MdiWindow::saveAs() {

    qDebug()<<"hp_MdiWindow::saveas";
    return false;
}

void hp_MdiWindow::show() {

    QMdiSubWindow::show();
}

hp_MdiWindow::~hp_MdiWindow() {

    qDebug()<<"Entering ~hpmdiWindow()";

}
