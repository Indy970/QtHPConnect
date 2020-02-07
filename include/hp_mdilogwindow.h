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

#ifndef HP_MDILOGWINDOW_H
#define HP_MDILOGWINDOW_H

#include <QObject>
#include <QWidget>
#include <QMdiSubWindow>
#include "hp_mdiwindow.h"

#include "texteditor.h"

class hp_MdiLogWindow: public hp_MdiWindow
{
    Q_OBJECT

protected:

    QTextEdit * textEdit;

public:
    hp_MdiLogWindow(QWidget * parent);
    void setup();
    QTextEdit * getEditor();
    void show();
    ~hp_MdiLogWindow();

};

#endif // HP_MDILOGWINDOW_H
