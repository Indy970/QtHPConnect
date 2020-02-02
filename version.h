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

#ifndef VERSION_H
#define VERSION_H

#define HP_VERSION_MAJOR 0
#define HP_VERSION_MINOR 1
#define HP_VERSION_PATCH 0
#define HP_VERSION_BUILD ""
#define HP_VERSION_STRING QString("%1.%2.%3%4").arg(HP_VERSION_MAJOR).arg(HP_VERSION_MINOR).arg(HP_VERSION_PATCH).arg(HP_VERSION_BUILD)

#endif // VERSION_H
