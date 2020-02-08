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

#ifndef HP_MDITEXTEDITOR_H
#define HP_MDITEXTEDITOR_H

#include <QObject>
#include <QWidget>
#include <QMdiSubWindow>
#include <QTreeView>
#include "hpdata.h"
#include "hp_mdiwindow.h"

class hpTreeItem;
class hpCalcData;

#include "texteditor.h"

class hp_mdiTextEdit: public hp_MdiWindow
{
    Q_OBJECT

protected:

    textEditor * textEdit  =nullptr;
    hpTreeItem * hptreeitem =nullptr;
//    hpCalcData * hpcalc =nullptr;
    QString filename;
    QFileInfo file;
    hp_DataType type;
    AbstractData * data =nullptr;
    QString calculator;
    bool content;
   void closeEvent(QCloseEvent *event) override;

public:
    hp_mdiTextEdit(QWidget * parent = nullptr, hpTreeItem * treeItem = nullptr,
                   AbstractData * calcData =nullptr);
    hp_mdiTextEdit(QWidget * parent, QFileInfo filename,
                   AbstractData * calcData);
    void setup();
    bool save() override;
    bool saveAs() override;
    bool maybeSave();
    void show();
    ~hp_mdiTextEdit() override;

signals:

public slots:

};

#endif // HP_MDITEXTEDITOR_H
