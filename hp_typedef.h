#ifndef HP_TYPEDEF_H
#define HP_TYPEDEF_H

#include <QString>
#include <QPixmap>
//#include "abstractdata.h"

#define FUNC_NUM 10

typedef  enum {
    // 5 is triggered periodically by the official connectivity kit. It returns something with a PNG header, but much smaller.
    CALC_SCREENSHOT_FORMAT_FIRST = 8,
    CALC_SCREENSHOT_FORMAT_PRIME_PNG_320x240x16 = 8,
    CALC_SCREENSHOT_FORMAT_PRIME_PNG_320x240x4 = 9,
    CALC_SCREENSHOT_FORMAT_PRIME_PNG_160x120x16 = 10,
    CALC_SCREENSHOT_FORMAT_PRIME_PNG_160x120x4 = 11,
    CALC_SCREENSHOT_FORMAT_LAST ///< Keep this one last
} hp_screenshot_format;

enum hp_DataType{
                HP_MAIN=0,
                HP_APP=1,
                HP_CAS=2,
                HP_REAL=3,
                HP_COMPLEX=4,
                HP_LIST=5,
                HP_MATRIX=6,
                HP_NOTE=7,
                HP_PROG=8,
                HP_VAR=9,
                HP_SCREEN=10,
                HP_SETTINGS=10
};

const QString mimetypes[FUNC_NUM][2]={     {"Main","application/x-calc"},
                                           {"Application Library","application/x-application"},
                                           {"CAS Vars","application/x-casvars"},
                                           {"Real","application/x-real"},
                                           {"Complex","application/x-complex"},
                                           {"Lists","application/x-list"},
                                           {"Matrices","application/x-matrix"},
                                           {"Notes","application/x-note"},
                                           {"Programs","application/x-programme"},
                                           {"Variables","application/x-var"}
                                           };

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
