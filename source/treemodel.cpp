/*
 * QtHP Connect: hand-helds support interface.
 * Copyright (C) 2019 Ian Gebbie
 * Code patterns and snippets borrowed from libhpcalcs :
 * Copyright (C) 1999-2009 Romain Li�vin
 * Copyright (C) 2009-2013 Lionel Debroux
 * Copyright (C) 1999-2013 libti* contributors.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */



//
// Model to contain the calculator data structure
//

#include "treemodel.h"
#include "hptreeitem.h"
#include <hp_infodialog.h>
#include <hp_mdivariableedit.h>
#include <hp_mditexteditor.h>
#include <QStringListModel>
#include <QMimeData>


//Constructor
treeModel::treeModel(QObject *parent)
    :QStandardItemModel(parent)
{
    setItemPrototype(new hpTreeItem());
    createRoot();
    setParent(parent);    
 }

//Create the start of the tree
int treeModel::createRoot()
{
    rootNode = invisibleRootItem();
    return 0;
}

//Create Item
AbstractData * treeModel::createData(hp_Data data_in) {

    AbstractData * obj=nullptr;
    errlog("TreeModel::Creating Data Stucture");

    switch (data_in.type) {
        case HP_APP: {
            qDebug()<<"hpCalcData::recvData - Application";
            Application * obj = new Application(data_in.name,data_in.type);
            return obj;
        }
        break;
     case HP_LIST: {
            List * obj = new List(data_in.name,data_in.type);
            return obj;
        }
        break;
    case HP_MATRIX: {
            qDebug()<<"hpCalcData::recvData - Matrix";
            Matrix * obj = new Matrix(data_in.name,data_in.type);
            return obj;
        }
        break;
    }

    return obj;
}

//Rework - name should be calc name
int treeModel::addCalculator(QString name, hpusb * handle){

    hpCalcData * hpData = new hpCalcData(name, handle);
    hpTreeItem * hpCalc = new hpTreeItem(name,hpData,0);
    hpCalc->setType(HP_MAIN);
    hpCalc->setIcon(QIcon(":/icons/monitor_32x32.png"));
    hpCalc->setToolTip(QString("Calculator contents"));
    QObject::connect(hpData, SIGNAL(dataChanged(hp_Change)),hpCalc, SLOT(dataChange(hp_Change)));
    if (parent()!=nullptr)
        QObject::connect(hpData, SIGNAL(dataChanged(hp_Change)),parent(), SLOT(dataChange(hp_Change)));
    setHpCalcData(name,hpData,hpCalc);
    rootNode->appendRow(hpCalc);
    hpData->readInfo();

    return 0;
}

//return the calculator data within the model
hpCalcData * treeModel::getCalculator(QString name){

    hpDataLink hplink;
    hpCalcData * hpdata = nullptr;

    if (!hpCalcList.isEmpty()) {
        QMap<QString, hpDataLink>::const_iterator i = hpCalcList.find(name);

        if (i!=hpCalcList.end()) {
            hplink = i.value();
            hpdata= hplink.dataItem;
        }
    }
    return hpdata;
}

void treeModel::clickAction(QMdiArea * mdiwin,QModelIndex &index) {

    qDebug()<<"row "<<index.row()<<" column "<<index.column();
    return openFile(mdiwin,index);
}

