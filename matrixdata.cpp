#include <QDebug>

#include "matrixdata.h"

MatrixData::MatrixData()
{
    clear();
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
    clear();
}
\
