#ifndef HPDATA_H
#define HPDATA_H

#include <QObject>
#include <QString>
#include <QList>
#include "hpusb.h"
#include "abstractdata.h"

class AbstractData;

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
    hp_DataType dataChange=HP_MAIN;
    hpCalcData * calc;
};

struct hp_Prog {
    QString filename;
    QString prog;
};

struct hp_Data {
    QString name;
    hp_DataType type;
    QByteArray data;
};

class hpCalcData: public QObject
{
    Q_OBJECT

private:
    const static QString func_list[][2];
    const static hp_DataType func_type[];
    QPixmap * screenShot=nullptr;
    hp_DataType type;
    hpusb * hp_api;
    hp_Handle hp_handle;
    hp_Information hp_info;
    hp_Settings hp_homesettings;
    QList<AbstractData *> lData;

public:
    hpCalcData(hpusb * hpapi);
    ~hpCalcData();
    hp_Handle * getHandle();
    hpusb * getAPI();
    void setInfo(hp_Information);
    void readInfo();
    void readSettings();
    void readScreen();
    void recvScreen(hp_ScreenShot);
    void recvSettings(hp_Settings);
    void recvInfo(hp_Information);
    void recvData(hp_Data);
    void recvProg(hp_Prog);

    void addData(AbstractData *);
    void deleteData(AbstractData *);
    int findData(QString name, hp_DataType dataType);
    AbstractData * dataAt(int i);
    int dataCount();

    hp_ScreenShot getScreenShot();
    hp_Information getInfo();
    QString getName();
    hp_Settings getSettings();
    int setSettings(hp_Settings set);
    void vpkt_send_experiments(int );
    void emitChange(hp_DataType type);

//public slots:

signals:
    void dataChanged(hp_Change datachange);
};

#endif // HPDATA_H


