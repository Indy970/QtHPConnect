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


#include "hp_mditexteditor.h"
#include "hp_mdiwindow.h"
#include "hptreeitem.h"
#include "hpdata.h"
#include "abstractdata.h"

hp_mdiTextEdit::hp_mdiTextEdit(QWidget * parent,hpTreeItem * treeItem, AbstractData * calcData)
    :hp_MdiWindow(parent)
{
    setMinimumSize(200,200);
    setMaximumSize(1000,1000);
    setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

    qDebug()<<"hp_mdiTextEdit::hp_mdiTextEdit";
    hptreeitem=treeItem;
    data = calcData;
    qDebug()<<data->getName();
    qDebug()<<data->getType();
    filename=treeItem->getFileName();
    qDebug()<<filename;

    type=treeItem->getType();

    setup();
    setWindowTitle(filename);
}

hp_mdiTextEdit::hp_mdiTextEdit(QWidget * parent,hp_DataStruct filedata, AbstractData * calcData)
    :hp_MdiWindow (parent)
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

    return textEdit->save();
}

bool hp_mdiTextEdit::saveAs(){

    return textEdit->saveAs();
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
