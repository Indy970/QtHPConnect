#ifndef ABSTRACTDATA_H
#define ABSTRACTDATA_H

#include <QString>
#include <QByteArray>
#include "hpusb.h"
#include "matrixdata.h"

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
                HP_SCREEN=10,
                HP_SETTINGS=10
};

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
    virtual QByteArray getData();
    virtual void parseData();
    virtual void parseData(QDataStream& in);
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
    void parseData();
public:
    List(QString, hp_DataType);
    QByteArray getData();
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
    void parseData();
public:
    Matrix(QString, hp_DataType);
    itemData getListItem(int row, int column);
    void setListItem(int, int, itemData);
    QString getItem(int row, int column);
    void setItem(int, int, QString);
    void setItem(int, int, QString, double);
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
};

class Notes:  public AbstractData
{
private:
        QString text;
        QString format;
        void parseData();
public:
    Notes(QString, hp_DataType, QString);
    void setNote(QString);
    QString getNote();
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
