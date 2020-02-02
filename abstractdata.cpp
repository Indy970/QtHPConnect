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

#include <QDebug>
#include <QTextCodec>
#include <QtMath>
#include <QByteArrayMatcher>
#include <QByteArray>

#include "global.h"
#include "abstractdata.h"
#include "hpusb.h"
#include "matrixdata.h"

uint16_t crc16_block(const uint8_t * buffer, uint32_t len);

int BCD2I(quint8 num) {

        int ds,ret;
        if(num>0) {
            ds=((num&0xF0)>>4)*10;
            ds+=(num&0x0F);
            ret=ds;
        }
        else {
            ret=0;
        }

   log(QString("Num= %1 Ret= %2").arg(num,1,16).arg(ret));
    return ret;
}

qint32 TwosComplement2Int(quint32 rawValue)
{

    qint32 ret;

    log(QString("2C: %1").arg(rawValue& 0x80000000,1,16));

    // If a positive value, return it
    if ((rawValue & 0x80000000) == 0)
    {
        ret = static_cast<qint32>(rawValue);
    }
    else {
       // Otherwise perform the 2's complement math on the value
      log(QString("List 2C Negative Detected"));

       ret = static_cast<qint32>(~(rawValue - 0x01)) * - 1;
//          ret = ~rawValue;
    }
    log(QString("2C: %1 %2").arg(rawValue,1,16).arg(ret,1,16));

    return ret;
}

qint16 TwosComp2Int_8(qint16 rawValue)
{

    qint16 ret;

    // If a positive value, return it
    if ((rawValue & 0x800) == 0)
    {
        ret = rawValue;
    }
    else {
       // Otherwise perform the 2's complement math on the value
       ret = (~(rawValue - 0x01)&(0x0FFF)) * -1;
//          ret = ~rawValue;
    }
    return ret;
}

QString value2Str(int sign, double m, double exp) {
    QString value;
    QString neg=QStringLiteral("");

    if (sign<0) {
        neg=QStringLiteral("-");
    }

    if (exp!=0.0) {
        value=neg+QString("%1E%2").arg(m).arg(exp);
    }
    else {
        if (m==0.0) {
            value=neg+QString("0");
        }
        else
            value=neg+QString("%1").arg(m);
    }

    return value;
}

QString complex2Str(QString real, QString imag) {
    QString value;

    if((imag[0]!="+")&&(imag[0]!="-")) {
       value = real+"+"+imag+"*i";
    }
    else
    {
        value = real+imag+"*i";
    }
    if(real=="0") {
        value = imag+"*i";
    }
    if ((real!="0")&&(imag=="0")) {
        value = real;
    }

    return value;
}

itemData extract16(QByteArray item) {
;
    itemData listvalue;
    QString msg;
    QString value=QStringLiteral("");
    QString neg=QStringLiteral("");
    qint8 sign;
    int multi;
    long exp;
    double base;
    double m,ret;
    int k;

        multi=1;
//        base=BCD2I(item[2]);
        base=10;

        sign=TwosComplement2Int(static_cast<qint8>(item[3]));

        exp=((((((((static_cast<qint8>(item[7])&0xFF)<<8)+(static_cast<qint8>(item[6])&0xFF))<<8)+(static_cast<qint8>(item[5])&0xFF)))<<8)
                +(static_cast<qint8>(item[4])&0xFF));
        log(QString("extract16: exp %1").arg(exp));
//        exp=TwosComplement2Int(exp);

        m=0;

        for (k=8;k<16;k++) {
            if(static_cast<quint8>(item[k])!=0) {
                m=m*0.01+static_cast<double>(multi)*BCD2I(static_cast<quint8>(item[k]));
            }
        }
        ret=sign*m*qPow(base,exp);

        value = value2Str(sign,m,exp);

        msg=QString("multi:%1 base:%2 sign:%3 exp:%4 m:%5").arg(multi).arg(base).arg(sign).arg(exp).arg(m);
//        msg=QString("value: %1").arg(value);

        log(msg);
        log((QString("Ans: %1").arg(ret)));

       listvalue.dReal=ret;
       listvalue.sValue=value;

       return listvalue;
}

