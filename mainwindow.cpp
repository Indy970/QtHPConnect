#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QtWidgets/QApplication>
#include <QtWidgets/QTableView>
#include <QMessageBox>

#include <QFile>
#include <QTextStream>

#include "hpusb.h"
#include "datamodel.h"
#include "treemodel.h"
#include "variableview.h"
#include "texteditor.h"
#include "hp_mdiwindow.h"
#include "hptreeitem.h"
#include "hpdata.h"
#include "hp_mdivariableedit.h"
#include "hp_mditexteditor.h"
#include <QInputDialog>

errorHandler *main_err;
#define log(a) main_err->error(L7,0,QString(a),QString());

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //setup
    QCoreApplication::setOrganizationName("Private");
    QCoreApplication::setOrganizationDomain("");
    QCoreApplication::setApplicationName("Linux HP Prime Interface");
    QSettings appSettings;

    //error handler
    main_err = new errorHandler(this);

    //data models
    myModel= new dataModel(0);
    hpTreeModel = new treeModel(0);

    qDebug()<<"1";

    //usbapi
    hpapi = new hpusb();

    qDebug()<<"2";
    //Interface
    ui->setupUi(this);
    setWindowIcon(QIcon::fromTheme("accessories-calculator",
                                   QIcon(":/icons/monitor_32x32.png")));
    createActions();
    createLogWindow();

    //setup trees
    ui->tvCalculators->setModel(hpTreeModel);
    ui->tvCalculators->show();
    QItemSelectionModel *selectionModel= ui->tvCalculators->selectionModel();

    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(onOpen()));
    connect(ui->actionAbout_HP_Connect,SIGNAL(triggered()),this,SLOT(about()));
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(exit()));
    connect(ui->actionContent,SIGNAL(triggered()),this,SLOT(showContent()));
    connect(ui->actionCalculators,SIGNAL(triggered()),this,SLOT(showCalculator()));
    connect(ui->actionMessages,SIGNAL(triggered()),this,SLOT(showMessage()));
    connect(ui->actionMonitor,SIGNAL(triggered()),this,SLOT(showMonitor()));
    connect(ui->tvCalculators,SIGNAL(clicked(QModelIndex)),this,SLOT(clickedCalculator(QModelIndex)));
    connect(ui->actionLog,SIGNAL(triggered()),this,SLOT(createLogWindow()));
    connect(ui->actionTest,SIGNAL(triggered()),this,SLOT(testFunction()));

    //default data
    log("Initialising....");

    if (hpapi->hp_init())
        err(L1,0,QString().sprintf("%s Failed to open libusb",__FUNCTION__));

    ui->dwMessenger->hide();
    ui->dwMonitor->hide();

    addDummy();
  //  openHP();
}

void MainWindow::testFunction() {

    hpCalcData * pH;

    int cmd;

    pH=getTreeModel()->getCalculator("IAN");
    if (pH) {

        cmd   = QInputDialog::getInt(this,"Get Command","CMD:",0,0,256);
        log("command is "+QString().sprintf("%x",cmd));
        pH->vpkt_send_experiments(cmd);
    }
    else
        log("Could not get calculator");

}

treeModel * MainWindow::getTreeModel() {
    return hpTreeModel;
}

void MainWindow::writeStatus(QString msg)
{
    statusBar()->showMessage(msg);
    return;
}

