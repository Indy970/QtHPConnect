//
//
//
//
// This class acts as a data store for the interface and calls for data as required
//
#include "hpdata.h"
#include "global.h"
#include "errorhandler.h"
#include "abstractdata.h"
#include "hp_typedef.h"

const QString hpCalcData::func_list[FUNC_NUM][2]={{"Application Library",":/icons/apps_32x32.png"},
                                           {"CAS Vars",":/icons/casFolder_32x32.png"},
                                           {"Real",":/icons/real_32x32.png"},
                                           {"Complex",":/icons/complex_32x32.png"},
                                           {"Lists",":/icons/list_32x32.png"},
                                           {"Matrices",":/icons/table_32x32.png"},
                                           {"Notes",":/icons/note_32x32.png"},
                                           {"Programs",":/icons/program_32x32.png"},
                                           {"Variables",":/icons/varFolder_32x32.png"}
                                           };


const hp_DataType hpCalcData::func_type[FUNC_NUM]={HP_APP,
                                           HP_CAS,
                                           HP_REAL,
                                           HP_COMPLEX,
                                           HP_LIST,
                                           HP_MATRIX,
                                           HP_NOTE,
                                           HP_PROG,
                                           HP_VAR
                                           };



//constructor
hpCalcData::hpCalcData(QString name, hpusb * handle)
        :QObject()
{
    hp_api = handle;
    lData.clear();
    calculatorName=name;

    //open usb port and store the handle
    if (hp_api) {
        hp_handle.calc=this;
        hp_api->hp_open(getHandle());
    }
}

//return the interface class
hp_Handle * hpCalcData::getHandle() {
    hp_handle.calc=this;
    return &hp_handle;
}

//return the interface class
hpusb * hpCalcData::getAPI() {
    return hp_api;
}

//data managment
hp_Information hpCalcData::getInfo() {

    return hp_info;
}

//get Name
QString hpCalcData::getName() {

    return hp_info.name;
}

//get Calculator Name
QString hpCalcData::getCalculatorName() {

    return calculatorName;
}


//get Settings
hp_Settings hpCalcData::getSettings() {

    return hp_homesettings;
}

//Add a new object to the list
void hpCalcData::addData(AbstractData * data) {
    lData.append(data);
}

void hpCalcData::deleteData(AbstractData * data) {
 //   lData.erase();
    if (data!=nullptr) {
        QString name=data->getName();
        hp_DataType type=data->getType();
        QList<AbstractData *>::iterator i;
        i=findData(name,type);

        lData.erase(i);
        delete data;
        qDebug()<<"hpCalcData::deleteData - Object Deleted";
    }
    else {
        qDebug()<<"hpCalcData::deleteData - Object not deleted";
    }
}

//returns position of entry in list or returns 0;
QList<AbstractData *>::iterator  hpCalcData::findData(QString name, hp_DataType dataType) {

    QList<AbstractData *>::iterator i;
    AbstractData * ldata;

    i = lData.begin();
    while (i!= lData.end()) {
        ldata=*i;
        if (ldata->equivalent(name,dataType)) {
            return i;
        }

    }

    return nullptr;
}

//returns position of entry in list or returns 0;
AbstractData * hpCalcData::getData(QString name, hp_DataType dataType) {


    for (int i = 0; i < lData.size(); ++i) {
        if (lData.at(i)->equivalent(name,dataType)) {

            qDebug()<<"hpCalcData::Data found"<<name<<" "<<dataType;
            return lData.at(i);
        }
    }

    qDebug()<<"hpCalcData::No data found"<<name<<" "<<dataType;;
    return nullptr;
}

AbstractData * hpCalcData::dataAt(int i) {
    return lData.at(i);
}

int hpCalcData::dataCount() {
    return lData.size();
}

void hpCalcData::refresh() {
    readInfo();
    readSettings();
}

//read Settings via usb
void hpCalcData::readSettings() {

    hpusb * api;
    hp_Handle * handle;
    hp_Settings hpset;

    errlog("hpCalcData::readSettings: -Reading Settings");
    qDebug()<<"Reading Settings";
    api=getAPI();
    handle=getHandle();

    if (api) {
      if(handle) {
        qDebug()<<QString().asprintf("%s %p",__FUNCTION__,handle->usbhandle);
        if (api) {
            api->get_settings(handle);
        }
      }
      else {
              errlog("hpCalcData::readSettings Handle null");
      }
    }
    else {
     errlog("hpCalcData::readSettings API null");
    }
 //   hp_homesettings=hpset;

 //   hp_Change change;
 //   change.dataChange=HP_MAIN;
 //   emit dataChanged(change);
}

//read Settings via usb
void hpCalcData::readScreen() {

    hpusb * api;
    hp_Handle * handle;
    hp_Settings hpset;

    errlog("Reading Screen");

    api=getAPI();
    handle=getHandle();

    QByteArray imageData;

    if (api) {
      if(handle) {
        qDebug()<<QString().asprintf("%s %p",__FUNCTION__,handle->usbhandle);
        if (api) {
            api->get_screen_shot(handle);
 //           if (screenShot!=nullptr) {
 //                   delete screenShot;
 //           }
//
//            screenShot = new QPixmap();
//            screenShot->loadFromData(imageData);
        }
      }
    }
//    emit emitChange(HP_SCREEN);
}

