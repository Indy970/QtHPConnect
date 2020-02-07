/*
 * This file is part of the QtHPConnect distribution (https://github.com/Indy970/QtHPConnect.git).
 * Copyright (c) 2020 Ian Gebbie.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 or later.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
