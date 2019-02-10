#ifndef HPTOOLBOX_H
#define HPTOOLBOX_H

#include <QToolBox>

namespace Ui {
class hpToolBox;
}

class hpToolBox : public QToolBox
{
    Q_OBJECT

public:
    explicit hpToolBox(QWidget *parent = 0);
    ~hpToolBox();

private:
    Ui::hpToolBox *ui;
};

#endif // HPTOOLBOX_H
