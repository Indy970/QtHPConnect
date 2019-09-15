#include "hp_mdivariableedit.h"
#include "hptreeitem.h"


hp_mdiVariableEdit::hp_mdiVariableEdit(QWidget *parent,
            hpTreeItem * treeItem,
            hpCalcData * dataStore)
    : QMdiSubWindow(parent)
{
    setMinimumSize(200,200);
    setMaximumSize(1000,1000);
    setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

    hptreeitem=treeItem;
    hpcalc = dataStore;
    filename=treeItem->getFileName();
    type=treeItem->getType();

    setup();

    setWindowTitle(filename);
}

void hp_mdiVariableEdit::setup() {

    if (hpcalc) {
        varmodel = new varTableModel(this,hpcalc,filename,type);
        tableView = new QTableView(this);
        tableView->setModel(varmodel);
        setWidget(tableView);
    }
}

void hp_mdiVariableEdit::show() {
    if(tableView)
        tableView->show();
    QMdiSubWindow::show();
}

hp_mdiVariableEdit::~hp_mdiVariableEdit() {

 //   qDebug()<<"Entering ~hpmdiVariableEdit()";

}
