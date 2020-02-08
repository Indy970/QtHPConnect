/*
 * This file is part of the QtHPConnect distribution (https://github.com/Indy970/QtHPConnect.git).
 * Copyright (c) 2020 Ian Gebbie.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 or later.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <global.h>
#include <cntfilesystemmodel.h>
#include <QMimeData>
#include <QList>
#include <QStringListModel>

#include <hp_mditexteditor.h>
#include <hp_infodialog.h>
#include <hp_mdivariableedit.h>
#include <QSettings>
#include <QTextCodec>
#include <utility>
#define FILE_NUM 9


//list of file types and associated icons
//Todo fix for all file types
const QString contentFileSystemModel::filetype_list[FILE_NUM][2]={{"hpprgm",":/icons/apps_16x16.png"},
                                           {"CAS Vars",":/icons/casFolder_16x16.png"},
                                           {"Complex",":/icons/complex_16x16.png"},
                                           {"hplist",":/icons/list_16x16.png"},
                                           {"Matrices",":/icons/table_16x16.png"},
                                           {"Notes",":/icons/note_16x16.png"},
                                           {"hpprgm",":/icons/program_16x16.png"},
                                           {"Real",":/icons/real_16x16.png"},
                                           {"Variables",":/icons/varFolder_16x16.png"}
                                           };


//list of data object types used by contentFileSystemModel
const hp_DataType contentFileSystemModel::func_type[FILE_TYPE]={
                                           HP_PROG,
                                           HP_LIST,
                                           HP_MATRIX,
                                           HP_NOTE,
                                           HP_MAIN};

/*
const std::array<std::pair<hp_DataType,QString>,FILE_TYPE> contentFileSystemModel::file_type{ {0,"hpprgm"},
                                                           {1,"hplist"},
                                                           {2,"hpmat"},
                                                           {3,"hpnote"},
                                                            {4,""}};
*/

//list of file type suffixes
const QString contentFileSystemModel::file_type[FILE_TYPE]{ "hpprgm",
                                                           "hplist",
                                                           "hpmat",
                                                           "hpnote",
                                                           ""};


//condstrutor
contentFileSystemModel::contentFileSystemModel(QObject * parent)
    :QFileSystemModel(parent)
{

}

//Get and pass on the data to be dragged
QMimeData* contentFileSystemModel::mimeData(const QModelIndexList &indexes) const
{

    QMimeData *mimeDataPtr = new QMimeData();
    QByteArray mydata;
    QModelIndex index;
    hp_DataStruct filedata;

    qDebug()<<"contentFileSystemModel::mimeData";

    foreach(index,indexes) {

    index=indexes.first();
 //   qDebug()<<index.data(Qt::DisplayRole);
 //   QString data;

    QFileInfo info = contentFileSystemModel::fileInfo(index);
    QFile file(info.absoluteFilePath());

    qDebug()<<info.absoluteFilePath();

    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);

        filedata=getFileType(info);

        in.setByteOrder(QDataStream::LittleEndian);
        qint8 c;

        //read in file
        in.startTransaction();
        while(!in.atEnd()) {
            in>>c;
            mydata.append(c);
        }
        mimeDataPtr->setText(info.baseName());


        switch (filedata.type) {

            case HP_PROG:
            case HP_APP:
            case HP_MATRIX:
            case HP_NOTE:
            case HP_LIST:
            case HP_VAR: {
              mimeDataPtr->setData(mimetypes[filedata.type][1],mydata);
            break;
            }
            case HP_CAS:
            case HP_MAIN:
            case HP_COMPLEX:
            case HP_SCREEN:
            case HP_REAL: {
                // no action
            }
        }
    }

    file.close();
    }

    //Allow copy to external file managers
    QList<QUrl> urls;
    QFileInfo info;
    QUrl url;
    foreach(index,indexes) {
        info = contentFileSystemModel::fileInfo(index);

        url = QUrl::fromLocalFile(info.absoluteFilePath());

        urls.append(url);
    }
    mimeDataPtr->setUrls(urls);

    return mimeDataPtr;
}

//Set drop actions supported
Qt::DropActions contentFileSystemModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction| Qt::TargetMoveAction;
}

//Not sure that this is neccesary. Item should control this
Qt::ItemFlags contentFileSystemModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QFileSystemModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

//Allow drop in location
bool contentFileSystemModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{

    qDebug()<<data->formats();

    if (data->hasUrls()) {
        return true;
    }

    if (data->hasText()) {
        return true;
    }

    return false;
}

