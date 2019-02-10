#ifndef HP_MDIVARIABLEEDIT_H
#define HP_MDIVARIABLEEDIT_H

#include <QObject>
#include <QWidget>
#include <QMdiSubWindow>
#include <QTableView>
#include "vartablemodel.h"

class hpTreeItem;

class hp_mdiVariableEdit : public QMdiSubWindow
{
    Q_OBJECT

protected:
        QTableView * tableView;
        hpTreeItem * hptreeitem;
        varTableModel * varmodel;

public:
    explicit hp_mdiVariableEdit(QWidget *parent = nullptr, hpTreeItem * treeItem = nullptr);
    void setup();
    void show();
    ~hp_mdiVariableEdit();

signals:

public slots:

};

#endif // HP_MDIVARIABLEEDIT_H
