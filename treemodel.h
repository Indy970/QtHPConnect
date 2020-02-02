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

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QStandardItemModel>
#include <QObject>
#include <QMap>
#include <QMdiArea>
#include "hp_typedef.h"
#include "hpdata.h"

class hpTreeItem;

struct hpDataLink {
    hpTreeItem * treeItem;
    hpCalcData * dataItem;
};

class treeModel: public QStandardItemModel
{

    Q_OBJECT

private:
    int createRoot();
     QStandardItem *rootNode=nullptr;
     QMap<QString,hpDataLink> hpCalcList;
     hpCalcData * hpdata=nullptr;

public:
    treeModel(QObject *parent);
    virtual ~treeModel() override;
    int addCalculator(QString name, hpusb * handle);
    int deletCalculator(QString name, hpusb * handle);
    int deleteAllCalculators();
    void clickAction(QMdiArea * mdiwin,QModelIndex &index);
    void openFile(QMdiArea * mdiwin,QModelIndex &index);
    void deleteFile(QModelIndex &index);
    void renameFile(QModelIndex &index,QString newName);
    hpCalcData * getCalculator(QString name);
    hpTreeItem * getCalculatorItem(QString name);
    hpCalcData * getHpCalcData(QString name) const;
    AbstractData * createData(hp_Data data_in);
    AbstractData * getData(QModelIndex) const;
    QString getName(QModelIndex) const;
    int addItem(QString calc, AbstractData * obj);
    int deleteItem(hpCalcData* hpcalc, AbstractData * obj);
    void setHpCalcData(QString name, hpCalcData * , hpTreeItem *);
    QString getLastDataKey();
    hpTreeItem * findTypeRoot(QString CalcName, hp_DataType type);
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex &index) override;
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
    Qt::DropActions supportedDropActions() const override;
    QMimeData* mimeData(const QModelIndexList &) const override;
    Qt::ItemFlags flags(const QModelIndex&) const override;

};

#endif // TREEMODEL_H

