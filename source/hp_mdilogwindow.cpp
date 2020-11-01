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

#include "hp_mdilogwindow.h"
#include <QDebug>

hp_MdiLogWindow::hp_MdiLogWindow(QWidget * parent)
    :hp_MdiWindow(parent)
{
    setMinimumSize(200,200);
    setMaximumSize(1000,1000);
    setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    setup();
    resize(500,400);
}

void hp_MdiLogWindow::setup() {

    textEdit = new QTextEdit(this);
    setWidget(textEdit);
}

QTextEdit * hp_MdiLogWindow::getEditor() {
    return textEdit;
}

void hp_MdiLogWindow::show() {
    textEdit->show();
    hp_MdiWindow::show();
}

hp_MdiLogWindow::~hp_MdiLogWindow() {

    qDebug()<<"Entering ~hpmdiLogWindow()";

}
