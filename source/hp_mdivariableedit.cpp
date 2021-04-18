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

#include <QBoxLayout>
#include <QToolBar>
#include <QToolButton>
#include <QMessageBox>
#include "hp_mdivariableedit.h"
#include "hptreeitem.h"

hp_mdiVariableEdit::hp_mdiVariableEdit(QWidget *parent,
            hpTreeItem * treeItem,
            hpCalcData * dataStore)
    : hp_MdiWindow(parent)
{
    setMinimumSize(200,200);
    setMaximumSize(1000,1000);
    setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

    hptreeitem=treeItem;
    hpcalc = dataStore;
    filename = QStringLiteral("NULL");

    if (hpcalc!=nullptr) {
        calculator=hpcalc->getCalculatorName();

        if (treeItem!=nullptr) {
            filename=treeItem->getFileName();
            type=treeItem->getType();
        }
        else {
            qWarning()<<"hpcalc is null";
        }
        data=hpcalc->getData(filename,type);
    }
    else {
        qWarning()<<"hpcalc is null";
    }
    content=false;
    setup();

    setWindowTitle(calculator+": "+filename);
}

hp_mdiVariableEdit::hp_mdiVariableEdit(QWidget *parent,
            QFileInfo fileinfo,
            AbstractData * data_in)
    : hp_MdiWindow(parent)
{
    setMinimumSize(200,200);
    setMaximumSize(1000,1000);
    setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

    calculator=QStringLiteral("Content: ");
    content=true;
    hptreeitem=nullptr;
    hpcalc = nullptr;
    file=fileinfo;
    filename=fileinfo.fileName();

    data = data_in;

    if (data!=nullptr)
        type=data->getType();

    setup();

    setWindowTitle(calculator+filename);
}

void hp_mdiVariableEdit::setup() {

    if (data!=nullptr) {
        varmodel = new varTableModel(this,data,filename,type);
        tableView = new QTableView(this);
        tableView->setModel(varmodel);

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
        layout->addWidget(tableView);
        setWidget(top);

        connect(actionSave,SIGNAL(triggered()),this,SLOT(eventSave()));
    }
}

void hp_mdiVariableEdit::show() {
    if(tableView!=nullptr)
        tableView->show();
    else {
        qWarning()<<"hp_mdiVariableEdit::show tableView null";
    }
    hp_MdiWindow::show();
}

void hp_mdiVariableEdit::eventSave(){
    save();
}

bool hp_mdiVariableEdit::save(){

    if (content==true) {
        return saveFile(file);
    }



    return false;
}

bool hp_mdiVariableEdit::saveAs(){

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    file.absoluteFilePath());
    if (fileName.isEmpty())
        return false;

    if (content==true) {
        QFileInfo fileinfo(fileName);
        return saveFile(fileinfo);
    }

    return false;
}

bool hp_mdiVariableEdit::saveFile(const QFileInfo fileinfo)
{
    QFile file(fileinfo.absoluteFilePath());
    if (!file.open(QFile::ReadWrite)) {

        QMessageBox::warning(this, tr("MDI"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileinfo.canonicalPath(), file.errorString()));
        return false;
    }

    qDebug()<<"Write file";

    //Show busy cursor
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QDataStream ds(&file);
    ds.setByteOrder(QDataStream::LittleEndian);

    varmodel->getData(ds);

    //reset busy cursor
    QApplication::restoreOverrideCursor();
    varmodel->resetModified();

    file.close();
    return true;
}


bool hp_mdiVariableEdit::maybeSave()
{

    if (varmodel==nullptr)
        return false;

    if (!varmodel->isModified()) {
        qDebug()<<"Var not modified";
        return true;
    }

    if (content==false) {
        return true;
    }

    const QMessageBox::StandardButton ret
            = QMessageBox::warning(this, tr("MDI"),
                                   tr("'%1' has been modified.\n"
                                      "Do you want to save your changes?")
                                   .arg(file.fileName()),
                                   QMessageBox::Save | QMessageBox::Discard
                                   | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return saveFile(file);
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }

    return true;
}


void hp_mdiVariableEdit::closeEvent(QCloseEvent *event)
{

    qDebug()<<"hp_mdiVariableEdit::closeEvent";

    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }

    QMdiSubWindow::closeEvent(event);
}

hp_mdiVariableEdit::~hp_mdiVariableEdit() {

    qDebug()<<"Entering ~hpmdiVariableEdit()";

    if (varmodel!=nullptr)
        delete varmodel;
}
