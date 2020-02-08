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
    bool openFile(QMdiArea * mdiwin,QModelIndex &index);
    bool openFile(QMdiArea * mdiwin,QFileInfo file);
    void deleteFile(QModelIndex &index);
    void renameFile(QModelIndex &index, QString newName);
    hp_DataStruct getFileType(QFileInfo info) const;
    bool isFileType(QFileInfo info) const;
    QString getFileType(hp_DataType) const;
    AbstractData * readFile(QFileInfo fileinfo) const;
    int writeFile(QFileInfo fileinfo, QByteArray data_in) const;
    bool createNewFolder(QString foldername);
    bool createNewNote(QMdiArea * mdiwin,QString foldername);
    bool createNewProgram(QMdiArea * mdiwin,QString foldername);
    ~contentFileSystemModel();

private:
    const static QString filetype_list[][2];
    const static hp_DataType func_type[];
    const static QString file_type[];
//    const static std::array<std::pair<hp_DataType,QString>,FILE_TYPE> file_type;
};

#endif // CONTENTFILESYSTEMMODEL_H
