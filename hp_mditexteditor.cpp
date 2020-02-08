/*
 * This file is part of the QtHPConnect distribution (https://github.com/Indy970/QtHPConnect.git).
 * Copyright (c) 2020 Ian Gebbie.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 or later.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QMessageBox>
#include <hp_mditexteditor.h>
#include <hp_mdiwindow.h>
#include <hptreeitem.h>
#include <hpdata.h>
#include <abstractdata.h>
#include <QBoxLayout>
#include <QToolBar>
#include <QToolButton>

//Called by the calculator Window
hp_mdiTextEdit::hp_mdiTextEdit(QWidget * parent,hpTreeItem * treeItem, AbstractData * calcData)
    :hp_MdiWindow(parent)
{
    setMinimumSize(200,200);
    setMaximumSize(1000,1000);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    qDebug()<<"hp_mdiTextEdit::hp_mdiTextEdit 1";
    hptreeitem=treeItem;
    content=false;
    data = calcData;

    filename=treeItem->getFileName();
    calculator=treeItem->getCalculatorName();
    type=treeItem->getType();

    setup();
    setWindowTitle(calculator+": "+filename);
}

//Called by the content window
hp_mdiTextEdit::hp_mdiTextEdit(QWidget * parent, QFileInfo filedata, AbstractData * calcData)
    :hp_MdiWindow (parent)
{

      qDebug()<<"hp_mdiTextEdit::hp_mdiTextEdit 2";
    setMinimumSize(200,200);
    setMaximumSize(1000,1000);
  //  setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    calculator=QStringLiteral("Content: ");
    content=true;
    hptreeitem=nullptr;
    data = calcData;
    filename=filedata.fileName();
    file=filedata;
    type=calcData->getType();
    setup();
    setWindowTitle(calculator+filename);
    resize(450,350);
}

void hp_mdiTextEdit::setup() {

    QString text;
    textEdit = new textEditor(this);

    if (data) {

        qDebug()<<data->getName();
        qDebug()<<data->getType();

        if (data->getType()==HP_NOTE) {
            Notes *note;
            note=static_cast<Notes *>(data);
            text=note->getNote();
        }

        if (data->getType()==HP_PROG) {
            Program *prog;
            prog=static_cast<Program *>(data);
            text=prog->getProg();
            textEdit->setDocumentTitle(prog->getName());
        }
        qDebug()<<text;
        textEdit->setPlainText(text);

    }
    else {
        qDebug()<<"hp_mdiTextEdit::setup - Data Null";
    }

    QIcon save(":/icons/save_22x22.png");
    QAction * actionSave= new QAction(save,"Save",this);

    QWidget * top = new QWidget();
    QBoxLayout * layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    top->setLayout(layout);

    QToolBar * toolbar = new QToolBar("Save");
    toolbar->addAction(actionSave);

    QToolButton *saveButton=new QToolButton();
    QMenu * menu = new QMenu(saveButton);
    menu->addAction(actionSave);
    layout->setMenuBar(menu);
//      layout->addWidget(toolbar);
//      QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
//      sizePolicy1.setHorizontalStretch(0);
//      sizePolicy1.setVerticalStretch(0);
//      sizePolicy1.setHeightForWidth(textEdit->sizePolicy().hasHeightForWidth());
//      textEdit->setSizePolicy(sizePolicy1);
//      layout->setSizeConstraint(QLayout::SetNoConstraint);

    layout->addWidget(textEdit);
    setWidget(top);

    connect(actionSave,SIGNAL(triggered()),this,SLOT(eventSave()));

    return;
}

void hp_mdiTextEdit::eventSave(){
    save();
}

bool hp_mdiTextEdit::save(){

    if(content)
        return textEdit->save(file);
    else {
        return textEdit->save(calculator);
    }
}

bool hp_mdiTextEdit::saveAs(){

    if(content)
        return textEdit->saveAs(file);
    else {
        return textEdit->saveAs(calculator);
    }
}

void hp_mdiTextEdit::closeEvent(QCloseEvent *event)
{

    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }

}

bool hp_mdiTextEdit::maybeSave()
{
    if (!textEdit->document()->isModified())
        return true;

    const QMessageBox::StandardButton ret
            = QMessageBox::warning(this, tr("MDI"),
                                   tr("'%1' has been modified.\n"
                                      "Do you want to save your changes?")
                                   .arg(file.fileName()),
                                   QMessageBox::Save | QMessageBox::Discard
                                   | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return textEdit->save(file);
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void hp_mdiTextEdit::show() {

        if (textEdit)
            textEdit->show();
        QMdiSubWindow::show();
}

hp_mdiTextEdit::~hp_mdiTextEdit() {

    qDebug()<<"Entering ~hpmdiTextEdit()";
    if (textEdit!=nullptr)
        delete textEdit;

}
