/*
 * QtHP Connect: hand-helds support interface.
 * Copyright (C) 2019 Ian Gebbie
 * Code patterns and snippets borrowed from libhpcalcs :
 * Copyright (C) 1999-2009 Romain Li�vin
 * Copyright (C) 2009-2013 Lionel Debroux
 * Copyright (C) 1999-2013 libti* contributors.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "options.h"
#include "ui_options.h"
#include "global.h"
#include <QSettings>


Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
    QString path;

    QSettings appSettings("IRGP","QtHPconnect");
    path=appSettings.value("contentPath").toString();
    ui->filePath->setText(path);
}

void Options::accept() {

    QString path;
    path=ui->filePath->text();
    QSettings appSettings("IRGP","QtHPconnect");
    appSettings.setValue("contentPath",path);
    QDialog::accept();
}

void Options::reject() {
    QDialog::reject();
}

Options::~Options()
{
    qDebug()<<"~Options()";
    delete ui;
}
