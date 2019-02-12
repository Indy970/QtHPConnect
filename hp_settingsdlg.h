#ifndef DLGSETTINGS_H
#define DLGSETTINGS_H

#include <QDialog>

namespace Ui {
class dlgSettings;
}

class dlgSettings : public QDialog
{
    Q_OBJECT

public:
    explicit dlgSettings(QWidget *parent = 0);
    ~dlgSettings();

private:
    Ui::dlgSettings *ui;
};

#endif // DLGSETTINGS_H