//recieve Screenshot
void hpCalcData::recvScreen(hp_ScreenShot shot) {

    errlog("Recieving Screen");

    QByteArray imageData;
    if (screenShot!=nullptr) {
              delete screenShot;
            }
     screenShot = shot.image;

    emit emitChange(HP_SCREEN);
}

//recieve Setting
void hpCalcData::recvSettings(hp_Data data) {

    QString filename;
    errlog("hpCalcData::recvSettings: Recieving Setting");
    filename = data.name;
    qDebug()<<filename;

    if (filename==QStringLiteral("calc.hpsettings")) {
        qDebug()<<"hpCalcData::recvSetting - Setting";

        qDebug()<<"hpCalcData::recvSetting - Real";
        errlog("hpCalcData::recvSetting - Real");
        Real * obj1 = new Real(data.name,HP_REAL);
        obj1->setData(data.data);
        addData(obj1);
        emit emitChange(HP_REAL);

        qDebug()<<"hpCalcData::recvSetting - Complex";
        errlog("hpCalcData::recvSetting - Complex");
        Complex * obj2 = new Complex(data.name,HP_COMPLEX);
        obj2->setData(data.data);
        addData(obj2);
        emit emitChange(HP_COMPLEX);

    }

    if (filename== QStringLiteral("cas.hpsettings")) {
        qDebug()<<"cas.chps";

        qDebug()<<"hpCalcData::recvSetting - CAS";
        CASVariables * obj2 = new CASVariables(data.name,HP_CAS);
        obj2->setData(data.data);
        addData(obj2);
        emit emitChange(HP_CAS);
    }

    if (filename==QStringLiteral("calc.hpvars")) {

        qDebug()<<"hpCalcData::recvSetting - Variables";
        Variables * var = new Variables(data.name,HP_VAR);
        var->setData(data.data);
        addData(var);
    }

    if (filename==QStringLiteral("settings")) {
        qDebug()<<"set-";

        qDebug()<<"hpCalcData::recvSetting - Variables";
        Settings * var = new Settings(data.name,HP_SETTINGS);
        var->setData(data.data);
        addData(var);
    }

    emit emitChange(HP_MAIN);
}


//recieve Program
void hpCalcData::recvProg(hp_Prog program) {

    errlog("Recieving Program");
    qDebug()<<"hpCalcData::recvProg";

    qDebug()<<program.filename;
    //qDebug()<<program.prog;

    Program * obj = new Program(program.filename,HP_PROG, program.prog);
    obj->setData(program.data);
    addData(obj);

    emit emitChange(HP_PROG);
}

//recieve Program
void hpCalcData::recvNote(hp_Note note) {

    errlog("Recieving Note");
    qDebug()<<"hpCalcData::recvNote";

    qDebug()<<note.filename;

    Notes * obj = new Notes(note.filename,HP_NOTE, note.text);
    obj->setData(note.data);
    addData(obj);

    emit emitChange(HP_NOTE);
}

//recieve Program
void hpCalcData::recvData(hp_Data data) {

    errlog("Recieving Data");

    switch (data.type) {
        case HP_APP: {
            qDebug()<<"hpCalcData::recvData - Application";
            Application * obj = new Application(data.name,data.type);
            obj->setData(data.data);
            addData(obj);
            emit emitChange(HP_APP);
        }
        break;
     case HP_LIST: {
            List * obj = new List(data.name,data.type);
            obj->setData(data.data);
            addData(obj);
            emit emitChange(HP_LIST);
        }
        break;
    case HP_MATRIX: {
            qDebug()<<"hpCalcData::recvData - Matrix";
            Matrix * obj = new Matrix(data.name,data.type);
            obj->setData(data.data);
            addData(obj);
            emit emitChange(HP_MATRIX);
        }
        break;
    }
}


void hpCalcData::emitChange(hp_DataType type) {

    hp_Change change;
    change.dataChange=type;
    change.calc = this;

    emit dataChanged(change);

}

hp_ScreenShot hpCalcData::getScreenShot() {
    hp_ScreenShot scn;

    scn.image = screenShot;
    scn.format = CALC_SCREENSHOT_FORMAT_PRIME_PNG_320x240x16;
    scn.calc = this;

    return scn;
}

//set Settings
int hpCalcData::setSettings(hp_Settings set) {

    hp_homesettings=set;
}

void hpCalcData::setInfo(hp_Information dtype) {
    hp_info=dtype;
    return;
}

//read information via hp interface
void hpCalcData::readInfo() {

    hpusb * api;
    hp_Handle * handle;
    hp_Information hpinfo;

    api=getAPI();
    handle=getHandle();

    if (api) {
      if(handle) {
        qDebug()<<QString().asprintf("%s %p",__FUNCTION__,handle->usbhandle);
        if (api)
            api->load_info(handle);
      }
    }
}

void hpCalcData::recvInfo(hp_Information hpinfo) {

       hp_info=hpinfo;
       emit emitChange(HP_MAIN);
}


void hpCalcData::vpkt_send_experiments(int cmd) {

    hpusb * api;
    hp_Handle * handle;

    api=getAPI();
    handle=getHandle();

    if (api) {
      if(handle) {
        if (api) {
            api->is_ready(handle);

           // api->vpkt_send_experiments(handle,cmd);

        }
      }
    }

}

hpCalcData::~hpCalcData() {
 qDebug()<<"Close ~hpCalcData";
};
