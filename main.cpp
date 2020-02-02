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

#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qthpconnect);

    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("");
    QCoreApplication::setApplicationName("Linux QtHP Connect");
    QCoreApplication::setApplicationVersion(HP_VERSION_STRING);
    QCommandLineParser parser;
    MainWindow w;

    parser.setApplicationDescription(QCoreApplication::applicationName());
    //REWORK
    parser.addHelpOption();
    parser.addVersionOption();
//    parser.addPositionalArgument("file", "The file to open.");
    parser.process(a);

//    a.setQuitLockEnabled(true);
//    if (!parser.positionalArguments().isEmpty())
//        w.loadFile(parser.positionalArguments().first());
    w.show();
    a.exec();
    qDebug()<<"Main.cpp:: closing main";
    return 1;
}
