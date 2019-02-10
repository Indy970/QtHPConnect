#include "hp_mdivariableedit.h"
#include "hptreeitem.h"


hp_mdiVariableEdit::hp_mdiVariableEdit(QWidget *parent, hpTreeItem * treeItem)
    : QMdiSubWindow(parent)
{
    setMinimumSize(200,200);
    setMaximumSize(1000,1000);
    setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    setup();
    hptreeitem=treeItem;
    setWindowTitle(treeItem->getName());
}

void hp_mdiVariableEdit::setup() {

    tableView = new QTableView(this);
    varmodel = new varTableModel(this);
    tableView->setModel(varmodel);
    setWidget(tableView);

}

void hp_mdiVariableEdit::show() {
    if(tableView)
        tableView->show();
    QMdiSubWindow::show();
}



hp_mdiVariableEdit::~hp_mdiVariableEdit() {

    if (tableView)
        delete tableView;
    if (varmodel)
        delete varmodel;
}
