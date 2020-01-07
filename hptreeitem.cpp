#include "hptreeitem.h"
#include "global.h"
#include "hpdata.h"
#include "hp_mditexteditor.h"
#include "hp_infodialog.h"
#include "hp_settingsdlg.h"

#include <QMessageBox>
#include <QString>

#define FUNC_NUM 10

const QString hpTreeItem::func_list[FUNC_NUM][2]={
                                           {"Main",":/icons/apps_32x32.png"},
                                           {"Application Library",":/icons/apps_32x32.png"},
                                           {"CAS Vars",":/icons/casFolder_32x32.png"},
                                           {"Complex",":/icons/complex_32x32.png"},
                                           {"Lists",":/icons/list_32x32.png"},
                                           {"Matrices",":/icons/table_32x32.png"},
                                           {"Notes",":/icons/note_32x32.png"},
                                           {"Programs",":/icons/program_32x32.png"},
                                           {"Real",":/icons/real_32x32.png"},
                                           {"Variables",":/icons/varFolder_32x32.png"}
                                           };

const hp_DataType hpTreeItem::func_type[FUNC_NUM]={
                                           HP_MAIN,
                                           HP_APP,
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

    setDragEnabled(true);
    setDropEnabled(true);

    filename=name;

    if (flag==0)
        setGraphicTree();
}

QStandardItem * hpTreeItem::clone() const {
    //return new hpTreeItem(QStringLiteral("test"),getDataStore(),0);
}

void hpTreeItem::setGraphicTree() {

    int func_num=FUNC_NUM;
    hpTreeItem * newChild;

    int i;

    for (i=1; i<func_num; i++)
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
    hpCalcData * calc;
    hp_DataStruct hpdata;
    calc=getDataStore();
    AbstractData * data=nullptr;

 //  QString test;
 //  test=data(Qt::DisplayRole).toString();

    switch (getType()) {
        case HP_MAIN:   {
                hpCalcData * dataStore;
                dataStore = getDataStore();
                hpinfo=dataStore->getInfo();
                hpinfodlg = new hp_infoDialog(mdiwin,hpinfo);
                hpinfodlg->show();
            }
            break;
        case HP_APP: {

             }
             break;
        case HP_CAS:
                if (hpvaredit==nullptr) {
                    if (calc) {
                        data=calc->getData(getFileName(),HP_COMPLEX);
                    }
                    if (data) {
                        hpvaredit = new hp_mdiVariableEdit(mdiwin,this,calc);
                    }
                }
                if (hpvaredit!=nullptr)
                        hpvaredit ->show();
        case HP_COMPLEX:
            if (hpvaredit==nullptr) {
                if (calc) {
                    data=calc->getData(getFileName(),HP_COMPLEX);
                }
                if (data) {
                    hpvaredit = new hp_mdiVariableEdit(mdiwin,this,calc);
                }
            }
            if (hpvaredit!=nullptr)
                hpvaredit ->show();
            break;
        case HP_LIST:
            if (hpvaredit==nullptr) {
                if (calc) {
                    data=calc->getData(getFileName(),HP_LIST);
                }
                if (data) {
                    hpvaredit = new hp_mdiVariableEdit(mdiwin,this,calc);
                }
            }
            if (hpvaredit!=nullptr)
                    hpvaredit ->show();
            break;
        case HP_MATRIX:
            if (hpvaredit==nullptr) {
                if (calc) {
                    data=calc->getData(getFileName(),HP_MATRIX);
                }
                if (data) {
                hpvaredit = new hp_mdiVariableEdit(mdiwin,this,calc);
                }
            }
            if (hpvaredit!=nullptr)
                    hpvaredit ->show();
            break;
        case HP_NOTE: {
             hpCalcData * dataStore;
             dataStore = getDataStore();

             AbstractData * data =nullptr;
             dataStore = getDataStore();
             if (dataStore) {
                 data=dataStore->getData(getFileName(),getType());
             }

            if (hptextedit==nullptr)
                hptextedit = new hp_mdiTextEdit(mdiwin,this, data);
            if (hptextedit!=nullptr)
                    hptextedit ->show();
            }
            break;
        case HP_PROG: {
            hpCalcData * dataStore;
            AbstractData * data =nullptr;
            dataStore = getDataStore();
            if (dataStore) {
                data=dataStore->getData(getFileName(),getType());
            }
            if (hptextedit==nullptr)
                hptextedit = new hp_mdiTextEdit(mdiwin,this, data);
            if (hptextedit!=nullptr)
                hptextedit ->show();
            }
            break;
        case HP_REAL:           
            if (hpvaredit==nullptr) {
                if (calc) {
                    data=calc->getData(getFileName(),HP_REAL);
                }
                if (data) {
                     hpvaredit = new hp_mdiVariableEdit(mdiwin,this,calc);
                }
            }
            if (hpvaredit!=nullptr)
                    hpvaredit ->show();
            break;
        case HP_VAR:

         break;
        default: ;
    }
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
       default:;
    };
    return 0;
}

