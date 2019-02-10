#include "hp_infodialog.h"
#include "ui_hp_infodialog.h"

hp_infoDialog::hp_infoDialog(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::hp_infoDialog)
{
    ui->setupUi(this);
}

hp_infoDialog::~hp_infoDialog()
{
    delete ui;
}
