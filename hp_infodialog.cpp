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

#include "hp_infodialog.h"
#include "ui_hp_infodialog.h"
#include <QDebug>

hp_infoDialog::hp_infoDialog(QWidget *parent, hp_Information info) :
    QDialog(parent),
    ui(new Ui::hp_infoDialog)
{
    ui->setupUi(this);
    ui->serialnum->setText(info.serialnum);
    ui->appver->setText(info.appver);
    ui->oppver->setText(info.osver);
}

hp_infoDialog::~hp_infoDialog()
{
 //  qDebug()<<"Entering ~hp_infoDialog()";
    delete ui;
}