//Process the drop action
bool contentFileSystemModel::dropMimeData(const QMimeData* md_data, Qt::DropAction action, int row,
            int column, const QModelIndex &parent)
{

    qDebug()<<"contentFileSystemModel::DropMineData";

    QSettings appSettings("IRGP","QtHPconnect");
    QString path=appSettings.value("contentPath").toString();

    QDir dir;
    dir= rootDirectory();

    if (!dir.exists()) {
        qWarning()<<tr("Content Path Does not exist: ")<<path;
        if(!dir.mkpath("."))
        {
           qWarning()<<tr("Path could not be created: ")<<path;
           return false;
        }
    }

    if (action == Qt::IgnoreAction) {

        return true;
    }

    //Check for external file drop
    if (md_data->hasUrls()) {

        QString name = md_data->text();
        QList<QUrl> urls = md_data->urls();
        QUrl url;
        QString filefrom;
        QString fileto;

        foreach(url,urls) {
            filefrom=url.toLocalFile();
            QFileInfo fileinfo(filefrom);
            fileto=path+"/"+url.fileName();
            if(isFileType(fileinfo)) {
                //consider check to limit unusual file types
                QFile::copy(filefrom,fileto);
            }
        }

      return true;
    }

    //rework
    QByteArray data_in;
    QByteArray typeary;
    QString name = md_data->text();
    typeary=md_data->data("application/x-type");
    int type_i=typeary[0];
    hp_DataType type=static_cast<hp_DataType>(type_i);
    QString type_str=getFileType(type);
    name=name+"_2"+"."+type_str;
    data_in=md_data->data("application/x-qabstractmodeldatalist");
    QFileInfo fileinfo(path,name);

    qDebug()<<data_in;
    qDebug()<<fileinfo;

//	QDataStream stream(&encodedData, QIODevice::ReadOnly);

    if (writeFile(fileinfo,data_in)>-1)
    {
        return true;
    }
    else
        return true;
}

//override to return a file object
QVariant contentFileSystemModel::data( const QModelIndex &index, int role ) const {

    if( role == Qt::DecorationRole )
    {
        QString name = index.data().toString();
        QFileInfo info = contentFileSystemModel::fileInfo(index);
        //QFileInfo info(name);

        if((info.isFile()&&(name==info.fileName())))
        {
            int i;
            for (i=0;i<FILE_NUM;i++) {
                if(info.suffix() == filetype_list[i][0])
                    return QPixmap(filetype_list[i][1]);//I pick the icon depending on the extension
            }
            return QPixmap(":/icons/file_16x16.png");
        }
    }
    return QFileSystemModel::data(index, role);
}

//action if file is left clicked
void contentFileSystemModel::clickAction(QMdiArea * mdiwin, QModelIndex &index) {
    openFile(mdiwin,index);
    return;
}

//open a file in the mdi window
bool contentFileSystemModel::openFile(QMdiArea * mdiwin, QModelIndex &index) {

    QFileInfo info = contentFileSystemModel::fileInfo(index);
    return openFile(mdiwin,info);
}

//open a file in the mdi window
bool contentFileSystemModel::openFile(QMdiArea * mdiwin,QFileInfo info) {

    hp_mdiTextEdit * hptextedit = nullptr;
    AbstractData * data=nullptr;

    QString calc;
    QString name;

    hp_DataType type;
    hp_Information hpinfo;
//    hp_infoDialog * hpinfodlg;
    hp_mdiVariableEdit * hpvaredit=nullptr;

     data = readFile(info);

        if (data!=nullptr) {
            type=data->getType();
            switch (type) {
                case HP_NOTE:
                case HP_PROG:   {
                    if (hptextedit==nullptr)
                        hptextedit = new hp_mdiTextEdit(mdiwin,info, data);
                    if (hptextedit!=nullptr)
                        hptextedit ->show();
                    }
                    break;
                case HP_CAS:
                case HP_REAL:
                case HP_COMPLEX:
                case HP_LIST:
                case HP_MATRIX: {
                    if (hpvaredit==nullptr) {
                        if (data!=nullptr) {
                            qDebug()<<"Opening Varedit";
                            hpvaredit = new hp_mdiVariableEdit(mdiwin,info,data);
                        }
                    }
                    if (hpvaredit!=nullptr)
                        hpvaredit ->show();
                    }
                    break;
            }
        }
        else {
            qWarning()<<"Read file return null data";
        }
        return false;
    qDebug()<<"ClickAction "<<info.absoluteFilePath();

}

//delete  a file
void contentFileSystemModel::deleteFile(QModelIndex &index) {
   QFileInfo fileinfo = contentFileSystemModel::fileInfo(index);
   qDebug()<<"deleteFile "<<fileinfo.absoluteFilePath();
   QFile file(fileinfo.absoluteFilePath());
   file.remove();
}

//rename a file
void contentFileSystemModel::renameFile(QModelIndex &index, QString newName) {
   QFileInfo fileinfo = contentFileSystemModel::fileInfo(index);

   QFile file(fileinfo.absoluteFilePath());
   QDir dir=fileinfo.absoluteDir();
   QString newPath;

   newPath=dir.absolutePath()+"/"+newName;

   if (file.rename(newPath)) {
    qDebug()<<"File rename succesfull";
   }
   else {
    qDebug()<<"File rename failed";
   }

   return;
}