QString hpTreeItem::getGroupName() {
    return func_list[dt2int()][0];
 }

QString hpTreeItem::getFileName() {

    return filename;
 }

void hpTreeItem::setFileName(QString file) {

    filename=file;
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
            case HP_REAL: {
                refresh();
            }
            break;
            case HP_APP: {
                refresh();
            }
            case HP_MATRIX: {
                refresh();
            }
            break;
            case HP_LIST: {
                refresh();
            }
            break;
            case HP_NOTE: {
                refresh();
            }
            break;
            case HP_PROG: {
                refresh();
            }
            break;
            default:;
    }
}

void hpTreeItem::addChild(AbstractData *obj) {
    hpTreeItem * subItem=nullptr;
    hp_DataType type;

    qDebug()<<"hpTreeItem:Adding Child";

    if (obj!=nullptr) {
        QString name;
        //create fixed variable list

        type= obj->getType();
        name=obj->getName();

        qDebug()<<"hpTreeItem:this is "<<getFileName()<<" of type:"<<getType()
               <<"Column:"<<columnCount();

        //check if this is the same type
        if ((getType()==type)&&(columnCount()<3))
        {

            qDebug()<<"hpTreeItem: Creating hpTreeItem:"<<type;
            qDebug()<<"Column Count:"<<columnCount();

            switch (type) {
                case HP_APP: {
                subItem= new hpTreeItem(name,getDataStore(),1);
                subItem->setType(type);
                subItem->setIcon(QIcon(func_list[HP_APP][1]));
                }
                break;
                case HP_MATRIX: {
                subItem= new hpTreeItem(name,getDataStore(),1);
                subItem->setType(type);
                subItem->setIcon(QIcon(func_list[HP_MATRIX][1]));
                }
                break;
                case HP_LIST: {
                subItem= new hpTreeItem(name,getDataStore(),1);
                subItem->setType(type);
                subItem->setIcon(QIcon(func_list[HP_LIST][1]));

                }
                break;
                case HP_NOTE: {
                subItem= new hpTreeItem(name,getDataStore(),1);
                subItem->setType(type);
                subItem->setIcon(QIcon(func_list[HP_NOTE][1]));
                    qDebug()<<"hpTreeItem: Creating Note type:"<<type;
                }
                break;
                case HP_PROG: {
                subItem= new hpTreeItem(name,getDataStore(),1);
                subItem->setType(type);
                subItem->setIcon(QIcon(func_list[HP_PROG][1]));
                }
                break;
                case HP_REAL: {
                    setType(type);
                    setFileName(name);
                }
                break;
                case HP_COMPLEX: {
                    setType(type);
                    setFileName(name);
                }
                break;
                default: {
                    subItem= new hpTreeItem(name,getDataStore(),1);
                    subItem->setType(type);
                }
            }
            if (subItem)
                appendRow(subItem);
        }
        else {
            qDebug()<<"hpTreeItem:No Object added";
        }

    }
}

