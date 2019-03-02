#include "global.h"
#include "vartablemodel.h"
#include "abstractdata.h"

varTableModel::varTableModel(QObject *parent,
        hpCalcData * dataStore,
        QString file,
        hp_DataType dtype)
    :QAbstractTableModel(parent)
{
    hpcalc = dataStore;
    filename=file;
    type=dtype;
    setup();
}

void varTableModel::setup()
{
    if (hpcalc) {

        dataobj=hpcalc->getData(filename,type);

  //      qDebug()<<"varTableModel: type"<<dataobj->getType();
    }

   return;
}

int varTableModel::rowCount(const QModelIndex & /*parent*/) const
{
  int size=16; //should be zero
  if (type==HP_LIST) {
      List * list;
      list = (List *)dataobj;
      size= list->getListSize();
  }
  if (type==HP_MATRIX) {
      Matrix * matrix;
      matrix = (Matrix *)dataobj;
      size= matrix->getMatrixRows();
  }

   return size;
}

int varTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    int size=1;
    if (type==HP_MATRIX) {
        Matrix * matrix;
        matrix = (Matrix *)dataobj;
        size= matrix->getMatrixColumns();
    }
    return size;
}

QVariant varTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {

      QString item;

      if (type==HP_LIST) {
             List * list;
             list = (List *)dataobj;
             item = list->getItem(index.row());
             return item;
       }
      if (type==HP_MATRIX) {
             Matrix * matrix;
             matrix = (Matrix *)dataobj;
             item = matrix->getItem(index.row(),index.column());
             return item;
       }

       return QString("Row%1, Column%2")
                   .arg(index.row() + 1)
                   .arg(index.column() +1);
    }
    return QVariant();
}
