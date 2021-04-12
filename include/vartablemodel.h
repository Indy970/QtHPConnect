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

#include <QtWidgets>
#include <QAbstractTableModel>
#include <QStringList>
#include <QFileInfo>
#include <QDir>

#ifndef VARTABLEMODEL_H
#define VARTABLEMODEL_H

#include "hpdata.h"
#include "abstractdata.h"

class varTableModel: public QAbstractTableModel
{
    Q_OBJECT

private:
    QObject * q_parent;
    const static QStringList real_header;
    const static QStringList complex_header;
    hpCalcData * hpcalc =nullptr;
    QString filename;
    hp_DataType type;
    AbstractData * dataobj =nullptr;
    QList<QList<double>> dataarray;
    void setup();
    bool ismodified=false;
    bool isUntitled;
    QDir defaultPath;


public:
    varTableModel(QObject *parent = nullptr,
                  AbstractData * data =nullptr,
                  QString file = QStringLiteral(""),
                  hp_DataType dtype = HP_MAIN);
    QModelIndex parent(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool getData(  QDataStream &ds);
    bool isModified();
    void resetModified();

    QVariant  headerData(int section, Qt::Orientation orientation, int role) const override;
    ~varTableModel() override;
};

#endif // VARTABLEMODEL_H