itemData extract8(QByteArray item) {
;
    itemData listvalue;
    QString value=QStringLiteral("");;
    qint8 sign=1;
    quint8 l;
    qint8 v;
    int multi;
    qint16 exp=0;
    double base;
    double m,ret,n;
    int k;

    multi=1;
    base=10;

    exp=(((((static_cast<qint8>(item[1])&0x0F)))<<8)+(static_cast<qint8>(item[0])&0xFF));

    exp=TwosComp2Int_8(exp);

    m=0;

    l=item[7]&0xF0;

    if ((l&0xFF)==0x90) {

        sign=-1;
    }
    for (k=1;k<8;k++) {
          v=item[k];
          if (k==7) {
              v=v&0x0F;
           }
          if (k==1) {
              v=v&0xF0;
           }

          n=static_cast<double>(multi)*BCD2I(v);
          m=m*0.01+n;
          if(k!=7) {
            if ((m>0)||(k>5))
                value=QString("%1").arg(n,0)+value;
          }
          else
            value=QString("%1.").arg(n,0)+value;

    }
    ret=sign*m*qPow(base,exp);

    if(sign<0) {
        value=QString("-")+value;
    }

    if(exp!=0)
        value=value+"E"+QString("%1").arg(exp,0);

       listvalue.dReal=ret;
       listvalue.sValue=value;

       return listvalue;
}

////////////////////////

AbstractData::AbstractData(QString name_in, hp_DataType type_in)
{

    qDebug()<<"base class called "<<name_in;
    setName(name_in);
    setType(type_in);

}

//Equivalence on name and type NOT on data
inline bool AbstractData::operator ==(AbstractData& lhs) {

    if(name==lhs.getName()&&type==lhs.getType())
        return true;
    else
        return false;
}

//Equivalence on name and type NOT on data
bool AbstractData::equivalent(QString name_in, hp_DataType type_in) {

    if((getName()==name_in)&&(getType()==type_in))
        return true;
    else
        return false;
}

void AbstractData::setName(QString name_in) {
    qDebug()<<"Abstract set name"<<name_in;
    name = name_in;
    return;
}

QString AbstractData::getName() {
    return name;
}

void AbstractData::setType(hp_DataType type_in) {
    type = type_in;
}

hp_DataType AbstractData::getType() {
    return type;
}

void AbstractData::setFileCode(hp_pkt_type type_in) {
    file_code = type_in;
}

hp_pkt_type AbstractData::getFileCode() {
    return file_code;
}

void AbstractData::setData(QByteArray data_in) {
    data = data_in;
    parseData();
     qDebug()<<"AbstrtactData::setData()";

    return;
}

QByteArray AbstractData::getData() {
    qDebug()<<"AbstrtactData::getData()";
    return data;
}

QByteArray AbstractData::fileOut() {
    return getData();
}

void AbstractData::parseData() {
     qDebug()<<"AbstractData::parseData";
}

void AbstractData::parseData(QDataStream& in) {
     qDebug()<<"AbstractData::parseData(DS)";

     in.setByteOrder(QDataStream::LittleEndian);
     qint8 c;
     QByteArray a1;
     uint length;
     length=16;

     qDebug()<<"Parsing Matrix";
     in.startTransaction();
     while(!in.atEnd()) {
         in>>c;
         a1.append(c);
     }
     data = a1;
     parseData();
}

AbstractData::~AbstractData() {
     qDebug()<<"~AbstractData";
}

//REAL
//
Application::Application(QString name_in, hp_DataType type_in):
    AbstractData(name_in, type_in) {

    setFileCode(HP_TP_FUNCTIONS);
}

void Application::parseData() {

}

//REAL
//
Real::Real(QString name_in, hp_DataType type_in):
    AbstractData(name_in, type_in) {

    setFileCode(HP_TP_SETTINGS);
}

