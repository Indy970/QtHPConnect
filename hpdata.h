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
                HP_VAR=9
};

struct hp_Information {
                QString name="---";
                QString serialnum="---";
                QString appver="-";
                QString osver="-";
};

struct hpDataStruct {

};

struct hp_Change {
    DataType dataChange=HP_MAIN;
};

class hpCalcData: public QObject
{
    Q_OBJECT

private:
    const static QString func_list[][2];
    const static DataType func_type[];
    DataType type;
    hpusb * hp_api;
    hp_Handle hp_handle;
    hp_Information hp_info;

public:
    hpCalcData(hpusb * hpapi);
    ~hpCalcData();
    hp_Handle * getHandle();
    hpusb * getAPI();
    void setInfo(hp_Information);
    void readInfo();
    hp_Information getInfo();
    QString getName();
    void vpkt_send_experiments(int );

//public slots:

signals:
    void dataChanged(hp_Change datachange);
};

#endif // HPDATA_H


