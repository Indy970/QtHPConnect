#include <QDebug>
#include <QTextCodec>
#include <QtMath>

#include "global.h"
#include "abstractdata.h"
#include "hpusb.h"
#include "matrixdata.h"

uint16_t crc16_block(const uint8_t * buffer, uint32_t len);

int BCD2I(quint16 num) {

        int ds,ret;
        if(num>0) {
            ds=((num&0xF0)>>4)*10;
            ds+=(num&0x0F);
            ret=ds;
        }
        else {
            ret=0;
        }

//        qDebug()<<QString("Num= %1 Ret= %2").arg(num,1,16).arg(ret);
    return ret;
}

long TwosComplement2Int(long rawValue)
{

    long ret;

    qDebug()<<QString("List 2C: %1").arg(rawValue&0x80000000,1,16);

    // If a positive value, return it
    if ((rawValue & 0x80000000) == 0)
    {
        ret = rawValue;
    }
    else {
       // Otherwise perform the 2's complement math on the value
      qDebug()<<QString("List 2C Negative Detected");

       ret = (long)(~(rawValue - 0x01)) * -1;
//          ret = ~rawValue;
    }
    qDebug()<<QString("List 2C: %1 %2").arg(rawValue,1,16).arg(ret,1,16);

    return ret;
}

qint8 TwosComplement2Int(quint8 rawValue)
{

    qint8 ret;

    qDebug()<<QString("List 2C UINT8: %1").arg(rawValue&0x80,1,16);

    // If a positive value, return it
    if ((rawValue & 0x80) == 0)
    {
        ret = rawValue;
    }
    else {
       // Otherwise perform the 2's complement math on the value
      qDebug()<<QString("List 2C UINT8 Negative Detected");

       ret = ~(rawValue - 0x01) * -1;
//          ret = ~rawValue;
    }
    qDebug()<<QString("List 2C UINT8: %1 %2").arg(rawValue,1,16).arg(ret,1,16);

    return ret;
}


itemData extractNubmer(QByteArray item) {
;
    itemData listvalue;
    QString msg;
    QString value=QStringLiteral("");;
    qint8 sign;
    int multi;
    long exp;
    double base;
    double m,ret;
    int k;

        multi=item[0];
        base=BCD2I(item[2]);
        sign=TwosComplement2Int((quint8)item[3]);

        exp=(((((((((qint8)item[7]&0xFF)<<8)+((qint8)item[6]&0xFF))<<8)+((qint8)item[5]&0xFF)))<<8)+((qint8)item[4]&0xFF));
        exp=TwosComplement2Int(exp);

        m=0;

        for (k=8;k<16;k++) {
            if(item[k]>0) {
                m=m*0.01+(double)multi*BCD2I(item[k]);
            }
        }
        ret=sign*m*qPow(base,exp);

        value=QString("%1E%2").arg(m).arg(exp);

//        msg=QString("multi:%1 base:%2 sign:%3 exp:%4 m:%5").arg(multi).arg(base).arg(sign).arg(exp).arg(m);
        msg=QString("value: %1").arg(value);

        log(msg);
 //       log(value);
        log((QString("Ans: %1").arg(ret)));
       qDebug()<<msg;
       qDebug()<<ret;

       listvalue.dValue=ret;
       listvalue.sValue=QString("%1").arg(ret);

       return listvalue;
}


////////////////////////

AbstractData::AbstractData(QString name_in, hp_DataType type_in)
{

 //   qDebug()<<"base class called "<<name_in;
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

    if((name==name_in)&&(type==type_in))
        return true;
    else
        return false;
}

void AbstractData::setName(QString name_in) {
    name = name_in;
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
}

QByteArray AbstractData::getData() {
    return data;
}

void AbstractData::parseData() {
     qDebug()<<"AbstractData::parseData";
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

void Real::parseData() {

}

//COMPLEX
Complex::Complex(QString name_in, hp_DataType type_in):
    AbstractData(name_in, type_in) {

    setFileCode(HP_TP_SETTINGS);
}

void Complex::parseData() {

}

//LIST
List::List(QString name_in, hp_DataType type_in):
    AbstractData(name_in, type_in) {

    setFileCode(HP_TP_LIST);
    values.clear();
}

QByteArray List::getData() {

    //temp

    //parseData();

    return data;

}

void List::parseData() {

    QByteArray a1;
    QByteArray item;
    itemData listvalue;
    QString name;
    int len;
    int start = 8;
    int j;

    qDebug()<<"List: Parsing a List";

    name=getName();

    a1=data;
    len=a1[4];

    for(j=0;j<len;j++) {

        item=a1.mid(start,16);
        listvalue=extractNubmer(item);
        setListItem(-1,listvalue);
        start+=16;
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
    return QString("%1").arg(values.at(row).dValue);
}

//Passes a  string representation of the list item in the list at position row
void List::setItem(int row, QString string) {

    itemData item;
    item.dValue=string.toDouble();
    item.sValue=string;
    setListItem(row,item);
}

//Passes a  string representation of the list item in the list at position row
void List::setItem(int row, QString string, double value) {

    itemData item;
    item.dValue=value;
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

    setFileCode(HP_TP_MATRIX);
}

void Matrix::parseData() {

    QByteArray a1;
    QByteArray item;
    itemData listvalue;
    QString name;
    QString msg;
    QString value=QStringLiteral("");;
    int len;
    int start = 8;
    qint8 sign;
    int multi;
    long exp;
    double base;
    double m,ret;
    int k,j;
    QString ds;

    qDebug()<<"Matrix: Parsing a Matrix";

    name=getName();
    log(name);

    a1=data;
    len=a1[4];
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

    return item;
}

//Passes a  string representation of the item in the matrix at position position row,column
void Matrix::setItem(int row, int column, QString string) {

    itemData item;
    item.dValue=string.toDouble();
    item.sValue=string;
    setListItem(row,column, item);
}

//Passes a  string representation of the list item in the matrix at position row,column
void Matrix::setItem(int row, int column, QString string, double value) {

    itemData item;
    item.dValue=value;
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
    text=data_in;
}

void Program::parseData() {

    int16_t len1,len2;
    QTextCodec * codec = QTextCodec::codecForName("UTF-16LE");

    QByteArray a1;
    a1=getData();
    text = codec->toUnicode(a1);
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
    quint16 crc;
    QTextCodec * codec = QTextCodec::codecForName("UTF-16LE");

    QByteArray a1,a3;

    a1=getData();
//    crc=qChecksum(a1.mid(4,-1),a1.size()-4,Qt::ChecksumIso3309);
//    crc=crc16_block((uint8_t *)a1.mid(4,-1).constData(),a1.size()-4);

/*
    QDataStream ds(a1);
    ds.setByteOrder(QDataStream::LittleEndian);

    ds >> len1;
    ds >> len2;
*/

    formatstart=a1.indexOf("C\x00S\x00W\x0D\x001\x001\x00");
    a3=a1.mid(formatstart,-1);
    a1=a1.mid(0,formatstart);

    text = codec->toUnicode(a1);
    format = codec->toUnicode(a3);

}

void Notes::setNote(QString data_in) {
    text=data_in;
}
