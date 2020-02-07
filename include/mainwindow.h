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

#include <QMainWindow>
#include <QItemSelection>
#include <QModelIndex>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QSettings>
#include <QFileSystemModel>

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "global.h"
#include "errorhandler.h"
#include "hpusb.h"
#include "datamodel.h"
#include "treemodel.h"
#include "texteditor.h"
#include "hp_mdilogwindow.h"
#include "hp_mdivariableedit.h"
#include "cntfilesystemmodel.h"
#include "eventtimer.h"


class treeModel;
class errorHandler;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void openHP();
    void closeHP();
    void writeStatus(QString);
    void writeChatter(QString);
    void addDummy();
    void createActions();
    hpusb * getAPI();

signals:
    void stopTimer();

private slots:
    void onOpen();
    void onTestSettings();
    void onTestScreen();
    void onOptions(bool);
    void selectionChangedSlot(const QItemSelection & /*newSelection*/, const QItemSelection & /*oldSelection*/);
    void about();
    void showContent();
    void showCalculator();
    void showMessage();
    void showMonitor();
    void dataChange(hp_Change);
    void clickedCalculator(QModelIndex);
    void clickedContent(QModelIndex);
    void closeEvent(QCloseEvent *) override;
    void createLogWindow();
    void testFunction();
    void writeSettings();
    void readSettings();
    void setTimerStopped();

    void eventSave();
    void eventSaveAs();
    void eventCreateFolder();
    void eventCreateNote();
    void eventCreateProgram();

    treeModel * getTreeModel();

    void treeMenuAction(bool);
    void treeOpenAction(bool);
    void treeRenameAction(bool);
    void treeDeleteAction(bool);

    void contentMenuAction(bool);
    void contentOpenAction(bool);
    void contentRenameAction(bool);
    void contentDeleteAction(bool);

    hp_MdiWindow * activeMdiChild();

    void on_tvCalculators_customContextMenuRequested(const QPoint &pos);
    void on_tvContent_customContextMenuRequested(const QPoint &pos);
    void eventHandler();
    void refresh(bool clicked);
    void hotplug_handler(int );


private:

    QString lastCalc;
    errorHandler *pErr;
    dataModel * myModel;
    treeModel * hpTreeModel;
    QThread * eventThread;

    EventTimer * eventTimer;
    QMenu * treeMenu=nullptr;
    QMenu * treeMenu1=nullptr;
    QMenu * treeMenu2=nullptr;
    QMenu * fileMenu1=nullptr;
    QMenu * fileMenu2=nullptr;

    hpusb * hpapi=nullptr;
    QMdiSubWindow * msgWindow=nullptr;
    hp_MdiLogWindow * logWindow=nullptr;
    contentFileSystemModel contentModel;
    QTextEdit * logEdit=nullptr;
    Ui::MainWindow *ui;
    void loadTextFile();
    void createTextWindow();
    QMdiArea * getMdi();
    void setContentWindow();
    void setTreeMenu();
    void monitorAddImage(hp_ScreenShot scrnshot);
    int timerStopped=0; //flag
};

#endif
//MAINWINDOW_H