//Find the vars in the calc.hpsettings file. Marker b4 01 00
void Real::parseData() {

    QByteArray a1;
    int ind=0;
    int len;
    int j;
    int start;
    QByteArray searchstr=QByteArrayLiteral("\x0c\x00\xc0\x05");
    QByteArrayMatcher matcher(searchstr);

    QByteArray item;
    itemData value1,value2;
    itemData listvalue;

    QString name;
    log("Real: Parsing Vars");

    name=getName();
    log(name);
    a1.clear();
    a1=getData();

    main_err->dump((uint8_t *)searchstr.constData(),4);

 //   ind=a1.indexOf(searchstr,0);
    ind=matcher.indexIn(a1,0);
    if (ind>0)
        len=((((a1[ind-3])&0x0F)<<8)+((a1[ind-4])&0xFF)-4)/16;
    else
        len=0;

    log(QString("Real: %1 %2 %3").arg(ind,0,10)
        .arg(searchstr.size())
        .arg(len,2,10));
    //   ind=a1.indexOf(searchstr,0);
       ind=matcher.indexIn(a1,0);
       if (ind>0)
           len=((((a1[ind-3])&0x0F)<<8)+((a1[ind-4])&0xFF)-4)/16;
       else
           len=0;

       start=ind+4;
       qDebug()<<start;
       for(j=0;j<len;j++) {
           item=a1.mid(start,16);
           if (static_cast<int>(item[2])==0x13) {
               main_err->dump((uint8_t *)item.constData(),16);
               value1=extract16(item);
               j++;
               start=start+16;
               item=a1.mid(start,16);
               main_err->dump((uint8_t *)item.constData(),16);
               value2=extract16(item);
               listvalue.dReal=value1.dReal;
               listvalue.sValue=value1.sValue;
               listvalue.dImaginary=value2.dReal;
               listvalue.sValue=complex2Str(value1.sValue,value2.sValue);
               start=start+16;
           }
           else {
               main_err->dump((uint8_t *)item.constData(),16);
               listvalue=extract16(item);
               start=start+16;
           }

        setListItem(getListSize(),listvalue);
      }
//    log("Real Dump");
//    main_err->dump((uint8_t *)a1.constData(),a1.size());
}

//Gets a list item from the list
itemData Real::getListItem(int row) {

    itemData null;

    if (row<values.size()) {
        return values.at(row);
    }
    return null;
}

//Gets a list item in the list at position row
void Real::setListItem(int row, itemData item) {

    //May need to pad for missing items
    values.insert(row,item);

}

//Gets a  string representation of the list item in the list at position row
QString Real::getItem(int row) {
    return QString("%1").arg(values.at(row).dReal);
}

//Passes a  string representation of the list item in the list at position row
void Real::setItem(int row, QString string) {

    itemData item;
    item.dReal=string.toDouble();
    item.sValue=string;
    setListItem(row,item);
}

//Passes a  string representation of the list item in the list at position row
void Real::setItem(int row, QString string, double value) {

    itemData item;
    item.dReal=value;
    item.sValue=string;
    setListItem(row,item);
}

//Passes the number of entries in the list
int Real::getListSize() {

    return values.size();
}

//COMPLEX
Complex::Complex(QString name_in, hp_DataType type_in):
    AbstractData(name_in, type_in) {

    setFileCode(HP_TP_SETTINGS);
}

void Complex::parseData() {

    qDebug()<<"Complex: Parsing Vars";
    QByteArray a1;
    int ind=0;
    int len;
    int j;
    int start;
    QByteArray searchstr=QByteArrayLiteral("\x0c\x00\x80\x05");
    QByteArrayMatcher matcher(searchstr);

    QByteArray item;
    itemData value1;
    itemData value2;
    itemData listvalue;
    QString name;
    log("Complex: Parsing Vars");

    name=getName();
    log(name);
    a1.clear();
    a1=getData();

    //   ind=a1.indexOf(searchstr,0);
       ind=matcher.indexIn(a1,0);
       if (ind>0)
           len=((((a1[ind-3])&0x0F)<<8)+((a1[ind-4])&0xFF)-4)/16;
       else
           len=0;

       log(QString("Real: %1 %2 %3").arg(ind,0,10)
           .arg(searchstr.size())
           .arg(len,2,10));
       //   ind=a1.indexOf(searchstr,0);
          ind=matcher.indexIn(a1,0);
          if (ind>0)
              len=((((a1[ind-3])&0x0F)<<8)+((a1[ind-4])&0xFF)-4)/16;
          else
              len=0;

          start=ind+4;
          qDebug()<<start;
          for(j=0;j<len;j++) {
              item=a1.mid(start,16);
              if (static_cast<int>(item[2])==0x13) {
                  main_err->dump((uint8_t *)item.constData(),16);
                  value1=extract16(item);
                  j++;
                  start=start+16;
                  item=a1.mid(start,16);
                  main_err->dump((uint8_t *)item.constData(),16);
                  value2=extract16(item);
                  listvalue.dReal=value1.dReal;
                  listvalue.sValue=value1.sValue;
                  listvalue.dImaginary=value2.dReal;
                  listvalue.sValue=complex2Str(value1.sValue,value2.sValue);
                  start=start+16;
              }
              else {
                  main_err->dump((uint8_t *)item.constData(),16);
                  listvalue=extract16(item);
                  start=start+16;
              }

           setListItem(getListSize(),listvalue);
         }
//    main_err->dump((uint8_t *)a1.constData(),a1.size());
}

