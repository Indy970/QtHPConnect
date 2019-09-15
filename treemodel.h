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
     QStandardItem *rootNode=nullptr;
     QMap<QString,hpDataLink> hpCalcList;
     hpCalcData * hpdata=nullptr;

public:
    treeModel(QObject *parent);
    virtual ~treeModel() override;
    int addCalculator(QString name, hpusb * handle);
    hpCalcData * getCalculator(QString name);
    hpCalcData * getHpCalcData(QString name);
    void setHpCalcData(QString name, hpCalcData * , hpTreeItem *);
    QString getLastDataKey();
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
    Qt::DropActions supportedDropActions() const override;
    QMimeData* mimeData(const QModelIndexList &) const override;
    Qt::ItemFlags flags(const QModelIndex&) const override;

};

#endif // TREEMODEL_H

