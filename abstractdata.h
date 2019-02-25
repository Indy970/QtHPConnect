#ifndef ABSTRACTDATA_H
#define ABSTRACTDATA_H

#include <QString>
#include <QByteArray>
#include "hpusb.h"

enum hp_DataType{
                HP_MAIN=0,
                HP_APP=1,
                HP_CAS=2,
                HP_COMPLEX=3,
                HP_LIST=4,
                HP_MATRIX=5,
                HP_NOTE=6,
                HP_PROG=7,
                HP_REAL=8,
                HP_VAR=9,
                HP_SCREEN=10
};

class AbstractData
{

private:
    QString name;
    hp_DataType type;
    hp_pkt_type file_code;
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
    void setData(QByteArray);
    QByteArray getData();
};


class Application: public AbstractData
{
public:
    Application(QString, hp_DataType);
};

class Real: public AbstractData
{
public:
    Real(QString, hp_DataType);
};

class Complex: public AbstractData
{
public:
    Complex(QString, hp_DataType);
};

class List: public AbstractData
{
public:
    List(QString, hp_DataType);
};

class Matrix: public AbstractData
{
public:
    Matrix(QString, hp_DataType);
};

class Program: public AbstractData
{
private:
    QString data;
public:
    Program(QString, hp_DataType, QString);
    void setProg(QString);
    QString getProg();
};

class Notes: public AbstractData
{
public:
    Notes(QString, hp_DataType);
};



#endif // ABSTRACTDATA_H
