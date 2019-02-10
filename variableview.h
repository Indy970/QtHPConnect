#ifndef VARIABLEVIEW_H
#define VARIABLEVIEW_H

#include <QWidget>
#include <QTableView>

namespace Ui {
class variableView;
}

class variableView : public QTableView
{
    Q_OBJECT

public:
    explicit variableView(QWidget *parent = 0);
    ~variableView();

private:
    Ui::variableView *ui;
};

#endif // VARIABLEVIEW_H
