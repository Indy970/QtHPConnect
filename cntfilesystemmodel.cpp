#include "global.h"
#include "cntfilesystemmodel.h"
#include <QMimeData>
#include <QStringListModel>

#include "hp_mditexteditor.h"

#define FILE_NUM 9
#define FILE_TYPE 5
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

const hp_DataType contentFileSystemModel::func_type[FILE_TYPE]={
                                           HP_PROG,
                                           HP_LIST,
                                           HP_MATRIX,
                                           HP_NOTE,
                                           HP_MAIN};

const QString contentFileSystemModel::file_type[FILE_TYPE]={"hpprgm",
                                                           "hplist",
                                                           "hpmat",
                                                           "hpnote",
                                                            ""};

contentFileSystemModel::contentFileSystemModel(QObject * parent)
    :QFileSystemModel(parent)
{

}

//Get and pass on the data to be dragged
QMimeData* contentFileSystemModel::mimeData(const QModelIndexList &indexes) const
{

    QMimeData *mimeDataPtr = new QMimeData();
    QByteArray mydata;

    qDebug()<<"contentFileSystemModel::mimeData";

    mimeDataPtr->setData("application/x-qstandarditemmodeldatalist",mydata);

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
    return true;
}

//Process the drop action
bool contentFileSystemModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row,
            int column, const QModelIndex &parent)
{
    qDebug()<<"contentFileSystemModel::DropMineData";
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

void contentFileSystemModel::clickAction(QMdiArea * mdiwin, QModelIndex &index) {

    hp_mdiTextEdit * hptextedit = nullptr;
    hp_DataStruct filedata;
    AbstractData * data=nullptr;

    QFileInfo info = contentFileSystemModel::fileInfo(index);
    QFile file(info.absoluteFilePath());
    if (file.open(QIODevice::ReadOnly),QFileDevice::AutoCloseHandle) {
        QDataStream in(&file);

        filedata=getFileType(info);

        switch (filedata.type) {

            case HP_PROG: {

                data = new Program(filedata.filename, HP_PROG, QStringLiteral(""));
                data->parseData(in);
            }
            break;
            default: ;
        }

        if (hptextedit==nullptr)
            hptextedit = new hp_mdiTextEdit(mdiwin,filedata, data);
        if (hptextedit!=nullptr)
            hptextedit ->show();

        file.close();
    }
    qDebug()<<"ClickAction "<<info.absoluteFilePath();
}

hp_DataStruct contentFileSystemModel::getFileType(QFileInfo info) {
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