//Up date the data
void hpTreeItem::refresh() {
    int rows,i,j;
    int datalen;
    hpTreeItem * ti_child;

    QString name;
    hp_DataType type;

    rows=rowCount();
    hpCalcData * calc;

    calc=getDataStore();

    if (calc) {
        if (columnCount()==1) {
            AbstractData * obj;
            datalen=calc->dataCount();
            //get object at from calc
            //get number of list items

            //for all data items in list
            for (j=0; j<datalen; j++ ) {
             //compare with data
             obj= calc->dataAt(j);

             if (obj) {
                 for (i=0; i<rows; i++) {
                        ti_child=(hpTreeItem *)child(i);
                        name=ti_child->getFileName();
                        type=ti_child->getType();

                        if(obj->getType()==type) {
                            ti_child->addFile(obj);
                        }
                  }
             }
             else
             {
                qDebug()<<"hpTreeItem:obj is null!";
             }
            }
        }

        /*
      //delete excess
      qDebug()<<"hpTreeItem: Checking for deletes";
      if (columnCount()==1) {
        AbstractData * obj;
        int ret;
        rows=rowCount();
        flag =0;
        //for all rows check data exists
        for (i=0; i<rows; i++) {
            ti_child=(hpTreeItem *)child(i);
            if (ti_child) {
                name=ti_child->getFileName();
                type=ti_child->getType();
                for (j=0; j<datalen; j++ ) {
                    //compare with all data
                    obj= calc->dataAt(j);
                    if (obj) {
                        qDebug()<<"hpTreeItem::refresh - calling object";
                        if(obj->getType()==type) {
                            ret=ti_child->findFile(obj->getName());
                        }
                        if(ret!=0) {
                            flag =1; //obj found
                        }
                    }
                }
            }
            //If data and no row add row
            if (flag==0) {
                qDebug()<<"hpTreeItem::refresh - deleting ";
                removeRow(i);
            }
        }
      }
      */
    }

}

//check if file is in the list and add it if not
void hpTreeItem::addFile(AbstractData * obj) {

      int i;
      int rows;
      int flag;

      hpTreeItem * ti_child;
      QString name;
      hp_DataType type;

//      qDebug()<<"hpTreeItem::addFile";

      rows=rowCount();
      flag=0;

      if (obj) {
//          qDebug()<<"hpTreeItem::addFile - File Not null";
          for (i=0; i<rows; i++) {
            ti_child=(hpTreeItem *)child(i);
            name=ti_child->getFileName();
            type=ti_child->getType();
            if(obj->equivalent(name,type)) {
               flag =1; //obj found
            }
         }
         //If data and no row add row
         if (flag==0) {
             qDebug()<<"hpTreeItem::addFile - Adding a child";
             addChild(obj);
         }
      }
      else
     {
        qDebug()<<"hpTreeItem:obj is null!";
      }
}

//check if file is in the list of children and add it if not return 0 else return row;
int hpTreeItem::findFile(QString dataname) {

      int i;
      int rows;
      int flag;

      rows=rowCount();
      hpTreeItem * ti_child;
      QString name;

      for (i=0; i<rows; i++) {
            ti_child=static_cast<hpTreeItem *>(child(i));
            dataname=ti_child->getFileName();
            if(name==dataname) {
               return i;
            }
      }
      return 0;
}


hpTreeItem::~hpTreeItem() {

 /*
    if (hpvaredit!=nullptr) {
        hpvaredit->close();
        delete hpvaredit;
        hpvaredit=nullptr;
    }

    if (hptextedit!=nullptr) {
        hptextedit->close();
        delete hptextedit;
        hptextedit=nullptr;
    }

    if (mdiarea!=nullptr) {
        mdiarea->close();
        mdiarea=nullptr;
    }
*/

    //qDebug()<<"hpTreeItem:: delete";
}

