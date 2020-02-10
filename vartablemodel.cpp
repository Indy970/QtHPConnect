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


#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QApplication>
#include <QSettings>

#include "global.h"
#include "vartablemodel.h"

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

Qt::ItemFlags varTableModel::flags(const QModelIndex &index) const {

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

//rework!
QModelIndex varTableModel::index(int row, int column, const QModelIndex &parent) const {

    return createIndex(row,column);
}

//alter the data table if data is edited
bool varTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {

    qDebug()<<"Data Changed";

    if (type==HP_LIST) {
           List * list;
           list = static_cast<List *>(dataobj);
           list->setItem(index.row(),value.toString(),value.toReal());
           return true;
     }
    if (type==HP_MATRIX) {
           Matrix * matrix;
           matrix = static_cast<Matrix *>(dataobj);
           matrix->setItem(index.row(),index.column(),value.toString(),value.toReal());
           return true;
     }
    if (type==HP_REAL) {
           Real * real;
           real = static_cast<Real *>(dataobj);
 //          item = real->getItem(index.row());
 //          return item;
     }
    if (type==HP_COMPLEX) {
           Complex * complex;
           complex = static_cast<Complex *>(dataobj);
 //          item = complex->getItem(index.row());
 //          return item;
     }

    return false;
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
      list = static_cast<List *>(dataobj);
      size= list->getListSize()+1;
  }
  if (type==HP_REAL) {
      Real * real;
      real = static_cast<Real *>(dataobj);
      size= real->getListSize();
  }
  if (type==HP_COMPLEX) {
      Complex * complex;
      complex = static_cast<Complex *>(dataobj);
      size= complex->getListSize();
  }
  if (type==HP_MATRIX) {
      Matrix * matrix;
      matrix = static_cast<Matrix *>(dataobj);
      size= matrix->getMatrixRows()+1;
//      qDebug()<<matrix->getName()<<" row"<<size;
  }

   return size;
}

int varTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    int size=1;
    if (type==HP_MATRIX) {
        Matrix * matrix;
        matrix = static_cast<Matrix *>(dataobj);
        size= matrix->getMatrixColumns()+1;
 //       qDebug()<<matrix->getName()<<" column"<<size;

    }
    if (type==HP_COMPLEX) {
        Complex * complex;
        complex = static_cast<Complex *>(dataobj);
 //       size= matrix->getMatrixColumns();
 //       qDebug()<<matrix->getName()<<" column"<<size;
        return 1;
    }
    return size;
}

void varTableModel::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles) {


    qDebug()<<"Data Changed";


}


QVariant varTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {

      QString item=QStringLiteral("-");

      if (type==HP_LIST) {
             List * list;
             list = static_cast<List *>(dataobj);
             item = list->getItem(index.row());
             return item;
       }
      if (type==HP_MATRIX) {
             Matrix * matrix;
             matrix = static_cast<Matrix *>(dataobj);
             item = matrix->getItem(index.row(),index.column());
             return item;
       }
      if (type==HP_REAL) {
             Real * real;
             real = static_cast<Real *>(dataobj);
             item = real->getItem(index.row());
             return item;
       }
      if (type==HP_COMPLEX) {
             Complex * complex;
             complex = static_cast<Complex *>(dataobj);
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


bool varTableModel::save(QString Calculator)
{
    if (isUntitled) {
        return saveAs(Calculator);
    } else {
        return saveFile("xx");
    }
}

bool varTableModel::save(QFileInfo file)
{
    if (isUntitled) {
        return saveAs(file);
    } else {
        return saveFile("xx");
    }
}

bool varTableModel::saveAs(QFileInfo fileinfo)
{

//    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
//                                                    fileinfo.absoluteFilePath());
//    if (fileName.isEmpty())
//        return false;

    //return saveFile(fileName);


}


bool varTableModel::saveAs(QString calculaor)
{

//    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
//                                                    fileinfo.absoluteFilePath());
    QString fileName="";
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool varTableModel::saveFile(const QString &fileName)
{
    QFileInfo fileinfo(defaultPath,fileName);
    QFile file(fileinfo.absoluteFilePath());
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
 //       QMessageBox::warning(this, tr("MDI"),
 //                            tr("Cannot write file %1:\n%2.")
 //                            .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
 //   out << toPlainText();
    QApplication::restoreOverrideCursor();

//    setCurrentFile(fileName);
    return true;
}


varTableModel::~varTableModel() {
       qDebug()<<"Entering ~varTableModel()";
}
