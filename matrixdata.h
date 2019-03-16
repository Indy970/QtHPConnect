#ifndef MATRIXDATA_H
#define MATRIXDATA_H

#include <QList>

struct itemData {
    double dReal=0.0;
    double dImaginary=0.0;
    QString sValue=QStringLiteral("NaH");
};


class MatrixData
{
private:
    QList<QList<itemData>*> matrix;

public:
    MatrixData();
    itemData at(int,int);
    void insert(int,int,itemData);
    void clear();
    int rows();
    void addRow(int);
    int columns();
    void resize(int,int);
    void upsize(int,int);
    void matchsize();
    ~ MatrixData();
};

#endif // MATRIXDATA_H
