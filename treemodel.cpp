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

//Create Item
AbstractData * treeModel::createData(hp_Data data_in) {

    AbstractData * obj=nullptr;
    log("TreeModel::Creating Data Stucture");

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
    AbstractData * absitem;

    qDebug()<<"treemodel::DropMineData "<<row<<" "<<column;
    if (action == Qt::IgnoreAction) {
        qDebug()<<"treemodel::IgnoreAction";
        return true;
    }

    hpTreeItem * item=nullptr;
    item = static_cast<hpTreeItem *>(itemFromIndex(index));
    if (item!=nullptr) {
        
        qDebug()<<md_data->formats();

        QString name;

        name=md_data->text();
        data_in=md_data->data("application/x-programme");

        QDataStream in(&data_in,QIODevice::ReadOnly);
        absitem = new Program(name, HP_PROG, QStringLiteral(""));
        absitem->parseData(in);

        QString calc = item->getCalculatorName();
        addItem(calc,absitem);

        qDebug()<<"treemodel::dropMimeData End";

    }
    return true;
}

//Find the hpTreeItem of a type in a calculator
hpTreeItem * treeModel::findTypeRoot(QString calcName, hp_DataType type) {

    qDebug()<<calcName;
    hpTreeItem * calc=getCalculatorItem(calcName);
    hpTreeItem *item;

    qDebug()<<calc->getGroupName();

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

treeModel::~treeModel() {

    if (rootNode!=nullptr)
            delete rootNode;

    qDebug()<<"treeModel:: delete";
}


