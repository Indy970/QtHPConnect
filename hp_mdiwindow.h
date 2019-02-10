#ifndef HP_MDIWINDOW_H
#define HP_MDIWINDOW_H

#include <QObject>
#include <QWidget>
#include <QMdiSubWindow>

#include "texteditor.h"

class hp_MdiWindow: public QMdiSubWindow
{
    Q_OBJECT

protected:

    QTextEdit * textEdit;

public:
    hp_MdiWindow(QWidget * parent);
    void setup();
    QTextEdit * getEditor();
    void show();
    ~hp_MdiWindow();

};

#endif // HP_MDIWINDOW_H
