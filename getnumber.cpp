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

#include "getnumber.h"
#include "ui_getnumber.h"
#include <QDebug>

getNumber::getNumber(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::getNumber)
{
    ui->setupUi(this);
}

getNumber::~getNumber()
{
 //   qDebug()<<"Entering ~getNumber()";
    delete ui;
}
