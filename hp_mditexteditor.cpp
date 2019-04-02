#include "hp_mditexteditor.h"
#include "hptreeitem.h"
#include "hpdata.h"
#include "abstractdata.h"

hp_mdiTextEdit::hp_mdiTextEdit(QWidget * parent,hpTreeItem * treeItem, AbstractData * calcData)
    :QMdiSubWindow(parent)
{
    setMinimumSize(200,200);
    setMaximumSize(1000,1000);
    setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

    hptreeitem=treeItem;
    data = calcData;
    filename=treeItem->getFileName();
    type=treeItem->getType();

    setup();
    setWindowTitle(filename);
}

hp_mdiTextEdit::hp_mdiTextEdit(QWidget * parent,hp_DataStruct filedata, AbstractData * calcData)
    :QMdiSubWindow(parent)
{
    setMinimumSize(200,200);
    setMaximumSize(1000,1000);
    setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

    hptreeitem=nullptr;
    data = calcData;
    filename=filedata.filename;
    type=filedata.type;
    setup();
    setWindowTitle(filename);
}

void hp_mdiTextEdit::setup() {

    QString text;
    textEdit = new textEditor(this);

    if (data) {

        qDebug()<<data->getName();
        qDebug()<<data->getType();

        if (data->getType()==HP_NOTE) {
            Notes *note;
            note=(Notes *)data;
            text=note->getNote();
        }

        if (data->getType()==HP_PROG) {
            Program *note;
            note=(Program *)data;
            text=note->getProg();
        }
        qDebug()<<text;

        textEdit->setPlainText(text);
    }
    else {
        qDebug()<<"hp_mdiTextEdit::setup - Data Null";
    }
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
