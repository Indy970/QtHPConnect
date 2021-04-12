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


#ifndef ABSTRACTDATA_H
#define ABSTRACTDATA_H

#include <QString>
#include <QByteArray>
#include "hp_typedef.h"
#include "hpusb.h"
#include "matrixdata.h"

struct m_Size {
    int row;
    int column;
};

class AbstractData
{

private:
    QString name;
    hp_DataType type;
    hp_pkt_type file_code;

protected:
    QByteArray data;

public:
    AbstractData(QString, hp_DataType);
    inline bool operator ==(AbstractData& lhs);
    bool equivalent(QString, hp_DataType);
    void setName(QString);
    QString getName();
    void setType(hp_DataType);
    hp_DataType getType();
    void setFileCode(hp_pkt_type);
    hp_pkt_type getFileCode();
    virtual void setData(QByteArray);
    virtual QByteArray getData(); //data extract
    virtual bool getData(QDataStream &ds);
    virtual void parseData();
    virtual void parseData(QDataStream& in);
    virtual QByteArray fileOut(); //full data out in file transmittable format
    virtual ~AbstractData();
};


class Application: public AbstractData
{
private:
    void parseData();
public:
    Application(QString, hp_DataType);
};

class Real:  public AbstractData
{
private:
    QList <itemData>values;
    void parseData();
public:
    Real(QString, hp_DataType);
    itemData getListItem(int);
    void setListItem(int, itemData);
    QString getItem(int);
    void setItem(int, QString);
    void setItem(int, QString, double);
    int getListSize();
};

class Complex: public AbstractData
{
private:
    QList <itemData>values;
    void parseData();
public:
    Complex(QString, hp_DataType);
    itemData getListItem(int);
    void setListItem(int, itemData);
    QString getItem(int);
    void setItem(int, QString);
    void setItem(int, QString, double);
    int getListSize();
};


class List:  public AbstractData
{
private:
    QList <itemData>values;
    void parseData() override;
public:
    List(QString, hp_DataType);
    QByteArray getData() override;
    itemData getListItem(int);
    void setListItem(int, itemData);
    QString getItem(int);
    void setItem(int, QString);
    void setItem(int, QString, double);
    int getListSize();
};

class Matrix:  public AbstractData
{
private:
    MatrixData mdata;
public:
    Matrix(QString, hp_DataType);
    itemData getListItem(int row, int column);
    void setListItem(int, int, itemData);
    void parseData() override;
    virtual void parseData(QDataStream& in) override;
    QString getItem(int row, int column);
    void setItem(int, int, QString);
    void setItem(int, int, QString, double);
//    QByteArray getData() override;
    bool getData(QDataStream &ds);
    m_Size getMatrixSize();
    int getMatrixRows();
    int getMatrixColumns();
};

class Program:  public AbstractData
{
private:
    QString text;

public:
    Program(QString, hp_DataType, QString);
    void setProg(QString);
    QString getProg();
    void parseData();
    virtual void parseData(QDataStream& in);
    virtual QByteArray fileOut();
    virtual ~Program();
};

class Notes:  public AbstractData
{
private:
        QString text;
        QString format;

public:
    Notes(QString, hp_DataType, QString);
    void setNote(QString);
    QString getNote();
    void parseData();
};

class Variables:  public AbstractData
{
private:
        QString text;
        QString format;
        void parseData();
public:
    Variables(QString, hp_DataType);
};

class CASVariables:  public AbstractData
{
private:
        QString text;
        QString format;
        void parseData();
public:
    CASVariables(QString, hp_DataType);
};

class Settings:  public AbstractData
{
private:
        QString text;
        QString format;
        void parseData();

public:
    Settings(QString, hp_DataType);
    virtual void setData(QByteArray);
};

#endif // ABSTRACTDATA_H
