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
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row,
                int column, const QModelIndex &parent);
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const;
    Qt::DropActions supportedDropActions() const;
    QMimeData* mimeData(const QModelIndexList &) const;
    Qt::ItemFlags flags(const QModelIndex&) const;

};

#endif // TREEMODEL_H

