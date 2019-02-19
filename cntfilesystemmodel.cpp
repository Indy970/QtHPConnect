#include "global.h"
#include "cntfilesystemmodel.h"

#define FILE_NUM 9

//Todo fix for all file types
const QString contentFileSystemModel::filetype_list[FILE_NUM][2]={{".hpprgm",":/icons/apps_32x32.png"},
                                           {"CAS Vars",":/icons/casFolder_32x32.png"},
                                           {"Complex",":/icons/complex_32x32.png"},
                                           {"hplist",":/icons/list_16x16.png"},
                                           {"Matrices",":/icons/table_32x32.png"},
                                           {"Notes",":/icons/note_32x32.png"},
                                           {"hpprgm",":/icons/program_16x16.png"},
                                           {"Real",":/icons/real_32x32.png"},
                                           {"Variables",":/icons/varFolder_32x32.png"}
                                           };


contentFileSystemModel::contentFileSystemModel(QObject * parent)
    :QFileSystemModel(parent)
{

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
