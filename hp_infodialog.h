#ifndef HP_INFODIALOG_H
#define HP_INFODIALOG_H

#include <QFrame>

namespace Ui {
class hp_infoDialog;
}

class hp_infoDialog : public QFrame
{
    Q_OBJECT

public:
    explicit hp_infoDialog(QWidget *parent = 0);
    ~hp_infoDialog();

private:
    Ui::hp_infoDialog *ui;
};

#endif // HP_INFODIALOG_H
