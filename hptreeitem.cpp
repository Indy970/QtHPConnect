#include "hptreeitem.h"
#include "global.h"
#include "hpdata.h"
#include "hp_mditexteditor.h"
#include "hp_infodialog.h"
#include "hp_settingsdlg.h"

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

const hp_DataType hpTreeItem::func_type[FUNC_NUM]={HP_APP,
                                           HP_CAS,
                                           HP_COMPLEX,
                                           HP_LIST,
                                           HP_MATRIX,
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
    }
}

void hpTreeItem::clickAction(QMdiArea * mdiwin) {

    hp_infoDialog * hpinfodlg;
    hp_Information hpinfo;
 //  QString test;
 //  test=data(Qt::DisplayRole).toString();

    switch (getType()) {
        case HP_MAIN:   {
                hpCalcData * dataStore;
                dataStore = getDataStore();
                hpinfo=dataStore->getInfo();
                hpinfodlg = new hp_infoDialog(mdiwin,hpinfo);
                //hpinfodlg->move(700,400);
                hpinfodlg->show();
            }
            break;
        case HP_APP: {

             }
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
        case HP_MATRIX:
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

void hpTreeItem::contextAction(QMdiArea * mdiwin, contextActionType cta) {

    hp_SettingsDlg * hpsettingsdlg;
    hp_Information hpinfo;
    hp_Settings hpset;
    int ret=0;
    switch (getType()) {
        case HP_MAIN: {
            switch (cta) {
                case CT_PREFERENCE: {
                    hpCalcData * dataStore;
                    dataStore = getDataStore();
                    dataStore->readSettings();
                    hpset=dataStore->getSettings();
                    hpsettingsdlg = new hp_SettingsDlg(mdiwin, &hpset);
                    ret=hpsettingsdlg->exec();
                    if (ret)
                        dataStore->setSettings(hpset);
                    }
                break;
                default: ;
            }
        }
        break;
        default: ;
    }
}

hp_DataType hpTreeItem::getType() {
    return type;
}

void hpTreeItem::setType(hp_DataType dtype) {
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
       case HP_MATRIX: return 4;
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

//Slot to respond to data changes
void hpTreeItem::dataChange(hp_Change hpchange) {

    hpCalcData * ptr=nullptr;
    QString name;
    switch (hpchange.dataChange) {
            case HP_MAIN:
                        ptr=getDataStore();
                        if (ptr) {
                            name=ptr->getName();
                            setData(name,Qt::DisplayRole);
                        }
                break;
            case HP_SCREEN: {
                    emit dataChanged(hpchange);
                }
            break;
            case HP_MATRIX:
            case HP_LIST: {
                refresh();
            }
            break;
    }
}

void hpTreeItem::addChild(AbstractData *obj) {
    hpTreeItem * subItem;
    hp_DataType type;

    qDebug()<<"Adding Child";

    if (obj) {
        QString name;
        //create fixed variable list

        type= obj->getType();
        name=obj->getName();

        //check if this is the same type
        if (getType()==type)
        {
            subItem= new hpTreeItem(name,getDataStore(),1);
            subItem->setType(type);

            switch (type) {
                case HP_MATRIX: {
                    subItem->setIcon(QIcon(func_list[HP_MATRIX][1]));
                }
                break;
                case HP_LIST: {
                    subItem->setIcon(QIcon(func_list[HP_LIST][1]));
                }
                break;
            }
        }
        appendRow(subItem);
    }
}

//Up date the data
void hpTreeItem::refresh() {
    int rows,i,j;
    int flag=0; //indicate 1 if data matches tree
    int datalen;
    hpTreeItem * ti_child;

    QString name;
    hp_DataType type;

    rows=rowCount();
    hpCalcData * calc;

    calc=getDataStore();
    if (calc) {

        AbstractData * obj;
        datalen=calc->dataCount();
        //get object at from calc
        //get number of list items

        //for all data items in list
        for (j=0; j<datalen; j++ ) {

            for (i=0; i<rows; i++) {
                ti_child=(hpTreeItem *)child(i);
                name=ti_child->getName();
                type=ti_child->getType();

            //compare with data
                obj= calc->dataAt(j);

                if(obj->equivalent(name,type)) {
                    flag =1; //obj found
                }

            }

            //If data and no row add row
            if (flag==0) {
                addChild(obj);
            }
        }

        //delete excess
        rows=rowCount();
        flag =0;
        //for all rows
        for (i=0; i<rows; i++) {
            for (j=0; j<datalen; j++ ) {
                ti_child=(hpTreeItem *)child(i);
                name=ti_child->getName();
                type=ti_child->getType();

            //compare with data
                obj= calc->dataAt(j);
                if(obj->equivalent(name,type)) {
                    flag =1; //obj found
                }
            }
            //If data and no row add row
            if (flag==0) {
                removeRow(i);
            }

        }

    }
}