void treeModel::openFile(QMdiArea * mdiwin, QModelIndex &index) {

    qDebug()<<"treeModel::openFile";
    hp_mdiTextEdit * hptextedit = nullptr;
    AbstractData * data=nullptr;
    hpTreeItem * item=nullptr;
    QString calc;
    QString name;

    hp_DataType type;
    hp_Information hpinfo;
    hp_infoDialog * hpinfodlg=nullptr;
    hpCalcData * hpdata=nullptr;
    hp_mdiVariableEdit * hpvaredit=nullptr;

    item = static_cast<hpTreeItem *>(itemFromIndex(index));
    data=getData(index);

    if ((data!=nullptr)&&(item!=nullptr)) {
        calc=item->getCalculatorName();
        name=item->getFileName();
        type=data->getType();
        hpdata=getHpCalcData(calc);
        switch (type) {
                case HP_MAIN:   {
                    hpinfo=hpdata->getInfo();
                    hpinfodlg = new hp_infoDialog(mdiwin,hpinfo);
                    hpinfodlg->show();
                }
                break;
                case HP_NOTE:
                case HP_PROG:   {
                    if (hptextedit==nullptr)
                        hptextedit = new hp_mdiTextEdit(mdiwin,item, data);
                    if (hptextedit!=nullptr)
                        hptextedit ->show();
                    }
                    break;
                case HP_CAS:
                case HP_REAL:
                case HP_COMPLEX:
                case HP_LIST:
                case HP_MATRIX: {
                    if (hpvaredit==nullptr) {
                        if (data!=nullptr) {
                            hpvaredit = new hp_mdiVariableEdit(mdiwin,item,hpdata);
                        }
                    }
                    if (hpvaredit!=nullptr)
                        hpvaredit ->show();
                    }
                    break;
        }
        }
        else {
            qDebug()<<"Null data";
        }

    return;
}

void treeModel::renameFile(QModelIndex &index,QString newName) {

    qDebug()<<"treeModel::renameFile";

    AbstractData * adata=nullptr;
    hpTreeItem * item=nullptr;
    QString calc;
    QString name;
    hp_DataType type;
    hpCalcData * hpdata=nullptr;

    qDebug()<<"row "<<index.row()<<" column "<<index.column();
    if (index.isValid()) {
        item = static_cast<hpTreeItem *>(itemFromIndex(index));

        if (item!=nullptr) {

            calc=item->getCalculatorName();
            name=item->getFileName();
            qDebug()<<name;
            type=item->getType();
            hpdata=getHpCalcData(calc);
            if (hpdata!=nullptr) {
                adata=hpdata->getData(name,type);
            }
            else {
                qDebug()<<"treeModel::renameFile hpdata is null";
            }

            //
            item->setFileName(newName);
            adata->setName(newName);

            //
        }
        else {
            qDebug()<<"Null data";
        }
    }
    else {
         qDebug()<<"treeModel::renameFile invalid index";
    }
}

void treeModel::deleteFile( QModelIndex &index) {

    qDebug()<<"treeModel::deleteFile";
    AbstractData * adata=nullptr;
    hpTreeItem * item=nullptr;
    QString calc;
    QString name;
    hp_DataType type;
    hpCalcData * hpdata=nullptr;

    if (index.isValid()) {
        item = static_cast<hpTreeItem *>(itemFromIndex(index));

        if (item!=nullptr) {

            calc=item->getCalculatorName();
            name=item->getFileName();
            qDebug()<<name;
            type=item->getType();
            hpdata=getHpCalcData(calc);
            if (hpdata!=nullptr) {
                adata=hpdata->getData(name,type);
            }
            else {
                qDebug()<<"treeMoel::deletFile hpdata is null";
            }

            //
            //delete hpTreeItem
  //          qDebug()<<"row "<<index.row()<<" column "<<index.column();
            if (removeRow(index.row(),index.parent()))
            {
                //qDebug()<<"Deleted";
            }
            else {
                qDebug()<<"Removal failed";
            }

            //delete Data
            hpdata->deleteData(adata);

        }
        else {
            qDebug()<<"Null data";
        }
    }
    else {
         qDebug()<<"treeMoel::deleteFile invalid index";
    }
}

//return the calculator data within the model
hpTreeItem * treeModel::getCalculatorItem(QString name){

    hpDataLink hplink;
    hpTreeItem * hpitem = nullptr;

    if (!hpCalcList.isEmpty()) {
        QMap<QString, hpDataLink>::const_iterator i = hpCalcList.find(name);

        if (i!=hpCalcList.end()) {
            hplink = i.value();
            hpitem= hplink.treeItem;
        }
    }
    return hpitem;
}

