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

#ifndef MATRIXDATA_H
#define MATRIXDATA_H

#include <QList>
#include <QString>


struct itemData {
    double dReal=0.0;
    double dImaginary=0.0;
    QByteArray buf_real[16];
    QByteArray buf_imag[16];
    QString sValue=QStringLiteral("");
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
    bool dataOut(QDataStream &out);
    void resize(int,int);
    void upsize(int,int);
    void matchsize();
    ~ MatrixData();
};

#endif // MATRIXDATA_H