//Gets a list item from the list
itemData Complex::getListItem(int row) {

    itemData null;

    if (row<values.size()) {
        return values.at(row);
    }
    return null;
}

//Gets a list item in the list at position row
void Complex::setListItem(int row, itemData item) {

    //May need to pad for missing items
    values.insert(row,item);

}

//Gets a  string representation of the list item in the list at position row
QString Complex::getItem(int row) {
//    return QString("%1+%2i").arg(values.at(row).dReal).arg(values.at(row).dImaginary);
      return values.at(row).sValue;
}

//Passes a  string representation of the list item in the list at position row
void Complex::setItem(int row, QString string) {

    itemData item;
    item.dReal=string.toDouble();
    item.sValue=string;
    setListItem(row,item);
}

//Passes a  string representation of the list item in the list at position row
void Complex::setItem(int row, QString string, double value) {

    itemData item;
    item.dReal=value;
    item.sValue=string;
    setListItem(row,item);
}

//Passes the number of entries in the list
int Complex::getListSize() {

    return values.size();
}

//LIST
List::List(QString name_in, hp_DataType type_in):
    AbstractData(name_in, type_in) {

    setFileCode(HP_TP_LIST);
    values.clear();
}

QByteArray List::getData() {
    return data;
}

void List::parseData() {

    QByteArray a1;
    QByteArray item;
    itemData listvalue,value1,value2;
    QString name;
    int len;
    int ind;
    int start = 8;
    int j;
    unsigned char searchstr[] = {0xFF,0x16,0x00};

    qDebug()<<"List: Parsing a List";

    name=getName();

    a1=data;
    ind=a1.indexOf(reinterpret_cast<char *>(searchstr),0);

    //look for FF 16 00

    len=a1[ind+3];

    for(j=0;j<len;j++) {
        item=a1.mid(start,16);
        if (static_cast<int>(item[2])==0x13) {
            main_err->dump((uint8_t *)(item.constData()),16);
            value1=extract16(item);
            j++;
            start=start+16;
            item=a1.mid(start,16);
            main_err->dump((uint8_t *)item.constData(),16);
            value2=extract16(item);
            listvalue.dReal=value1.dReal;
            listvalue.sValue=value1.sValue;
            listvalue.dImaginary=value2.dReal;
            listvalue.sValue=complex2Str(value1.sValue,value2.sValue);
            start=start+16;
        }
        else {
            main_err->dump((uint8_t *)item.constData(),16);
            listvalue=extract16(item);
            start=start+16;
        }

        setListItem(-1,listvalue);

    }
    main_err->dump((uint8_t *)a1.constData(),a1.size());
}

//Gets a list item from the list
itemData List::getListItem(int row) {

    itemData null;

    if (row<values.size()) {
        return values.at(row);
    }
    return null;
}

//Gets a list item in the list at position row
void List::setListItem(int row, itemData item) {

    //May need to pad for missing items
    values.insert(row,item);

}

//Gets a  string representation of the list item in the list at position row
QString List::getItem(int row) {
    return QString("%1").arg(values.at(row).dReal);
}

//Passes a  string representation of the list item in the list at position row
void List::setItem(int row, QString string) {

    itemData item;
    item.dReal=string.toDouble();
    item.sValue=string;
    setListItem(row,item);
}

//Passes a  string representation of the list item in the list at position row
void List::setItem(int row, QString string, double value) {

    itemData item;
    item.dReal=value;
    item.sValue=string;
    setListItem(row,item);
}

//Passes the number of entries in the list
int List::getListSize() {

    return values.size();
}

//Matrix

Matrix::Matrix(QString name_in, hp_DataType type_in):
    AbstractData(name_in, type_in) {
    qDebug()<<"Create Matrix";
    setFileCode(HP_TP_MATRIX);
}

void Matrix::parseData(QDataStream & in) {

    in.setByteOrder(QDataStream::LittleEndian);
    qint8 c;
    QByteArray a1;
    uint length;
    length=16;

    qDebug()<<"Parsing Matrix";
    in.startTransaction();
    while(!in.atEnd()) {
        in>>c;
        a1.append(c);
    }
    data = a1;
    parseData();
}


