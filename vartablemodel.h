#ifndef VARTABLEMODEL_H
#define VARTABLEMODEL_H

#include <QAbstractTableModel>

#include "hpdata.h"
#include "abstractdata.h"

class varTableModel: public QAbstractTableModel
{
    Q_OBJECT

private:
    hpCalcData * hpcalc =nullptr;
    QString filename;
    hp_DataType type;
    AbstractData * dataobj =nullptr;
    QList<QList<double>> dataarray;
    void setup();

public:
    varTableModel(QObject *parent = nullptr,
                  hpCalcData * dataStore =nullptr,
                  QString file = QStringLiteral(""),
                  hp_DataType dtype = HP_MAIN);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif // VARTABLEMODEL_H
