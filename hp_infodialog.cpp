#include "hp_infodialog.h"
#include "ui_hp_infodialog.h"

hp_infoDialog::hp_infoDialog(hp_Information info, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::hp_infoDialog)
{
    ui->setupUi(this);
    ui->serialnum->setText(info.serialnum);
    ui->appver->setText(info.appver);
    ui->oppver->setText(info.osver);
}

hp_infoDialog::~hp_infoDialog()
{
    delete ui;
}
