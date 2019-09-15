#include "hp_infodialog.h"
#include "ui_hp_infodialog.h"
#include <QDebug>

hp_infoDialog::hp_infoDialog(QWidget *parent, hp_Information info) :
    QDialog(parent),
    ui(new Ui::hp_infoDialog)
{
    ui->setupUi(this);
    ui->serialnum->setText(info.serialnum);
    ui->appver->setText(info.appver);
    ui->oppver->setText(info.osver);
}

hp_infoDialog::~hp_infoDialog()
{
 //  qDebug()<<"Entering ~hp_infoDialog()";
    delete ui;
}
