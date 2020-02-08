/*
 * QtHP Connect: hand-helds support interface.
 * Copyright (C) 2019 Ian Gebbie
 * Code patterns and snippets borrowed from libhpcalcs :
 * Copyright (C) 1999-2009 Romain Li�vin
 * Copyright (C) 2009-2013 Lionel Debroux
 * Copyright (C) 1999-2013 libti* contributors.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */


#include "global.h"
#include "vartablemodel.h"
#include "abstractdata.h"

const QStringList varTableModel::real_header={ "A",
                                            "B",
                                            "C",
                                            "D",
                                            "E",
                                            "F",
                                            "G",
                                            "H",
                                            "I",
                                            "J",
                                            "K",
                                            "L",
                                            "M",
                                            "N",
                                            "o",
                                            "P",
                                            "Q",
                                            "R",
                                            "S",
                                            "T",
                                            "U",
                                            "V",
                                            "W",
                                            "X",
                                            "Y",
                                            "Z",
                                            "THETA"
                                           };

const QStringList varTableModel::complex_header={ "Z0",
                                            "Z1",
                                            "Z2",
                                            "Z3",
                                            "Z4",
                                            "Z5",
                                            "Z6",
                                            "Z7",
                                            "Z8",
                                            "Z9"
                                           };

varTableModel::varTableModel(QObject *parent,
        AbstractData * data,
        QString file,
        hp_DataType dtype)
    :QAbstractTableModel(parent)
{
    q_parent=parent;
    dataobj = data;
    filename=file;
    type=dtype;
    setup();
}

//REWORK!
QModelIndex varTableModel::parent(const QModelIndex &index) const {


    return QModelIndex();
}

//rework!
QModelIndex varTableModel::index(int row, int column, const QModelIndex &parent) const {

    return createIndex(row,column);
}


void varTableModel::setup()
{
//    if (hpcalc) {

//        dataobj=hpcalc->getData(filename,type);

  //      qDebug()<<"varTableModel: type"<<dataobj->getType();
//    }

   return;
}

int varTableModel::rowCount(const QModelIndex & parent) const
{
  int size=16; //should be zero
  if (type==HP_LIST) {
      List * list;
      list = (List *)dataobj;
      size= list->getListSize();
  }
  if (type==HP_REAL) {
      Real * real;
      real = (Real *)dataobj;
      size= real->getListSize();
  }
  if (type==HP_COMPLEX) {
      Complex * complex;
      complex = (Complex *)dataobj;
      size= complex->getListSize();
  }
  if (type==HP_MATRIX) {
      Matrix * matrix;
      matrix = (Matrix *)dataobj;
      size= matrix->getMatrixRows();
//      qDebug()<<matrix->getName()<<" row"<<size;
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
 //       qDebug()<<matrix->getName()<<" column"<<size;

    }
    if (type==HP_COMPLEX) {
        Complex * complex;
        complex = (Complex *)dataobj;
 //       size= matrix->getMatrixColumns();
 //       qDebug()<<matrix->getName()<<" column"<<size;
        return 1;
    }
    return size;
}

QVariant varTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {

      QString item=QStringLiteral("-");

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
      if (type==HP_REAL) {
             Real * real;
             real = (Real *)dataobj;
             item = real->getItem(index.row());
             return item;
       }
      if (type==HP_COMPLEX) {
             Complex * complex;
             complex = (Complex *)dataobj;
             item = complex->getItem(index.row());
             return item;
       }

       return QString("Row%1, Column%2")
                   .arg(index.row() + 1)
                   .arg(index.column() +1);
    }
    return QVariant();
}

QVariant  varTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
         }

      if (orientation == Qt::Horizontal) {
        if (type==HP_REAL) {
             return QString("%1").arg(section+1);
        }
        return QString("%1").arg(section+1);

      }
      if (orientation == Qt::Vertical) {
        if (type==HP_REAL) {
            if (section < real_header.size()) {
                return real_header.at(section);
            }
            else {
                return QString("%1").arg(section);
            }
        }

        if (type==HP_COMPLEX) {
            if (section < complex_header.size()) {
                return complex_header.at(section);
            }
            else {
                return QString("%1").arg(section);
            }
        }

        return QString("%1").arg(section);
      }

    return QVariant();
}

varTableModel::~varTableModel() {
       qDebug()<<"Entering ~varTableModel()";
}
