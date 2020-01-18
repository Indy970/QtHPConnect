#ifndef HP_TYPEDEF_H
#define HP_TYPEDEF_H

#include <QString>
#include <QPixmap>
#include "abstractdata.h"

#define FUNC_NUM 9


    struct hp_DataStruct {
        QString filename;
        hp_DataType type;
    };

    class hpCalcData;

    struct hp_ScreenShot {
        QPixmap * image;
        hp_screenshot_format format;
        hpCalcData * calc;
    };

    struct hp_Change {
        hp_DataType dataChange=HP_MAIN;
        hpCalcData * calc;
    };

    struct hp_Prog {
        QString filename;
        QString prog;
        QByteArray data;
    };

    struct hp_Note {
        QString filename;
        QString text;
        QByteArray data;
    };

    struct hp_Data {
        QString name;
        hp_DataType type;
        QByteArray data;
    };




#endif // HP_TYPEDEF_H
