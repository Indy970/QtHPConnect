#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QMutexLocker>
#include <QDateTime>
#include <QDebug>

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
