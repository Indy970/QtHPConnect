#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QMutexLocker>
#include <QDateTime>
#include <QDebug>
#include <mainwindow.h>

enum ErrLevel {
    L0,    //abort
    L1,    //fault
    L2,    //warning
    L3,
    L4,
    L7     //information
};

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
    int error(ErrLevel, int, QString, QString Data);
    int dump(uint8_t *, int);
};

#endif // ERRORHANDLER_H
