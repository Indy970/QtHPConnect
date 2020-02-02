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

#ifndef HP_MDIVARIABLEEDIT_H
#define HP_MDIVARIABLEEDIT_H

#include <QObject>
#include <QWidget>
#include <QMdiSubWindow>
#include <QTableView>
#include "vartablemodel.h"
#include "hpdata.h"

class hpTreeItem;

class hp_mdiVariableEdit : public QMdiSubWindow
{
    Q_OBJECT

protected:
        QTableView * tableView =nullptr;
        hpTreeItem * hptreeitem  =nullptr;
        varTableModel * varmodel  =nullptr;
        hpCalcData * hpcalc =nullptr;
        QString filename;
        hp_DataType type;
        void setup();

public:
    explicit hp_mdiVariableEdit(QWidget *parent = nullptr,
                                hpTreeItem * treeItem = nullptr,
                                hpCalcData * dataStore =nullptr
                                );
//    explicit hp_mdiVariableEdit(QWidget *parent,
//                                    hp_DataStruct filedata,
//                                    hpCalcData * dataStore
//                                    );
    void show();
    ~hp_mdiVariableEdit();

signals:

public slots:

};

#endif // HP_MDIVARIABLEEDIT_H
