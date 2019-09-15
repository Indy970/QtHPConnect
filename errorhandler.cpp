#include "errorhandler.h"

errorHandler::errorHandler()
{
}

errorHandler::errorHandler(QObject * parent)
{
    pParent = parent;
}

QString errorHandler::getLogFileName()
{
    return QString("hpinterface.log");
}

int errorHandler::writeLog(QString lineToBeLogged)
{

    QMutexLocker locker(&m_lineLoggerMutex);
    QFile fileLog(getLogFileName());

    //doRollLogsIfNeeded(static_cast<qint64>(fileLog.size() + lineToBeLogged.length()));

    // Do not open in append mode but seek() to avoid warning for unseekable
    // devices, note that if open is made with WriteOnly without Append, the
    // file gets truncated
    if (!fileLog.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream out(stdout);
        out << "CANNOT OPEN LOG FILE: " << getLogFileName();
        return -1;
    }
    // seek() does nothing on sequential devices, this is in essence what QFile
    // does when Append flag is set in open()
    if (!fileLog.isSequential())
    {
        fileLog.seek(fileLog.size());
    }
    QDateTime time(QDateTime::currentDateTime());

    QTextStream out(&fileLog);
    out << time.toString("yyyy/MM/dd hh:mm:ss") << " "<< lineToBeLogged << endl;

    fileLog.close();

    return 0;
}

int errorHandler::writeStatus(QString msg)
{
     if (MainWindow *pb = qobject_cast<MainWindow*> (pParent))
     {
         pb->writeStatus( msg);
     }
     return 0;
}

int errorHandler::writeChatter(QString msg)
{
     if (MainWindow *pb = qobject_cast<MainWindow*> (pParent))
     {
         pb->writeChatter( msg);
     }
     return 0;
}

int errorHandler::error(ErrLevel el, int num, QString msg, QString Data=QString())
{
    switch (el) {
       case L0:
            writeLog("Abort: "+msg);
            writeStatus("Abort: "+msg);
            exit(num);
        break;
       default:
            writeLog(" "+msg+Data);
            writeStatus(" "+msg);
            if (Data=="")
                writeChatter(" "+msg);
            else
                writeChatter(" "+msg+Data);
        break;
    }
    return 0;
}

int errorHandler::dump(uint8_t * data, int size)
{
    int i,j;
    QString text = QString("Dump:\n");
    QString texta = QString("ASCII:\n");
    j=0;
   text = text + QString().sprintf("%04d | ",0);
    for (i=0; i< size; i++)
    {
            text = text + QString(" %1 ").arg(QChar(data[i]).unicode(),2,16,QChar('0'));
            j++;
            if (j>15) {
                j=0;
                text=text+"\n";
                text = text + QString().sprintf("%04d | ",i+1);
            }
            texta = texta +" "+QChar(data[i]).unicode();

    }
    text = text + "\n";
    texta = texta + "\n";

    writeLog(text);
    writeChatter(text);
    writeLog(texta);
    writeChatter(texta);

    return 0;
}

errorHandler::~errorHandler() {
    qDebug()<<"errorHandler:: close";
}
