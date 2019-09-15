#include "variableview.h"
#include "ui_variableview.h"
#include <QDebug>

variableView::variableView(QWidget *)

{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

variableView::~variableView()
{
    qDebug()<<"delete variableView";
    delete ui;
}
