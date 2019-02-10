#include "hp_mditexteditor.h"
#include "hptreeitem.h"

hp_mdiTextEdit::hp_mdiTextEdit(QWidget * parent,hpTreeItem * treeItem)
    :QMdiSubWindow(parent)
{
    setMinimumSize(200,200);
    setMaximumSize(1000,1000);
    setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    setup();
    hptreeitem=treeItem;
    setWindowTitle(treeItem->getName());
}

void hp_mdiTextEdit::setup() {

    textEdit = new textEditor(this);
    setWidget(textEdit);
}

void hp_mdiTextEdit::show() {

        if (textEdit)
            textEdit->show();
        QMdiSubWindow::show();
}

hp_mdiTextEdit::~hp_mdiTextEdit() {

    if (textEdit)
        delete textEdit;

}
