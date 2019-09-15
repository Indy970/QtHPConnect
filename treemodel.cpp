#include "treemodel.h"
#include "hptreeitem.h"
#include <QStringListModel>
#include <QMimeData>

treeModel::treeModel(QObject *parent)
    :QStandardItemModel(parent)
{
    setItemPrototype(new hpTreeItem());
    createRoot();
    setParent(parent);    
}

int treeModel::createRoot()
{
    rootNode = invisibleRootItem();
    return 0;
}

//Rework - name should be calc name
int treeModel::addCalculator(QString name, hpusb * handle){

    hpCalcData * hpData = new hpCalcData(handle);
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

QString treeModel::getLastDataKey() {
    if (hpCalcList.isEmpty())
        return QStringLiteral("");
    else
        return hpCalcList.lastKey();
}

//manage link between tree and data
//A map stores the treeItem, dataItem and in future perhaps the handle in a list
// retrievable by a string key
hpCalcData * treeModel::getHpCalcData(QString name) {

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

Qt::DropActions treeModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction | Qt::TargetMoveAction;
}

//Get and pass on the data to be dragged
QMimeData* treeModel::mimeData(const QModelIndexList &indexes) const
{

    QMimeData *mimeDataPtr = new QMimeData();
    QByteArray mydata;

    qDebug()<<"treeModel::mimeData";

    mimeDataPtr->setData("application/x-qabstractmodeldatalist",mydata);

    /* Store row id list */
    QList<int> rowIdList;
    int rowId;
    foreach (QModelIndex index, indexes) {
        if (index.isValid()) {
            rowId = index.row();

            if (!rowIdList.contains(rowId)) {
                rowIdList << rowId;
            }
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
bool treeModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row,
            int column, const QModelIndex &parent)
{
    qDebug()<<"treemodel::DropMineData";
    if (action == Qt::IgnoreAction) {
        return true;
    }

    if (column > 1) {
        return false;
    }

    int position;

    if (row != -1) {
        position = row;
    } else if (parent.isValid()) {
        position = parent.row();
    } else {
        position = rowCount(QModelIndex());
    }

//	QByteArray encodedData = data->data("application/text.list");
//	QDataStream stream(&encodedData, QIODevice::ReadOnly);

        /* Retrieve row id */
    QList<int> rowIdList;
//	while (!stream.atEnd()) {
//		QString text;
//		stream >> text;
//		rowIdList << text.toInt();
//	}

       /* Insert rows (one by one) */
//	foreach(int rowId, rowIdList) {
//		insertRow(position, parent, rowId);
//	}

    return true;
}

Qt::ItemFlags treeModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QStandardItemModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

treeModel::~treeModel() {

    qDebug()<<"treeModel:: delete";
}


