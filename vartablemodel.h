#ifndef VARTABLEMODEL_H
#define VARTABLEMODEL_H

#include <QAbstractTableModel>

class varTableModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    varTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif // VARTABLEMODEL_H
