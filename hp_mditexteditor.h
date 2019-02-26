#ifndef HP_MDITEXTEDITOR_H
#define HP_MDITEXTEDITOR_H

#include <QObject>
#include <QWidget>
#include <QMdiSubWindow>
#include <QTreeView>
#include "hpdata.h"

class hpTreeItem;
class hpCalcData;

#include "texteditor.h"

class hp_mdiTextEdit: public QMdiSubWindow
{
    Q_OBJECT

protected:

    textEditor * textEdit  =nullptr;
    hpTreeItem * hptreeitem =nullptr;
    hpCalcData * hpcalc =nullptr;
    QString filename;
    hp_DataType type;
    AbstractData * data =nullptr;

public:
    hp_mdiTextEdit(QWidget * parent = nullptr, hpTreeItem * treeItem = nullptr,
                   hpCalcData * dataStore =nullptr);
    void setup();
    void show();
    ~hp_mdiTextEdit();

signals:

public slots:

};

#endif // HP_MDITEXTEDITOR_H
