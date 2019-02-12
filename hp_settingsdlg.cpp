#include "dlgsettings.h"
#include "ui_dlgsettings.h"

dlgSettings::dlgSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgSettings)
{
    ui->setupUi(this);
}

dlgSettings::~dlgSettings()
{
    delete ui;
}
