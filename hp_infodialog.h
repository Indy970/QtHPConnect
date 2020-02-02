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
