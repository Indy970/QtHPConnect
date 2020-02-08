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

//Called by the calculator Window
hp_mdiTextEdit::hp_mdiTextEdit(QWidget * parent,hpTreeItem * treeItem, AbstractData * calcData)
    :hp_MdiWindow(parent)
{
    setMinimumSize(200,200);
    setMaximumSize(1000,1000);
    setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

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
    setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    calculator=QStringLiteral("Content: ");
    content=true;
    hptreeitem=nullptr;
    data = calcData;
    filename=filedata.fileName();
    file=filedata;
    type=calcData->getType();
    setup();
    setWindowTitle(calculator+filename);
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
    setWidget(textEdit);
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