//read a file and return the resulting data object
AbstractData * contentFileSystemModel::readFile(QFileInfo fileinfo) const {

    AbstractData * data=nullptr;
    hp_DataStruct filedata;
    QFile file(fileinfo.absoluteFilePath());

    if (file.open(QIODevice::ReadOnly),QFileDevice::AutoCloseHandle) {
        QDataStream in(&file);
//        QTextCodec *codec = QTextCodec::codecForName("UTF-16");
//        in.setCodec(codec);
        in.setByteOrder(QDataStream::LittleEndian);

        filedata=getFileType(fileinfo);

        switch (filedata.type) {

            case HP_PROG: {
                qDebug()<<"HP_PROG";
                data = new Program(filedata.filename, HP_PROG, QStringLiteral(""));
                data->parseData(in);
            }
            break;
            case HP_NOTE: {
                qDebug()<<"HP_NOTE";
                data = new Notes(filedata.filename, HP_NOTE, QStringLiteral(""));
                data->parseData(in);
            }
            break;
            case HP_LIST: {
                qDebug()<<"HP_LIST";
                data = new List(filedata.filename, HP_LIST);
                data->parseData(in);
            }
            break;
            case HP_MATRIX: {
                qDebug()<<"HP_MATRIX";
                data = new Matrix(filedata.filename, HP_MATRIX);
                data->parseData(in);
            }
            break;
            default: ;
        }

        file.close();
    }

    return data;
}

//write a file to the directory store
int contentFileSystemModel::writeFile(QFileInfo fileinfo, QByteArray data_in) const {

    QFile file(fileinfo.absoluteFilePath());

    if (file.open(QIODevice::ReadWrite)) {
        QDataStream out(&file);
//        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
//        out.setCodec(codec);
        out.setByteOrder(QDataStream::LittleEndian);

//        quint8 c;
//        for (int i =0 ;i< data_in.length();i++)
//        {
//            c= data_in[i];
//            out<<(quint8)c;
//        }
 //      out.writeRawData(data_in,data_in.length());
        qDebug()<<"Wriiting";
        qDebug()<<data_in;

        out<<data_in;

        file.close();
        return 0;
    }

    return -1;
}

//create a new folder in the contents location
bool contentFileSystemModel::createNewFolder(QString foldername) {

    QSettings appSettings("IRGP","QtHPconnect");
    QString path=appSettings.value("contentPath").toString();

    path=path+foldername;

    QDir dir(path);

    if (!dir.exists()) {
        if(!dir.mkpath("."))
        {
           return false;
        }
    }
    qDebug()<<"Create new folder pressed"<<path;
    return true;
}

//create a new program file
bool contentFileSystemModel::createNewProgram(QMdiArea * mdiwin, QString filename) {

    QSettings appSettings("IRGP","QtHPconnect");
    QString path=appSettings.value("contentPath").toString();
    QDir dir(path);
    QFileInfo fileinfo(dir,filename+"."+getFileType(HP_PROG));

    qDebug()<<"Create new program pressed";
    return openFile(mdiwin,fileinfo);
}

//create a new note file
bool contentFileSystemModel::createNewNote(QMdiArea * mdiwin,QString filename) {

    QSettings appSettings("IRGP","QtHPconnect");
    QString path=appSettings.value("contentPath").toString();
    QDir dir(path);
    QFileInfo fileinfo(dir,filename+"."+getFileType(HP_NOTE));

    qDebug()<<"Create new program pressed";
    return openFile(mdiwin,fileinfo);
}

//return the file type
hp_DataStruct contentFileSystemModel::getFileType(QFileInfo info) const {
    hp_DataStruct filedata;

    int i;
    QString suffix;
    hp_DataType type=HP_MAIN;
    suffix=info.completeSuffix();
    for (i=0;i<FILE_TYPE;i++) {
        if (suffix==file_type[i])
            break;
    }
    type=func_type[i];
    if (type!=HP_MAIN) {
        filedata.filename=info.completeBaseName();
        filedata.type=type;
    }

    return filedata;
}

//return the file type
bool contentFileSystemModel::isFileType(QFileInfo info) const {
    hp_DataStruct filedata;

    int i;
    QString suffix;
    suffix=info.completeSuffix();
    for (i=0;i<FILE_TYPE;i++) {
        if (suffix==file_type[i])
            return true;
    }

    return false;
}

//return the file type suffix
QString contentFileSystemModel::getFileType(hp_DataType type) const {
    int i;
    QString suffix=QStringLiteral("");

        switch (type) {
        case HP_PROG: {
                i=0;
                break;
            }
            case HP_LIST: {
                i=1;
                break;
            }
        case HP_MATRIX: {
            i=2;
            break;
        }
        case HP_NOTE: {
            i=3;
            break;
        }

            default: {
                i=-1;
            }
        }
        if (i>-1) {
                suffix=file_type[i];
        }

    return suffix;
}

//destructor
contentFileSystemModel::~contentFileSystemModel() {
    qDebug()<<"contentFileSystemModel::delete";
}
