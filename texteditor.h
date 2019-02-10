#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QDockWidget>

namespace Ui {
class textEditor;
}

class textEditor : public QDockWidget
{
    Q_OBJECT

public:
    explicit textEditor(QWidget *parent = 0);
    ~textEditor();

private:
    Ui::textEditor *ui;
};

#endif // TEXTEDITOR_H
