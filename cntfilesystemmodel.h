#ifndef CONTENTFILESYSTEMMODEL_H
#define CONTENTFILESYSTEMMODEL_H
#include <QObject>
#include <QFileSystemModel>

class contentFileSystemModel : public QFileSystemModel
{
    Q_OBJECT
public:
    contentFileSystemModel(QObject * parent=nullptr);
    virtual QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;

private:
    const static QString filetype_list[][2];
};

#endif // CONTENTFILESYSTEMMODEL_H
