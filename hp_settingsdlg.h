#ifndef DLGSETTINGS_H
#define DLGSETTINGS_H

#include <QDialog>
#include <QAbstractButton>
#include "hpdata.h"

namespace Ui {
class hp_SettingsDlg;
}

class hp_SettingsDlg : public QDialog
{
    Q_OBJECT
    hp_Settings dlgset;
    hp_Settings * retsettings=nullptr;

public:
    explicit hp_SettingsDlg(QWidget *parent, hp_Settings * hpset);
    void setupSettings();
    void putSettings();
    ~hp_SettingsDlg();

public slots:
    void resetSettings();
    void on_clicked(QAbstractButton *);

private:
    Ui::hp_SettingsDlg *ui;
};

#endif // DLGSETTINGS_H
