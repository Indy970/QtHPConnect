#include "variableview.h"
#include "ui_variableview.h"


variableView::variableView(QWidget *parent)

{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

}

variableView::~variableView()
{
    delete ui;
}
