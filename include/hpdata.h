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

#ifndef HPDATA_H
#define HPDATA_H

#include <QObject>
#include <QString>
#include <QList>
#include <iterator>
#include "hpusb.h"
#include "abstractdata.h"
#include "hp_typedef.h"

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
    QString calculatorName;

public:
    hpCalcData(QString name, hpusb * hpapi);
    ~hpCalcData();
    hp_Handle * getHandle();
    hpusb * getAPI();
    void setInfo(hp_Information);
    void readInfo();
    void readSettings();
    void readScreen();
    void recvScreen(hp_ScreenShot);
    void recvSettings(hp_Data);
    void recvInfo(hp_Information);
    void recvData(hp_Data);
    void recvProg(hp_Prog);
    void recvNote(hp_Note);
    void refresh();
    void addData(AbstractData *);
    void deleteData(AbstractData *);
    QList<AbstractData *>::iterator findData(QString name, hp_DataType dataType);
    AbstractData * getData(QString name, hp_DataType dataType);
    AbstractData * dataAt(int i);
    int dataCount();

    hp_ScreenShot getScreenShot();
    hp_Information getInfo();
    QString getName();
    hp_Settings getSettings();
    QString getCalculatorName();
    int setSettings(hp_Settings set);
    void vpkt_send_experiments(int );
    void emitChange(hp_DataType type);


//public slots:

signals:
    void dataChanged(hp_Change datachange);
    void hotplugDetected();
};

#endif // HPDATA_H


