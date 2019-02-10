#include "vartablemodel.h"

varTableModel::varTableModel(QObject *parent)
    :QAbstractTableModel(parent)
{

}

int varTableModel::rowCount(const QModelIndex & /*parent*/) const
{
   return 16;
}

int varTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 1;
}

QVariant varTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
       return QString("Row%1, Column%2")
                   .arg(index.row() + 1)
                   .arg(index.column() +1);

    return QVariant();
}
