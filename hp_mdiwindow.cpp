#include "hp_mdiwindow.h"

hp_MdiWindow::hp_MdiWindow(QWidget * parent)
    :QMdiSubWindow(parent)
{
    setMinimumSize(400,400);
    setMaximumSize(1000,1000);
    setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    setup();
}

void hp_MdiWindow::setup() {

    textEdit = new QTextEdit(this);
    setWidget(textEdit);
}

QTextEdit * hp_MdiWindow::getEditor() {
    return textEdit;
}

void hp_MdiWindow::show() {
    textEdit->show();
    QMdiSubWindow::show();
}

hp_MdiWindow::~hp_MdiWindow() {
    if (textEdit)
        delete textEdit;
}
