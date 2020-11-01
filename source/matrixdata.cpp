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

#include <QDebug>
#include "matrixdata.h"

extern bool BCD(QDataStream &ds, double real) ;

MatrixData::MatrixData()
{
//    clear();
    qDebug()<<"MatrixData Construct";
}

//Return the item or a zero item
itemData MatrixData::at(int row, int column)
{
    QList<itemData> * rowlist;
    itemData item;

    if (row<matrix.size()) {
        rowlist=matrix.at(row);
        if (rowlist) {
            if (column<rowlist->size()) {
                item=rowlist->at(column);
            }
        }
     }
    return item;
}

//Insert a new item and resize the list if larger
void MatrixData::insert(int row, int column, itemData data)
{
   QList<itemData> * rowlist=nullptr;

   upsize(row+1,column+1);

   if(row<matrix.size())
    rowlist=matrix.at(row);

   if(rowlist) {
        rowlist->replace(column,data);
   }
}

//Add a new row
void MatrixData::addRow(int row)
{
    int rows;
    int i;
    QList<itemData> * rowlist;


//    qDebug()<<"MatrixData::addRow" << row;
    rows=matrix.size();
    if (rows<=row)
    {
        for (i=rows; i<=row;i++) {
 //           qDebug()<<"MatrixData::addRow "<<i;

            rowlist = new QList<itemData>;
            matrix.insert(i,rowlist);
        }
    }
}

bool MatrixData::dataOut(QDataStream & ds) {

    int row;
    int column;
    int i,j=0;

        qDebug()<<"MatrixData::dataOut(ds)";

    row=rows();
    column=columns();

    itemData item;
    QString str;
    //header

    static const quint8 mydata[] = {0x02, 0x01, 0x04,0x03,0x06,0x05,0x08,0x07};

    QByteArray test= QByteArray::fromRawData((char *)mydata,sizeof (mydata));

    ds<<static_cast<quint16>(0x0001);
    ds<<static_cast<quint16>(0x8014);
    ds<<static_cast<quint16>(0x0002);
    ds<<static_cast<quint16>(0x0000);

    ds<<static_cast<quint32>(row);
    ds<<static_cast<quint32>(column);
//    ds<<static_cast<quint8>(0x30);
//    ds<<static_cast<quint8>(0x31);

   //  qDebug()<<test;
    //body
    double real;
    for (i=0;i<row;i++) {
        for (j=0;j<column;j++) {
            item=at(i,j);

            real=item.dReal;
            BCD(ds,real);
 //           if(j!=column)
 //               out.append("");
        }
//        if (i!=row)
//            out.append("");
    }

    //footer

    return true;
}

//get all columns to same size by padding it zero items
void MatrixData::resize(int newrow,int newcol) {

    QList<itemData> * row;
    int rows=0;
    int maxrows=0;
    int columns=0;
    int maxcolumns=0;
    int i;
    int j;

    qDebug()<<"MatrixData::resize";

    itemData item;
    rows = matrix.size();
    qDebug()<<"MatrixData::resize - maxcolumns "<<maxcolumns;

  //decrease rows if not requiered
    for(i=0;i<rows;i++) {
      if (i>newrow) {
          matrix.removeAt(i);
      }
  }

  //increase rows to match
  if(newrow>rows) {

      for(i=rows;i<newrow;i++) {
              addRow(i);
      }
  }

  rows = matrix.size();
  qDebug()<<"MatrixData::resize - rows "<<rows;
//match colums
  for(i=0;i<rows;i++) {
        columns=matrix.at(i)->size();
        if (columns<newcol) {
            row=matrix.at(i);
            for(j=columns;j<maxcolumns; j++) {
                row->insert(j,item);
            }
        }
        if (columns>newcol) {
            row=matrix.at(i);
            if (row) {
                for(j=columns;j<maxcolumns; j++) {
                    row->removeAt(j);
                }
            }
        }
    }
}

//get all columns to same size by padding it zero items
void MatrixData::upsize(int newrow,int newcol) {

    QList<itemData> * row;
    int rows=0;

    int columns=0;

    int i;
    int j;

    qDebug()<<"MatrixData::upsize";

    itemData item;
    item.sValue=QStringLiteral("0");

    rows = matrix.size();
//    qDebug()<<"MatrixData::upsize - rows:"<<newrow<<" columns:"<<newcol;

   //increase rows to match
   if(rows<newrow) {
      for(i=rows;i<newrow;i++) {
              addRow(i);
      }
   }

   rows = matrix.size();
//   qDebug()<<"MatrixData::upsize - rows "<<rows;
    //match colums
   for(i=0;i<rows;i++) {
            row=matrix.at(i);
            if (row) {
                columns=row->size();
                for(j=columns;j<newcol; j++) {
                    if (j<newcol) {
 //                       qDebug()<<"MatrixData::upsize - column "<<newcol<<" at"<<j;
                        row->insert(j,item);
                    }
                }
            }
            else {
                qDebug()<<"MatrixData::upsize  - Null row!";
            }
    }
}

//get all columns to same size by padding it zero items
void MatrixData::matchsize() {

    QList<itemData> * row;
    int rows=0;
    int maxrows=0;
    int columns=0;
    int maxcolumns=0;
    int i;
    int j;

    qDebug()<<"MatrixData::matchsize";

    itemData item;
    rows = matrix.size();

    for(i=0;i<rows;i++) {
        columns=matrix.at(i)->size();
        if (maxcolumns<columns)
            maxcolumns=columns;
    }
  qDebug()<<"MatrixData::matchsize - maxcolumns "<<maxcolumns;
    for(i=0;i<rows;i++) {
        columns=matrix.at(i)->size();
        if (columns<maxcolumns) {
            row=matrix.at(i);
            for(j=columns;j<maxcolumns; j++) {
                row->insert(j,item);
            }
        }
    }
}

void MatrixData::clear()
{
    int i;
    int r;
    QList<itemData> * row;

    r=rows();

    for(i=0; i<r;i++) {
        row=matrix.at(i);
        if (row) {
            row->clear();
            delete row;
        }
    }
    matrix.clear();
}

int MatrixData::columns()
{
    QList<itemData> * row;

    if (matrix.size()>0) {
        row=matrix.at(0);
        if (row) {
            qDebug()<<"MatrixData: column "<<row->size();
            return row->size();
        }
    }
    return 0;
}

int MatrixData::rows()
{
   qDebug()<<"MatrixData: row"<<matrix.size();
    matrix.size();
}

MatrixData::~MatrixData()
{
    qDebug()<<"Entering ~MatrixData()";
    clear();
}