//index system for data retrieval
//review QStandardItemModel should already have one in place
QString treeModel::getLastDataKey() {
    if (hpCalcList.isEmpty())
        return QStringLiteral("");
    else
        return hpCalcList.lastKey();
}

//manage link between tree and data
//A map stores the treeItem, dataItem and in future perhaps the handle in a list
// retrievable by a string key
hpCalcData * treeModel::getHpCalcData(QString name) const {

    hpDataLink hplink;
    hpCalcData * hpdata=nullptr;

    if (!hpCalcList.isEmpty()) {
        QMap<QString, hpDataLink>::const_iterator i = hpCalcList.find(name);

        if (i!=hpCalcList.end()) {
            hplink = i.value();
            hpdata= hplink.dataItem;
        }
    }
    return hpdata;
}

//manage link between tree and data
void treeModel::setHpCalcData(QString name, hpCalcData * data, hpTreeItem * tree ){

    hpDataLink hplink;
    hplink.dataItem=data;
    hplink.treeItem=tree;
    hpCalcList.insert(name,hplink);

    return;
}

//Part the the drag and drop system
Qt::DropActions treeModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction | Qt::TargetMoveAction;
}

QString treeModel::getName(QModelIndex index) const {

    QString calc;
    QString name=QStringLiteral("None");
    hpTreeItem * item=nullptr;
    if (index.isValid()) {
        item = static_cast<hpTreeItem *>(itemFromIndex(index));
        if (item!=nullptr) {
            calc=item->getCalculatorName();
            name=item->getFileName();
        }
    }
    return name;
}



//Return the data object belonging to an item
AbstractData * treeModel::getData(QModelIndex index) const {

    AbstractData * adata=nullptr;
    QString calc;
    QString name;
    hp_DataType type;
    hpTreeItem * item=nullptr;
    hpCalcData * hpdata=nullptr;
    if (index.isValid()) {
        item = static_cast<hpTreeItem *>(itemFromIndex(index));
        if (item!=nullptr) {
            calc=item->getCalculatorName();
            name=item->getFileName();
            qDebug()<<name;
            type=item->getType();
            hpdata=getHpCalcData(calc);
            if (hpdata!=nullptr) {
                adata=hpdata->getData(name,type);
            }
            else {
                qDebug()<<"treeMoel::getData hpdata is null";
            }
        }
        else {
            qDebug()<<"treeMoel::getData item is null";
        }
    }
    else {
        qDebug()<<"treeMoel::getData invalid index";
    }
    return adata;
}


//Get and pass on the data to be dragged
QMimeData* treeModel::mimeData(const QModelIndexList &indexes) const
{

    QMimeData *mimeDataPtr = new QMimeData();
    AbstractData * adata = nullptr;
    QByteArray mydata;
    QByteArray datatype;
    QModelIndex index;

    qDebug()<<"treeModel::mimeData";

    for(int i =0; i<indexes.count(); i++)
    {
        index = indexes.at(i);
        adata = getData(index);
        if (adata!=nullptr) {
            qDebug()<<"Data "<<adata->getType();
            mydata = adata->getData();
            mimeDataPtr->setText(adata->getName());
            datatype[0] = adata->getType();
            mimeDataPtr->setData("application/x-type",datatype);
            mimeDataPtr->setData("application/x-qabstractmodeldatalist",mydata);
        }
        else {
             qDebug()<<"treeModel::mimeData No Data";
        }

    }

    return mimeDataPtr;
}

//Allow drop in location
bool treeModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    return true;
}

