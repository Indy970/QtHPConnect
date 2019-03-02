#ifndef MATRIXDATA_H
#define MATRIXDATA_H

#include <QList>

struct itemData {
    double dValue=0.0;
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
    int columns();
    void resize();
    ~ MatrixData();
};

#endif // MATRIXDATA_H
