#ifndef VARIABLEVIEW_H
#define VARIABLEVIEW_H

#include <QWidget>

namespace Ui {
class variableView;
}

class variableView : public QWidget
{
    Q_OBJECT

public:
    explicit variableView(QWidget *parent = 0);
    ~variableView();

private:
    Ui::variableView *ui;
};

#endif // VARIABLEVIEW_H
