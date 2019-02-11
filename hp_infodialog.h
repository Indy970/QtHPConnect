#ifndef HP_INFODIALOG_H
#define HP_INFODIALOG_H

#include <QDialog>
#include "hpdata.h"

namespace Ui {
class hp_infoDialog;
}

class hp_infoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit hp_infoDialog(QWidget *parent, hp_Information info );
    ~hp_infoDialog();

private:
    Ui::hp_infoDialog *ui;
};

#endif // HP_INFODIALOG_H
