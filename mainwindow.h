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
#include "hp_mdiwindow.h"
#include "hp_mdivariableedit.h"
#include "cntfilesystemmodel.h"
#include "eventthread.h"

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
    treeModel * getTreeModel();

    void treeMenuAction(bool);
    void on_tvCalculators_customContextMenuRequested(const QPoint &pos);
    void eventHandler();
    void refresh(bool clicked);
    void hotplug_handler(int );

private:

    QString lastCalc;
    errorHandler *pErr;
    dataModel * myModel;
    treeModel * hpTreeModel;
    QThread * eventThread;

    EventThread * eventTimer;
    QMenu * treeMenu=nullptr;
    hpusb * hpapi;
    QMdiSubWindow * msgWindow=nullptr;
    hp_MdiWindow * logWindow=nullptr;
    contentFileSystemModel contentModel;
    QTextEdit * logEdit=nullptr;
    Ui::MainWindow *ui;
    void loadTextFile();
    void createTextWindow();
    QMdiArea * getMdi();
    void setContentWindow();
    void setTreeMenu();
    void monitorAddImage(hp_ScreenShot scrnshot);
};

#endif // MAINWINDOW_H
