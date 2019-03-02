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

    if (matrix.size()<row) {
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
    QList<itemData> * rowlist;

    if (matrix.size()<row)
        rowlist=matrix.at(row);
        if (rowlist) {
            if (column<rowlist->size()) {
                rowlist->insert(column,data);
            }
            else {
                resize();
            }
    }
    else {
        QList<itemData> * newrow = new QList<itemData>;
        newrow->insert(column,data);
        matrix.insert(row,newrow);
        resize();
    }
}

//get all columns to same size by padding it zero items
void MatrixData::resize() {

    QList<itemData> * row;
    int rows;
    int maxrows;
    int columns;
    int maxcolumns;
    int i;
    int j;
    itemData item;
    rows = matrix.size();

    for(i=0;i<rows;i++) {
        columns=matrix.at(i)->size();
        if (maxcolumns<columns)
            maxcolumns=columns;
    }

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
            return row->size();
        }
    }
    return 0;
}

int MatrixData::rows()
{
    matrix.size();
}

MatrixData::~MatrixData()
{
    clear();
}
\
