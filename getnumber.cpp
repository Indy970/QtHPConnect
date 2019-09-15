#include "getnumber.h"
#include "ui_getnumber.h"
#include <QDebug>

getNumber::getNumber(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::getNumber)
{
    ui->setupUi(this);
}

getNumber::~getNumber()
{
 //   qDebug()<<"Entering ~getNumber()";
    delete ui;
}
