#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QStandardItemModel>
#include <QObject>
#include <QMap>

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
     QStandardItem *rootNode;
     QMap<QString,hpDataLink> hpCalcList;
     hpCalcData * hpdata;

public:
    treeModel(QObject *parent);
    ~treeModel();
    int addCalculator(QString name, hpusb * handle);
    hpCalcData * getCalculator(QString name);
    hpCalcData * getHpCalcData(QString name);
    void setHpCalcData(QString name, hpCalcData * , hpTreeItem *);
    QString getLastDataKey();

};

#endif // TREEMODEL_H

