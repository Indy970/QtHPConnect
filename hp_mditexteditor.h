#ifndef HP_MDITEXTEDITOR_H
#define HP_MDITEXTEDITOR_H

#include <QObject>
#include <QWidget>
#include <QMdiSubWindow>
#include <QTreeView>

class hpTreeItem;

#include "texteditor.h"

class hp_mdiTextEdit: public QMdiSubWindow
{
    Q_OBJECT

protected:

    textEditor * textEdit;
    hpTreeItem * hptreeitem;

public:
    hp_mdiTextEdit(QWidget * parent = nullptr, hpTreeItem * treeItem = nullptr);
    void setup();
    void show();
    ~hp_mdiTextEdit();

signals:

public slots:

};

#endif // HP_MDITEXTEDITOR_H
