#include "getnumber.h"
#include "ui_getnumber.h"

getNumber::getNumber(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::getNumber)
{
    ui->setupUi(this);
}

getNumber::~getNumber()
{
    delete ui;
}
