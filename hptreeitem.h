#ifndef HPTREEITEM_H
#define HPTREEITEM_H

#include <QStandardItem>
#include <QMenu>
#include <QAction>
#include <QString>
#include "hpdata.h"
#include "mainwindow.h"

class hp_mdiVariableEdit;
class hp_mdiTextEdit;

enum DataType;

enum contextActionType {
            CT_PREFERENCE
};

class hpTreeItem: public QObject, public QStandardItem
{
    Q_OBJECT

public:

    hpTreeItem();
    hpTreeItem(const QString &,hpCalcData *, int);
    virtual QStandardItem *clone() const;
    void setGraphicTree();
    void setType(DataType);
    DataType getType();
    void clickAction(QMdiArea * mwin);
    void contextAction(QMdiArea * mdiwin, contextActionType cta );
    void setDataStore(hpCalcData *);
    hpCalcData *  getDataStore();
    QString getName();

public slots:
    void dataChange(hp_Change hpchange);

private:
    const static QString func_list[][2];
    const static DataType func_type[];
    DataType type;
    hpCalcData * hp_data=nullptr;
    QMdiArea * mdiarea=nullptr;
    hp_mdiVariableEdit * hpvaredit = nullptr;
    hp_mdiTextEdit * hptextedit = nullptr;
    int dt2int();

signals:
    void dataChanged(hp_Change datachange);

};

Q_DECLARE_METATYPE(hpTreeItem *)

#endif // HPTREEITEM_H