//Process the drop action
bool treeModel::dropMimeData(const QMimeData* md_data, Qt::DropAction action, int row,
            int column, const QModelIndex &index)
{
    QByteArray data_in;
    AbstractData * absitem;

    qDebug()<<"treemodel::DropMineData "<<row<<" "<<column;
    if (action == Qt::IgnoreAction) {
        qDebug()<<"treemodel::IgnoreAction";
        return true;
    }

    hpTreeItem * item=nullptr;
    item = static_cast<hpTreeItem *>(itemFromIndex(index));
    if (item!=nullptr) {
        hp_DataType type=HP_MAIN;
        QStringList formatList;
        QString mimeType;
        formatList=md_data->formats();

        foreach(const QString& format, formatList) {
            qDebug()<<format;
            for(int i = HP_MAIN; i < HP_SETTINGS; i++) {
                mimeType=mimetypes[i][1];
                qDebug()<<mimeType;
                if( mimeType==format) {
                    type=static_cast<hp_DataType>(i);
                    break;
                }
            }
        }

        if ( type!=HP_MAIN) {
            QString name=md_data->text();
            data_in=md_data->data(mimetypes[type][1]);

            QDataStream in(&data_in,QIODevice::ReadOnly);

            qDebug()<<"Type="<<type;
            switch(type) {
                case HP_NOTE: {
                    absitem = new Notes(name, HP_NOTE,QStringLiteral(""));
                    absitem->parseData(in);
                    break;
                }
                case HP_LIST: {
                    absitem = new List(name, HP_LIST);
                    absitem->parseData(in);
                    break;
                }
                case HP_MATRIX: {
                    absitem = new Matrix(name, HP_MATRIX);
                    absitem->parseData(in);
                    break;
                }
                case HP_PROG: {
                    absitem = new Program(name, HP_PROG, QStringLiteral(""));
                    absitem->parseData(in);
                    break;
                }
                case HP_VAR: {
                    absitem = new Variables(name, HP_VAR);
                    absitem->parseData(in);
                break;
            }
        }

        QString calc = item->getCalculatorName();
        addItem(calc,absitem);

 //       qDebug()<<"treemodel::dropMimeData End";
        }
        else {
            qDebug()<<"treemodel::sropMimeData type not found "<<type;
            return false;
        }
    }
    return true;
}

//Find the hpTreeItem of a type in a calculator
hpTreeItem * treeModel::findTypeRoot(QString calcName, hp_DataType type) {

    qDebug()<<calcName;
    hpTreeItem * calc=getCalculatorItem(calcName);
    hpTreeItem *item;

//    qDebug()<<calc->getGroupName();

    QModelIndex in = calc->index();

    for (int e = 0; e<calc->rowCount(); e++) {
        QModelIndex si=index(e,0,in);
        item = static_cast<hpTreeItem *>(itemFromIndex(si));
        if(item->getType()==type) {
            return item;
        }
    }

    return nullptr;

}

//add an object to the correct place in the tree of a calaculator
int treeModel::addItem(QString calc, AbstractData * obj) {

    hp_DataType type;
    hpCalcData * hpdata;

    if (obj!=nullptr) {
        type = obj->getType();
    }
    else {
        return -1;
    }

    hpTreeItem * rootitem = findTypeRoot(calc,type);

    if(rootitem!=nullptr) {
        hpdata=rootitem->getDataStore();
    }
    else {
        return -1;
    }
    rootitem->addChild(obj);
    hpdata->addData(obj);

    return 0;
}


int treeModel::deleteItem(hpCalcData* hpcalc, AbstractData * obj) {

}


Qt::ItemFlags treeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QStandardItemModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

int deletCalculator(QString name, hpusb * handle) {

}

int treeModel::deleteAllCalculators() {

    hpDataLink hplink;
    hpCalcData * hpdata = nullptr;

    foreach(QString key, hpCalcList.keys()) {

        QMap<QString, hpDataLink>::const_iterator i = hpCalcList.find(key);
        hplink=i.value();
        delete(hplink.dataItem);
        hpCalcList.remove(key);
    }
    hpCalcList.clear();
    return 0;

}


treeModel::~treeModel() {

    deleteAllCalculators();

    qDebug()<<"treeModel:: delete";
}


