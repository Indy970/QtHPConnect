#include "variableview.h"
#include "ui_variableview.h"

variableView::variableView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::variableView)
{
    ui->setupUi(this);
}

variableView::~variableView()
{
    delete ui;
}
