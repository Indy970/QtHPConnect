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

//enum hp_DataType;

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
    void setType(hp_DataType);

    void clickAction(QMdiArea * mwin);
    void contextAction(QMdiArea * mdiwin, contextActionType cta );
    void setDataStore(hpCalcData *);
    hpCalcData *  getDataStore();
    QString getCalculatorName();
    QString getGroupName();
    QString getFileName();
    bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent);
    void setFileName(QString);
    hp_DataType getType();
    void refresh();
    int findFile(QString);
    void addFile(AbstractData *);
    void addChild(AbstractData * obj);
    virtual ~hpTreeItem();

public slots:
    void dataChange(hp_Change hpchange);

private:
    const static QString func_list[][2];
    const static hp_DataType func_type[];
    QString filename;
    hp_DataType type;
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
