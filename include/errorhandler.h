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

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QMutexLocker>
#include <QDateTime>
#include <QDebug>

#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

enum ErrLevel {
    L0,    //abort
    L1,    //fault
    L2,    //warning
    L3,
    L4,
    L7     //information
};

typedef enum {

    ERR_SUCCESS = 0, // Must be equal to ERR_HPLIBS_GENERIC_FIRST
    ERR_MALLOC,
    ERR_INVALID_HANDLE,
    ERR_INVALID_PARAMETER,
    ERR_INVALID_MODEL,
    ERR_LIBRARY_INIT,
    ERR_LIBRARY_EXIT,
    ERR_LIBRARY_CONFIG_VERSION,

    ERR_FILE_FIRST,
    ERR_FILE_FILENAME,
    ERR_FILE_LAST

} hp_error;


class errorHandler {
private:
    QObject * pParent;
    QMutex m_lineLoggerMutex;

    QString getLogFileName();
    int writeLog(QString);
    int writeStatus(QString);
    int writeChatter(QString);


public:
    errorHandler();
    errorHandler(QObject *);
    ~errorHandler();
    int error(ErrLevel, int, QString, QString Data);
    int dump(uint8_t *, int);
};

#endif // ERRORHANDLER_H
