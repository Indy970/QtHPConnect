#ifndef HPDATA_H
#define HPDATA_H

#include <QObject>
#include <QString>
#include "hpusb.h"

enum DataType {
                HP_MAIN=0,
                HP_APP=1,
                HP_CAS=2,
                HP_COMPLEX=3,
                HP_LIST=4,
                HP_MATRIC=5,
                HP_NOTE=6,
                HP_PROG=7,
                HP_REAL=8,
                HP_VAR=9,
                HP_SCREEN=10
};

struct hp_Information {
                QString name="---";
                QString serialnum="---";
                QString appver="-";
                QString osver="-";
};

struct hp_Settings {
                int angle_measure=0;
                int number_format=0;
                int precision=5;
                int digit_grouping=0;
                int entry=2;
                int integers=3;
                int bits=32;
                bool signed_int=false;
                int complex=0;
                int language =1;
                int font_size=2;
                int theme=1;
                bool textbook=true;
                bool menu=false;
};


struct hpDataStruct {

};

class hpCalcData;

struct hp_ScreenShot {
    QPixmap * image;
    hp_screenshot_format format;
    hpCalcData * calc;
};

struct hp_Change {
    DataType dataChange=HP_MAIN;
    hpCalcData * calc;
};

class hpCalcData: public QObject
{
    Q_OBJECT

private:
    const static QString func_list[][2];
    const static DataType func_type[];
    QPixmap * screenShot=nullptr;
    DataType type;
    hpusb * hp_api;
    hp_Handle hp_handle;
    hp_Information hp_info;
    hp_Settings hp_homesettings;

public:
    hpCalcData(hpusb * hpapi);
    ~hpCalcData();
    hp_Handle * getHandle();
    hpusb * getAPI();
    void setInfo(hp_Information);
    void readInfo();
    void readSettings();
    void readScreen();
    hp_ScreenShot getScreenShot();
    hp_Information getInfo();
    QString getName();
    hp_Settings getSettings();
    int setSettings(hp_Settings set);
    void vpkt_send_experiments(int );
    void emitChange(DataType type);

//public slots:

signals:
    void dataChanged(hp_Change datachange);
};

#endif // HPDATA_H


