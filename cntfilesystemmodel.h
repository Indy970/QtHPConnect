#ifndef CONTENTFILESYSTEMMODEL_H
#define CONTENTFILESYSTEMMODEL_H
#include <QObject>
#include <QFileSystemModel>
#include <QDropEvent>
#include <QMdiArea>
#include "hpdata.h"

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
    hp_DataStruct getFileType(QFileInfo info);

private:
    const static QString filetype_list[][2];
    const static hp_DataType func_type[];
    const static QString file_type[];
};

#endif // CONTENTFILESYSTEMMODEL_H
