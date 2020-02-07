#include <hptoolbox.h>
#include "ui_hptoolbox.h"

hpToolBox::hpToolBox(QWidget *parent) :
    QToolBox(parent),
    ui(new Ui::hpToolBox)
{
    ui->setupUi(this);
}

hpToolBox::~hpToolBox()
{
    delete ui;
}
