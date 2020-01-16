//
// Model to contain the calculator data structure
//

#include "treemodel.h"
#include "hptreeitem.h"
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

//Part the the drag and drop system
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
bool treeModel::dropMimeData(const QMimeData* md_data, Qt::DropAction action, int row,
            int column, const QModelIndex &index)
{
    QByteArray data_in;

    qDebug()<<"treemodel::DropMineData "<<row<<" "<<column;
    if (action == Qt::IgnoreAction) {
        qDebug()<<"treemodel::IgnoreAction";
        return true;
    }

    if (column > 1) {
        qDebug()<<"treemodel::column>1";
        return false;
    }

    int position;

    if (row != -1) {
        position = row;
    } else if (index.isValid()) {
        position = index.row();
    } else {
        position = rowCount(QModelIndex());
    }

    hpTreeItem * item=nullptr;
    item = static_cast<hpTreeItem *>(itemFromIndex(index));
    if (item!=nullptr) {
        qDebug()<<item->getType();
    }
    //item = getItem(parent);


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

    data_in=md_data->data("application/x-qstandarditemmodeldatalist");
    qDebug()<<position;
    //qDebug()<<data_in;
    qDebug()<<"treemodel::dropMimeData End";
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

    if (rootNode!=nullptr)
            delete rootNode;
    qDebug()<<"treeModel:: delete";
}