void Matrix::parseData() {

    QByteArray a1;
    QByteArray item;
    itemData listvalue,value1,value2;
    QString name;
    QString msg;
    QString value=QStringLiteral("");;
    int mrows;
    int mcolumns;
    int start;
    int k,j;
    int ind;
    int flag =0;
    qint8 vtype;

    QString ds;
//    QByteArray searchstr((char *)std::begin<quint8>({0x01,0x00}),2);
   //Start keeps changing 01 or 02
   //0x14 real 0x94 Complex

    qDebug()<<"Matrix: Parsing a Matrix";

    name=getName();
    log(name);
    a1=data;
//    ind=a1.indexOf(searchstr,0);
    ind=0;
    vtype=a1[ind+2];

    log(QString("vtype=%1").arg(vtype,1,16));
    if ((vtype&0x7F)==0x14) {
       log("matrix found");
       flag =1;
    }
        log(QString("vtype=%1").arg(vtype&0x7F,1,16));
    if ((vtype&0x80)==0x80) {
       log("complex found");
       flag =2;
    }
    log(QString("vtype=%1").arg(vtype&0x80,1,16));

    //look for 00 14
    log(QString("Found string at %1").arg(ind));

    mcolumns=a1[ind+12];
    mrows=a1[ind+8];
    qDebug()<<QString("Matrix: Row %1 Column %2").arg(mrows).arg(mcolumns);
    log(QString("Matrix: Row %1 Column %2").arg(mrows).arg(mcolumns));

    start=ind+16;
    for(j=0;j<mrows;j++) {
        for(k=0;k<mcolumns;k++) {


        item=a1.mid(start,8);
        qDebug()<<item;
        listvalue=extract8(item);
        start+=8;

        //if complex
        if (flag==2) {
          item=a1.mid(start,8);
          start+=8;
          value2=extract8(item);
          if ((value2.dReal) != 0.0) {
            listvalue.dImaginary=value2.dReal;
            listvalue.sValue=complex2Str(listvalue.sValue,value2.sValue);
          }
        }

        setListItem(j,k,listvalue);
        }
    }

    main_err->dump((uint8_t *)a1.constData(),a1.size());
}

//Gets a item from the matrix
itemData Matrix::getListItem(int row, int column) {

    itemData data;
    data=mdata.at(row,column);
    return data;
}

//Gets a list item in the matrix at position row,column
void Matrix::setListItem(int row, int column, itemData item) {

    mdata.insert(row,column,item);
}

//Gets a  string representation of the list item in the matrix at position row,column
QString Matrix::getItem(int row, int column) {

    itemData data;
    QString item;

    data=mdata.at(row,column);
    item=data.sValue;
//    item=QString("%1").arg(data.dReal,5);
    return item;
}

//Passes a  string representation of the item in the matrix at position position row,column
void Matrix::setItem(int row, int column, QString string) {

    itemData item;
    item.dReal=string.toDouble();
    item.sValue=string;
    setListItem(row,column, item);
}

//Passes a  string representation of the list item in the matrix at position row,column
void Matrix::setItem(int row, int column, QString string, double value) {

    itemData item;
    item.dReal=value;
    item.sValue=string;
    setListItem(row,column,item);
}

//Passes the number of entries in the list
m_Size Matrix::getMatrixSize() {

    m_Size size;
    size.row=mdata.rows();
    size.column=mdata.columns();

    return size;
}

//Passes the number of entries in the list
int Matrix::getMatrixRows() {

    return mdata.rows();
}

//Passes the number of entries in the list
int Matrix::getMatrixColumns() {

    return mdata.columns();
}

//Program
Program::Program(QString name_in, hp_DataType type_in, QString data):
    AbstractData(name_in, type_in) {

    setFileCode(HP_TP_PROG);
    setProg(data);
}

QString Program::getProg() {
    return text;
}

void Program::setProg(QString data_in) {
    QByteArray ba_data=data_in.toUtf8();
    setData(ba_data);
    text=data_in;
    qDebug()<<text;
    return;
}

void Program::parseData() {

    QTextCodec * codec = QTextCodec::codecForName("UTF8");
    QByteArray a1;
    a1=getData();
    text = codec->toUnicode(a1);

    return;
}

