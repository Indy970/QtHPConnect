#ifndef HP_MDIVARIABLEEDIT_H
#define HP_MDIVARIABLEEDIT_H

#include <QObject>
#include <QWidget>
#include <QMdiSubWindow>
#include <QTableView>
#include "vartablemodel.h"
#include "hpdata.h"

class hpTreeItem;

class hp_mdiVariableEdit : public QMdiSubWindow
{
    Q_OBJECT

protected:
        QTableView * tableView =nullptr;
        hpTreeItem * hptreeitem  =nullptr;
        varTableModel * varmodel  =nullptr;
        hpCalcData * hpcalc =nullptr;
        QString filename;
        hp_DataType type;
        void setup();

public:
    explicit hp_mdiVariableEdit(QWidget *parent = nullptr,
                                hpTreeItem * treeItem = nullptr,
                                hpCalcData * dataStore =nullptr
                                );
    void show();
    ~hp_mdiVariableEdit();

signals:

public slots:

};

#endif // HP_MDIVARIABLEEDIT_H
