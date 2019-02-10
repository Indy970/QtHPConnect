#ifndef GETNUMBER_H
#define GETNUMBER_H

#include <QDialog>

namespace Ui {
class getNumber;
}

class getNumber : public QDialog
{
    Q_OBJECT

public:
    explicit getNumber(QWidget *parent = 0);
    ~getNumber();

private:
    Ui::getNumber *ui;
};

#endif // GETNUMBER_H
