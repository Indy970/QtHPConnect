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

#include <QWidget>
#include <QMdiSubWindow>
#include <QTableView>
#include <QFileInfo>

#ifndef HP_MDIVARIABLEEDIT_H
#define HP_MDIVARIABLEEDIT_H

#include "vartablemodel.h"
#include "hpdata.h"
#include "hp_mdiwindow.h"

class hpTreeItem;

class hp_mdiVariableEdit : public hp_MdiWindow
{
    Q_OBJECT

protected:
        QTableView * tableView =nullptr;
        hpTreeItem * hptreeitem  =nullptr;
        varTableModel * varmodel  =nullptr;
        hpCalcData * hpcalc =nullptr;
        AbstractData * data = nullptr;
        QString filename;
        hp_DataType type=HP_MAIN;
        QFileInfo file;
        QString calculator;
        bool content;
        void setup();
        void closeEvent(QCloseEvent *event) override;


public:
    hp_mdiVariableEdit(QWidget *parent = nullptr,
                                hpTreeItem * treeItem = nullptr,
                                hpCalcData * dataStore =nullptr
                                );
    hp_mdiVariableEdit(QWidget *parent,
                                    QFileInfo file,
                                    AbstractData * data
                                    );
    void show();
    bool save() override;
    bool saveAs() override;
    bool saveFile(const QFileInfo fileinfo);
    bool maybeSave();
    ~hp_mdiVariableEdit() override;

signals:

public slots:
    void eventSave();
};

#endif // HP_MDIVARIABLEEDIT_H
