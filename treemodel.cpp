#include "treemodel.h"
#include "hptreeitem.h"

treeModel::treeModel(QObject *parent)
    :QStandardItemModel(parent)
{
    setItemPrototype(new hpTreeItem());
    createRoot();
}

int treeModel::createRoot()
{
    rootNode = invisibleRootItem();
    return 0;
}

//Rework - name should be calc name
int treeModel::addCalculator(QString name, hpusb * handle){


    hpDataLink hplink;
    hpCalcData * hpData = new hpCalcData(handle);
    hpTreeItem * hpCalc = new hpTreeItem(name,hpData,0);
    hpCalc->setType(HP_MAIN);
    hpCalc->setIcon(QIcon(":/icons/monitor_32x32.png"));
    hpCalc->setToolTip(QString("Calculator contents"));
    QObject::connect(hpData, SIGNAL(dataChanged(hp_Change)),hpCalc, SLOT(dataChange(hp_Change)));
    setHpCalcData(name,hpData,hpCalc);
    rootNode->appendRow(hpCalc);
    hpData->readInfo();

}

hpCalcData * treeModel::getCalculator(QString name){

    hpDataLink hplink;
    hpCalcData * hpdata = nullptr;

    QMap<QString, hpDataLink>::const_iterator i = hpCalcList.find(name);

    if (i!=hpCalcList.end()) {
        hplink = i.value();
        hpdata= hplink.dataItem;
    }

    return hpdata;
}

QString treeModel::getLastDataKey() {
    return hpCalcList.lastKey();
}

//manage link between tree and data
//A map stores the treeItem, dataItem and in future perhaps the handle in a list
// retrievable by a string key
hpCalcData * treeModel::getHpCalcData(QString name) {

    hpDataLink hplink;
    hpCalcData * hpdata;

    QMap<QString, hpDataLink>::const_iterator i = hpCalcList.find(name);

    if (i!=hpCalcList.end()) {
        hplink = i.value();
        hpdata= hplink.dataItem;
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

treeModel::~treeModel() {

    delete rootNode;
}


