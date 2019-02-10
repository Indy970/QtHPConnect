#include "hptreeitem.h"
#include "global.h"
#include "hpdata.h"
#include "hp_mditexteditor.h"
#include "hp_infodialog.h"

#include <QMessageBox>
#include <QString>

#define FUNC_NUM 9

const QString hpTreeItem::func_list[FUNC_NUM][2]={{"Application Library",":/icons/apps_32x32.png"},
                                           {"CAS Vars",":/icons/casFolder_32x32.png"},
                                           {"Complex",":/icons/complex_32x32.png"},
                                           {"Lists",":/icons/list_32x32.png"},
                                           {"Matrices",":/icons/table_32x32.png"},
                                           {"Notes",":/icons/note_32x32.png"},
                                           {"Programs",":/icons/program_32x32.png"},
                                           {"Real",":/icons/real_32x32.png"},
                                           {"Variables",":/icons/varFolder_32x32.png"}
                                           };

const DataType hpTreeItem::func_type[FUNC_NUM]={HP_APP,
                                           HP_CAS,
                                           HP_COMPLEX,
                                           HP_LIST,
                                           HP_MATRIC,
                                           HP_NOTE,
                                           HP_PROG,
                                           HP_REAL,
                                           HP_VAR
                                           };

//QString hpTreeItem::hp_name;


hpTreeItem::hpTreeItem()
    :QStandardItem() {

}

hpTreeItem::hpTreeItem(const QString & name,hpCalcData * hpDataStore,int flag)
:QStandardItem(name)
{
    setEditable(0);
    setDataStore(hpDataStore);

    if (flag==0)
        setGraphicTree();
}

QStandardItem * hpTreeItem::clone() const {
    //return new hpTreeItem(QStringLiteral("test"),getDataStore(),0);
}

void hpTreeItem::setGraphicTree() {

    int func_num=FUNC_NUM;
    hpTreeItem * newChild;
    hpTreeItem * subItem;

    int i,j;

    for (i=0; i<func_num; i++)
    {
        newChild= new hpTreeItem(func_list[i][0],getDataStore(),1);
        newChild->setType(func_type[i]);
        newChild->setIcon(QIcon(func_list[i][1]));
        newChild->setEditable(0);
        newChild->setToolTip(QString("Manage Applications"));
//        newChild->getDataStore()->getInfo().serialnum="12-3456";
        appendRow(newChild);
        switch (func_type[i]) {
        case HP_MATRIC: {
            QString name;
            //create fixed variable list
            for (j=0;j<10;j++) {
                name="M"+QString().sprintf("%d",j);
                subItem= new hpTreeItem(name,getDataStore(),1);
                subItem->setIcon(QIcon(func_list[i][1]));
                subItem->setType(func_type[i]);
                newChild->appendRow(subItem);
            }
            }
            break;
        case HP_LIST: {
            QString name;
            //create fixed variable list
            for (j=0;j<10;j++) {
                name="L"+QString().sprintf("%d",j);

                subItem= new hpTreeItem(name,getDataStore(),1);
                subItem->setIcon(QIcon(func_list[i][1]));
                subItem->setType(func_type[i]);
                newChild->appendRow(subItem);
            }
            }
            break;
        }
    }
}

void hpTreeItem::clickAction(QMdiArea * mdiwin) {

    hp_infoDialog * hpinfodlg;
    hp_Information hpinfo;
 //  QString test;
 //  test=data(Qt::DisplayRole).toString();

    switch (getType()) {
        case HP_APP:
                    hpCalcData * dataStore;
                    dataStore = getDataStore();
                    hpinfo=dataStore->getInfo();
                    hpinfodlg = new hp_infoDialog(hpinfo,0);
                    hpinfodlg->show();
             break;
        case HP_CAS:
                if (hpvaredit==nullptr)
                    hpvaredit = new hp_mdiVariableEdit(mdiwin,this);
                if (hpvaredit!=nullptr)
                        hpvaredit ->show();
        case HP_COMPLEX:
            if (hpvaredit==nullptr)
                hpvaredit = new hp_mdiVariableEdit(mdiwin,this);
            if (hpvaredit!=nullptr)
                hpvaredit ->show();
            break;
        case HP_LIST:
            if (hpvaredit==nullptr)
                hpvaredit = new hp_mdiVariableEdit(mdiwin,this);
            if (hpvaredit!=nullptr)
                    hpvaredit ->show();
            break;
        case HP_MATRIC:
            if (hpvaredit==nullptr)
                hpvaredit = new hp_mdiVariableEdit(mdiwin,this);
            if (hpvaredit!=nullptr)
                    hpvaredit ->show();
            break;
        case HP_NOTE:
            if (hptextedit==nullptr)
                hptextedit = new hp_mdiTextEdit(mdiwin,this);
            if (hptextedit!=nullptr)
                    hptextedit ->show();
            break;
        case HP_PROG:
            break;
        case HP_REAL:
            if (hpvaredit==nullptr)
                hpvaredit = new hp_mdiVariableEdit(mdiwin,this);
            if (hpvaredit!=nullptr)
                    hpvaredit ->show();
            break;
        case HP_VAR:

         break;
        default: ;
    }

   // log(test+getDataStore()->getInfo().serialnum);

}

DataType hpTreeItem::getType() {
    return type;
}

void hpTreeItem::setType(DataType dtype) {
    type=dtype;
    return;
}

//manage access to datastore
hpCalcData * hpTreeItem::getDataStore() {
    if (hp_data) {
        return hp_data;
    }
    else
        return nullptr;
}

void hpTreeItem::setDataStore(hpCalcData * dtype) {
    hp_data=dtype;
    return;
}

int hpTreeItem::dt2int() {
    switch (getType()) {
       case HP_APP:  return 0;
       case HP_CAS:  return 1;
       case HP_COMPLEX: return 2;
       case HP_LIST: return 3;
       case HP_MATRIC: return 4;
       case HP_NOTE: return 5;
       case HP_PROG: return 6;
       case HP_REAL: return 7;
       case HP_VAR: return 8;
    };
    return 0;
}

QString hpTreeItem::getName() {
    return func_list[dt2int()][0];
 }

void hpTreeItem::dataChange(hp_Change hpchange) {

    hpCalcData * ptr=nullptr;
    QString name;
    qDebug()<<"In data Change";
    switch (hpchange.dataChange) {
            case HP_MAIN:
                    qDebug()<<"test";
                        ptr=getDataStore();
                        qDebug()<<"past test";
                        if (ptr) {
                            name=ptr->getName();
                            qDebug()<<"get name";
                            setData(name,Qt::DisplayRole);
                        }
                break;
    }
}
