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

#ifndef HPTREEITEM_H
#define HPTREEITEM_H

#include <QStandardItem>
#include <QMenu>
#include <QAction>
#include <QString>
#include "hpdata.h"
#include "mainwindow.h"

class hp_mdiVariableEdit;
class hp_mdiTextEdit;

//enum hp_DataType;

enum contextActionType {
            CT_PREFERENCE
};

class hpTreeItem: public QObject, public QStandardItem
{
    Q_OBJECT

public:

    hpTreeItem();
    hpTreeItem(const QString &,hpCalcData *, int);
    virtual QStandardItem *clone() const;
    void setGraphicTree();
    void setType(hp_DataType);

    void clickAction(QMdiArea * mwin);
    void contextAction(QMdiArea * mdiwin, contextActionType cta );
    void setDataStore(hpCalcData *);
    hpCalcData *  getDataStore();
    QString getCalculatorName();
    QString getGroupName();
    QString getFileName();
    bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent);
    void setFileName(QString);
    hp_DataType getType();
    void refresh();
    int findFile(QString);
    void addFile(AbstractData *);
    void addChild(AbstractData * obj);
    virtual ~hpTreeItem();

public slots:
    void dataChange(hp_Change hpchange);

private:
    const static QString func_list[][2];
    const static hp_DataType func_type[];
  //  QString filename;
    hp_DataType type;
    hpCalcData * hp_data=nullptr;
    QMdiArea * mdiarea=nullptr;
    hp_mdiVariableEdit * hpvaredit = nullptr;
    hp_mdiTextEdit * hptextedit = nullptr;
    int dt2int();

signals:
    void dataChanged(hp_Change datachange);

};

Q_DECLARE_METATYPE(hpTreeItem *)

#endif // HPTREEITEM_H
