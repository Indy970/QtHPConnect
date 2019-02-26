#include <QDebug>

#include "global.h"
#include "abstractdata.h"
#include "hpusb.h"

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
}

QByteArray AbstractData::getData() {
    return data;
}

//REAL
//
Application::Application(QString name_in, hp_DataType type_in):
    AbstractData(name_in, type_in) {

    setFileCode(HP_TP_FUNCTIONS);
}



//REAL
//
Real::Real(QString name_in, hp_DataType type_in):
    AbstractData(name_in, type_in) {

    setFileCode(HP_TP_SETTINGS);
}


//COMPLEX
Complex::Complex(QString name_in, hp_DataType type_in):
    AbstractData(name_in, type_in) {

    setFileCode(HP_TP_SETTINGS);
}


//LIST
List::List(QString name_in, hp_DataType type_in):
    AbstractData(name_in, type_in) {

    setFileCode(HP_TP_LIST);
}

//Matrix

Matrix::Matrix(QString name_in, hp_DataType type_in):
    AbstractData(name_in, type_in) {

    setFileCode(HP_TP_MATRIX);
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

//Notes
Notes::Notes(QString name_in, hp_DataType type_in, QString data):
    AbstractData(name_in, type_in) {

    setFileCode(HP_TP_NOTE);
    setNote(data);
    qDebug()<<"Taking Note";
    qDebug()<<data;
}

QString Notes::getNote() {

    qDebug()<<"Getting Note";
    qDebug()<<text;
    return text;
}

void Notes::setNote(QString data_in) {
    text=data_in;
}
