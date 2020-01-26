#ifndef CONTENTFILESYSTEMMODEL_H
#define CONTENTFILESYSTEMMODEL_H
#include <QObject>
#include <QFileSystemModel>
#include <QDropEvent>
#include <QMdiArea>
#include "hpdata.h"

#define FILE_TYPE 5

class contentFileSystemModel : public QFileSystemModel
{
    Q_OBJECT
public:
    contentFileSystemModel(QObject * parent=nullptr);
    virtual QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row,
                int column, const QModelIndex &parent);
    QMimeData* mimeData(const QModelIndexList &indexes) const;
    Qt::DropActions supportedDropActions() const;
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    void clickAction(QMdiArea * mdiwin,QModelIndex &index);
    void openFile(QMdiArea * mdiwin,QModelIndex &index);
    void deleteFile(QModelIndex &index);
    void renameFile(QModelIndex &index, QString newName);
    hp_DataStruct getFileType(QFileInfo info) const;
    QString getFileType(hp_DataType) const;
    AbstractData * readFile(QFileInfo fileinfo) const;
    int writeFile(QFileInfo fileinfo, QByteArray data_in) const;
    ~contentFileSystemModel();

private:
    const static QString filetype_list[][2];
    const static hp_DataType func_type[];
    const static QString file_type[];
//    const static std::array<std::pair<hp_DataType,QString>,FILE_TYPE> file_type;
};

#endif // CONTENTFILESYSTEMMODEL_H