void MainWindow::writeChatter(QString line)
{
    if (logEdit)
    {
           logEdit->append(line);
    }
    return;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onOpen()
{
    qDebug()<<"MainWindow::in on OPen";

//test to see if data model works
    hp_Information hpi;
    hpi.serialnum="123-56";
    QString key;
    key=hpTreeModel->getLastDataKey();
    hpCalcData * hpdata;

    qDebug()<<"MainWindow:: getKey";

    hpdata=hpTreeModel->getHpCalcData(key);
    if(hpdata)
        hpdata->setInfo(hpi);

    openHP();
}

//Experimental
void MainWindow::loadTextFile()
{
    QFile inputFile(":/input.txt");
    inputFile.open(QIODevice::ReadOnly);

    QTextStream in(&inputFile);
    QString line = in.readAll();
    inputFile.close();

}

//open the first calculator
//rework, find multiple or allow choice
void MainWindow::openHP()
{
     hpCalcData  * data;

     qDebug()<<"openHP";
        QString name1 = QString("IAN");
        data=hpTreeModel->getCalculator(name1);
        if(data) {
                 qDebug()<<"Read Info";
            data->readInfo();
        }
        else {
            qDebug()<<"In open Func";
            hpTreeModel->addCalculator(name1,hpapi);
            //handle lost on second call!
 //           hpapi->submit_sync_transfer(&handle);
        }
}

//testcode to add a dummy calculator to the tree
void MainWindow::addDummy() {

    QString name = QString("DUMMY");
    hpTreeModel->addCalculator(name,nullptr);
}

//slot to handle tree selection
void MainWindow::selectionChangedSlot(const QItemSelection & /*newSelection*/, const QItemSelection & /*oldSelection*/)
{
    //get the text of the selected item
    const QModelIndex index = ui->tvCalculators->selectionModel()->currentIndex();

 //   hpTreeItem * item = (hpTreeItem *)index.internalPointer();

//    if(item) {
 //           if (item->isSelectable())
  //              item->hptSelection(5);
  //  };


   // hpTreeItem hpt= ui->tvCalculators->selectionModel()->
    QString selectedText = index.data(Qt::DisplayRole).toString();         

    //find out the hierarchy level of the selected item
    int hierarchyLevel=1;
    QModelIndex seekRoot = index;
    while(seekRoot.parent() != QModelIndex())
    {
        seekRoot = seekRoot.parent();
        hierarchyLevel++;
    }
    QString showString = QString("%1, Level %2").arg(selectedText)
                         .arg(hierarchyLevel);
    setWindowTitle(showString);
}

void MainWindow::clickedCalculator(QModelIndex index) {

    hp_mdiVariableEdit * hpvaredit;

    QStandardItem * item = hpTreeModel->itemFromIndex(index);

    hpTreeItem * treeItem = dynamic_cast<hpTreeItem *>(hpTreeModel->itemFromIndex(index));

    if(treeItem) {

      DataType treetype;
      treetype=treeItem->getType();

      switch (treetype) {
       case HP_APP:
          break;
       case HP_CAS:

          break;
      }
      treeItem->clickAction(getMdi());

    }
    else
    {
        log(QStringLiteral("treeItem is null"));
    }
    log(item->data(Qt::DisplayRole).toString());

}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Application"),
            tr("The <b>Application</b> example demonstrates how to "
               "write modern GUI applications using Qt, with a menu bar, "
               "toolbars, and a status bar."));
}

//Dummy from examples -- edit
void MainWindow::createActions()
{

  //  ui->toolBar->addWidget(QPushButton(QIcon(":/icons/about_32x32.png"),"test"));
//    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/icons/new.png"));
//    QAction *newAct = new QAction(newIcon, tr("&New"), this);

};

//show or hide content window
void MainWindow::showContent() {

    if (ui->dwContent_2->isVisible()) {
        ui->dwContent_2->hide();
    }
    else
    {
        ui->dwContent_2->show();
    }
}

//show or hide calculator window
void MainWindow::showCalculator() {

    if (ui->dwCalculator->isVisible()) {
        ui->dwCalculator->hide();
    }
    else
    {
        ui->dwCalculator->show();
    }
}

//show or hide message window
void MainWindow::showMessage() {

    if (ui->dwMessenger->isVisible()) {
        ui->dwMessenger->hide();
    }
    else
    {
        ui->dwMessenger->show();
    }
}

//show or hide message window
void MainWindow::showMonitor() {

    if (ui->dwMonitor->isVisible()) {
        ui->dwMonitor->hide();
    }
    else
    {
        ui->dwMonitor->show();
    }
}

void MainWindow::exit() {
    close();
}

//opens a new test window
//todo - link to hpdata
void MainWindow::createTextWindow() {

    msgWindow = new QMdiSubWindow(ui->mdiArea);
    QTextEdit * textWindow = new QTextEdit("hell",msgWindow);
}

//opens the log window
void MainWindow::createLogWindow() {

    if (!logWindow)
        logWindow = new hp_MdiWindow(ui->mdiArea);
    else
        logWindow->show();
   if (!logEdit)
        logEdit = logWindow->getEditor();
}

//returns the multi document area
QMdiArea * MainWindow::getMdi() {
    return ui->mdiArea;
}
