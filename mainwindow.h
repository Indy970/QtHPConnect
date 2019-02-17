#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemSelection>
#include <QModelIndex>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QSettings>

#include "global.h"
#include "errorhandler.h"
#include "hpusb.h"
#include "datamodel.h"
#include "treemodel.h"
#include "texteditor.h"
#include "hp_mdiwindow.h"
#include "hp_mdivariableedit.h"

class treeModel;
class errorHandler;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void openHP();
    void writeStatus(QString);
    void writeChatter(QString);
    void addDummy();
    void createActions();

private slots:
    void onOpen();
    void onTestSettings();
    void onTestScreen();
    void selectionChangedSlot(const QItemSelection & /*newSelection*/, const QItemSelection & /*oldSelection*/);
    void about();
    void showContent();
    void showCalculator();
    void showMessage();
    void showMonitor();
    void dataChange(hp_Change);
    void clickedCalculator(QModelIndex);
    void exit();
    void createLogWindow();
    void testFunction();
    treeModel * getTreeModel();
    void setTreeMenu();
    void treeMenuAction(bool);
    void on_tvCalculators_customContextMenuRequested(const QPoint &pos);

private:

    errorHandler *pErr;
    dataModel * myModel;
    treeModel * hpTreeModel;
    QMenu * treeMenu=nullptr;
    hpusb * hpapi;
    QMdiSubWindow * msgWindow=0;
    hp_MdiWindow * logWindow=0;
    QTextEdit * logEdit=0;
    Ui::MainWindow *ui;
    void loadTextFile();
    void createTextWindow();
    QMdiArea * getMdi();
    void monitorAddImage(hp_ScreenShot scrnshot);


};

#endif // MAINWINDOW_H
