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

#include "datamodel.h"

#include <QDebug>
#include <QWidget>

dataModel::dataModel(QObject *parent)
    :QAbstractTableModel(parent)
{
}

int dataModel::rowCount(const QModelIndex & /*parent*/) const
{
return 2;
}

int dataModel::columnCount(const QModelIndex & /*parent*/) const
{
return 3;
}

QVariant dataModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
 //   // generate a log message when this method gets called
 //   qDebug() << QString("row %1, col%2, role %3")
 //           .arg(row).arg(col).arg(role);

    switch(role){
    case Qt::DisplayRole:
        if (row == 0 && col == 1) return QString("<--left");
        if (row == 1 && col == 1) return QString("right-->");

        return QString("Row%1, Column%2")
                .arg(row + 1)
                .arg(col +1);
        break;
/*    case Qt::FontRole:
        if (row == 0 && col == 0) //change font only for cell(0,0)
        {
            QFont boldFont;
            boldFont.setBold(true);
            return boldFont;
        }
        break;
    case Qt::BackgroundRole:

        if (row == 1 && col == 2)  //change background only for cell(1,2)
        {
            QBrush redBackground(Qt::red);
            return redBackground;
        }
        break;
    case Qt::TextAlignmentRole:

        if (row == 1 && col == 1) //change text alignment only for cell(1,1)
        {
            return Qt::AlignRight + Qt::AlignVCenter;
        }
        break;
    case Qt::CheckStateRole:

        if (row == 1 && col == 0) //add a checkbox to cell(1,0)
        {
            return Qt::Checked;
        }
*/    }
    return QVariant();
}

dataModel::~dataModel() {

    qDebug()<<"dataModel::close";
}
