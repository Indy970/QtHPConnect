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

    qDebug()<<"Entering ~hpmdiVariableEdit()";

}