void Program::parseData(QDataStream& in) {

    QTextCodec * codec = QTextCodec::codecForName("UTF-16");
    QByteArrayMatcher matcher;
    QByteArray search;
    QByteArray phrase;
    int ind;
    int pos;
    QString str;
    in.setByteOrder(QDataStream::LittleEndian);
    qint8 c;
    QByteArray a1;
    uint length;
    length=16;

    qDebug()<<"Parsing Program";
    in.startTransaction();
    while(!in.atEnd()) {
    in>>c;
    a1.append(c);
    }

    main_err->dump((uint8_t *)a1.constData(),a1.size());

    search="\x7c\x61";
    matcher.setPattern(search);
    ind=matcher.indexIn(a1,0);
    if (ind>-1) {
        search=QByteArrayLiteral("\x54\x0\x0\x0\x44\x0\x0\x0");
        main_err->dump((uint8_t *)search.data(),search.length());
        matcher.setPattern(search);
        pos=0;
        while(pos>-1) {
            pos=matcher.indexIn(a1,ind);
            if (pos>-1) {
                ind=pos+10;
                phrase=a1.mid(ind,16*3);
                //add array to catch variable list
                str=codec->toUnicode(phrase);
                log(QString("TD...%1 %2").arg(pos,0,16).arg(ind));
                main_err->dump((uint8_t *)phrase.data(),3*16);
                log(str);
            }
        }
    }
    else {
        log("Data is not a program file");
    }


    //    char cstr[20];
//    in.readRawData(cstr,20);
//    str=QString(cstr);
//    qDebug()<<str;

//    a1=getData();
//    qDebug()<<codec->toUnicode(a1);
    setProg(codec->toUnicode(a1));

    return;
}


QByteArray Program::fileOut() {
    QByteArray out;
    out.clear();
    out.append("\x7c\x61\x8a\x62\xfe\xff\xff\xff\x00\x00\x00\x00\x08\x00\x00\x00");
    out.append("\x05\xff\x7f\x00\x00\x00\x00\x00\x08\x00\x00\x00\x05\xff\x3f\x02");
    out.append(data); //temp should be string text
    qDebug()<<out;
    return out;

}

Program::~Program() {
     qDebug()<<"~Program";
}

//Notes
Notes::Notes(QString name_in, hp_DataType type_in, QString data_in):
    AbstractData(name_in, type_in) {

    setFileCode(HP_TP_NOTE);
    setNote(data_in);
}

QString Notes::getNote() {

    return text;
}

void Notes::parseData() {

//    quint16 len1,len2;
    int formatstart;
    QTextCodec * codec = QTextCodec::codecForName("UTF-16LE");

    QByteArray a1,a3;

    a1=getData();

    formatstart=a1.indexOf("C\x00S\x00W\x0D\x001\x001\x00");
    a3=a1.mid(formatstart,-1);
    a1=a1.mid(0,formatstart);

    text = codec->toUnicode(a1);
    format = codec->toUnicode(a3);

}

void Notes::setNote(QString data_in) {
    text=data_in;
}

//Variables
Variables::Variables(QString name_in, hp_DataType type_in):
    AbstractData(name_in, type_in) {

    setFileCode(HP_TP_SETTINGS);

}

void Variables::parseData() {

//    quint16 len1,len2;

    QByteArray a1,a3;

    a1=getData();

}

//CAS Variables
CASVariables::CASVariables(QString name_in, hp_DataType type_in):
    AbstractData(name_in, type_in) {

    setFileCode(HP_TP_SETTINGS);

}

void CASVariables::parseData() {

//    quint16 len1,len2;

    QByteArray a1,a3;

    a1=getData();

}

//Settings
Settings::Settings(QString name_in, hp_DataType type_in):
    AbstractData(name_in, type_in) {

    setFileCode(HP_TP_SETTINGS);

}

void Settings::parseData() {

//    quint16 len1,len2;

    QByteArray a1,a3;

    a1=getData();

}

void Settings::setData(QByteArray datain) {

        qDebug()<<"Settings Compare:";

        int len1=data.size();
        int len2=datain.size();
        log(QString("Settings Compare %1 %2").arg(len1).arg(len2));
        int i=0;
        while ((i<len1)&&(i<len2)) {
            if (data[i]!=datain[i]) {
                log(QString("Settings diff at:%1").arg(i));
            }
            i++;
        }

        data = datain;
        len1=data.size();
        len2=datain.size();
        log(QString("Settings Compare 2 %1 %2").arg(len1).arg(len2));

        //parseData();
}
