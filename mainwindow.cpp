/*
 * QtHP Connect: hand-helds support interface.
 * Copyright (C) 2019 Ian Gebbie
 * Code patterns and snippets borrowed from libhpcalcs :
 * Copyright (C) 1999-2009 Romain Li�vin
 * Copyright (C) 2009-2013 Lionel Debroux
 * Copyright (C) 1999-2013 libti* contributors.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QTableView>
#include <QMessageBox>
#include <QTreeView>
#include <QLabel>
#include <QInputDialog>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QFileIconProvider>

#include "global.h"
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
#include "options.h"
#include "eventthread.h"
#include "eventtimer.h"

errorHandler *main_err;
#define log(a) main_err->error(L7,0,QString(a),QString());

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //setup
    QCoreApplication::setOrganizationName("IRGP");
    QCoreApplication::setOrganizationDomain("");
    QCoreApplication::setApplicationName("Linux QtHPConnect");
    QSettings appSettings("IRGP","QtHPconnect");

    //Set config file location (default used)
    if(!appSettings.contains("contentPath")) {
        appSettings.setValue("contentPath",QDir::homePath()+"/.local/share/qthpconnect/contents/");
    }

    //error handler
    main_err = new errorHandler(this);

    //data models
    myModel= new dataModel(this);
    hpTreeModel = new treeModel(this);

    //usbapi
    hpapi = new hpusb();

    //Interface
    ui->setupUi(this);
    setWindowIcon(QIcon::fromTheme("accessories-calculator",
                                   QIcon(":/icons/monitor_32x32.png")));
    createActions();
    createLogWindow();
    setTreeMenu();
    setContentWindow();

    //Hack to fix QT resizing bug
    resizeDocks({ui->dwCalculator,ui->dwContent},{0,0}, Qt::Horizontal);

    //setup trees
    ui->tvCalculators->setModel(hpTreeModel);
    ui->tvCalculators->setAcceptDrops(true);
    ui->tvCalculators->setDragEnabled(true);
    ui->tvCalculators->setDragDropMode(QAbstractItemView::DragDrop);
    ui->tvCalculators->setDropIndicatorShown(true);
    ui->tvCalculators->show();





    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(onOpen()));
    connect(ui->actionAbout_HP_Connect,SIGNAL(triggered()),this,SLOT(about()));
  //  connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(exit()));
    connect(ui->actionContent,SIGNAL(triggered()),this,SLOT(showContent()));
    connect(ui->actionCalculators,SIGNAL(triggered()),this,SLOT(showCalculator()));
    connect(ui->actionMessages,SIGNAL(triggered()),this,SLOT(showMessage()));
    connect(ui->actionMonitor,SIGNAL(triggered()),this,SLOT(showMonitor()));
    connect(ui->tvCalculators,SIGNAL(clicked(QModelIndex)),this,SLOT(clickedCalculator(QModelIndex)));
    connect(ui->tvContent,SIGNAL(clicked(QModelIndex)),this,SLOT(clickedContent(QModelIndex)));
    connect(ui->actionLog,SIGNAL(triggered()),this,SLOT(createLogWindow()));
    connect(ui->actionTest,SIGNAL(triggered()),this,SLOT(testFunction()));
    connect(ui->actionTestSettings,SIGNAL(triggered()),this,SLOT(onTestSettings()));
    connect(ui->actionTestScreen,SIGNAL(triggered()),this,SLOT(onTestScreen()));
    connect(ui->actionRefresh,SIGNAL(triggered(bool)),this,SLOT(refresh(bool)));
    connect(ui->actionPreferences,SIGNAL(triggered(bool)),this,SLOT(onOptions(bool)));
    connect(ui->tvCalculators, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(on_tvCalculators_customContextMenuRequested(const QPoint &)));
    connect(ui->tvContent, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(on_tvContent_customContextMenuRequested(const QPoint &)));
    connect(hpapi, SIGNAL(hotplug(int)), this, SLOT(hotplug_handler(int)));

    //default data
    log("Initialising....");

    if (hpapi->hp_init())
        err(L1,0,QString().sprintf("%s Failed to open libusb",__FUNCTION__));

    //setup event handler
    eventThread = new EventThread(this);
    eventTimer = new EventTimer(this);
    //    connect(eventTimer,SIGNAL(timeout()),this,SLOT(eventHandler()));
    connect(eventThread,SIGNAL(finished()),eventTimer,SLOT(exit()), Qt::DirectConnection);
    connect(eventTimer,SIGNAL(stopped()),this,SLOT(setTimerStopped()), Qt::DirectConnection);
    connect(this,SIGNAL(stopTimer()),eventTimer,SLOT(exit()), Qt::DirectConnection);
    connect(eventThread,SIGNAL(startTimer()),eventTimer,SLOT(start()));
    eventTimer->moveToThread(eventThread);
    eventThread->start();

    ui->dwMessenger->hide();
    ui->dwMonitor->hide();

  //  addDummy();
  //  openHP();
}

void MainWindow::testFunction() {

    hpCalcData * pH;

    int cmd;
    qDebug()<<"In Test Function";
    pH=getTreeModel()->getCalculator("IAN");
    if (pH) {

        cmd   = QInputDialog::getInt(this,"Get Command","CMD:",0,0,0xFFFF);
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

//eventhandler - for comms events
void MainWindow::eventHandler() {

    if(hpapi) {
//        qDebug()<<"In Eventhandler";
//        hpapi->eventHandler();
    }
}

void MainWindow::onOpen()
{
    qDebug()<<"MainWindow::in on Open";

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

void MainWindow::onTestSettings()
{
    qDebug()<<"MainWindow::in test Settings";

    QString key;
    key=hpTreeModel->getLastDataKey();
    qDebug()<<key;
    hpCalcData * hpdata;
    hpdata=hpTreeModel->getHpCalcData(key);
    if(hpdata)
        hpdata->readSettings();
}

void MainWindow::onTestScreen()
{
    qDebug()<<"MainWindow::in test Screen";

    QString key;
    key=hpTreeModel->getLastDataKey();
    hpCalcData * hpdata;
    qDebug()<<"MainWindow:: getKey";

    hpdata=hpTreeModel->getHpCalcData(key);
    if(hpdata)
        hpdata->readScreen();
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
            data->readSettings();
        }
        else {
            qDebug()<<"In open Func";
            hpTreeModel->addCalculator(name1,hpapi);
            //handle lost on second call!
 //           hpapi->submit_sync_transfer(&handle);
        }
}

//close and delete HP and associated functions.
void MainWindow::closeHP() {

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

    QStandardItem * item = hpTreeModel->itemFromIndex(index);

    hpTreeItem * treeItem = dynamic_cast<hpTreeItem *>(hpTreeModel->itemFromIndex(index));

    if(treeItem) {

      hp_DataType treetype;
      treetype=treeItem->getType();
      treeItem->clickAction(getMdi());

    }
    else
    {
        log(QStringLiteral("treeItem is null"));
    }

    //HACK
    lastCalc=item->data(Qt::DisplayRole).toString();
    log(item->data(Qt::DisplayRole).toString());
}

void MainWindow::clickedContent(QModelIndex index) {

   contentModel.clickAction(getMdi(),index);
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About QtHP Connect"),
                      QString("QtHp Connect is an interface for the HP Prime G2 Calculator\n\n")+QString("Version: ")+HP_VERSION_STRING);

}

//Show options window
void MainWindow::onOptions(bool clicked)
{
    Options * optiondlg = new Options(this);
    optiondlg->show();
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

    if (ui->dwContent->isVisible()) {
        ui->dwContent->hide();
    }
    else
    {
        ui->dwContent->show();
    }
}

//Setup the content window to show saved content
void MainWindow::setContentWindow() {

    QSettings appSettings("IRGP","QtHPconnect");
    QString path;

    path=appSettings.value("contentPath").toString();
    qDebug()<<"Content Path:"<<path;

    contentModel.setRootPath(path);
  //  contentModel.iconProvider()->setOptions(QFileIconProvider::DontUseCustomDirectoryIcons);

    ui->tvContent->setModel(&contentModel);

    QDir dir(path);

    if (!dir.exists()) {
        qDebug()<<"Content Path Does not Exist:"<<path;
        if(!dir.mkpath("."))
        {
           qDebug()<<"Path could not be created"<<path;
        }
    }

    if (!path.isEmpty()) {
        qDebug()<<"Content Path Empty:"<<path;
        const QModelIndex rootIndex = contentModel.index(QDir::cleanPath(path));
        if (rootIndex.isValid()) {
            ui->tvContent->setRootIndex(rootIndex);
            ui->tvContent->setAcceptDrops(true);
            ui->tvContent->setDragEnabled(true);
            ui->tvContent->setDragDropMode(QAbstractItemView::DragDrop);
            ui->tvContent->setDropIndicatorShown(true);

        }
    }
    readSettings();
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

// slot to process low level changes that affect the main Window
void MainWindow::dataChange(hp_Change hpchange) {

    hpCalcData * ptr=nullptr;
   qDebug()<<"MainWindow Datachange";
    switch (hpchange.dataChange) {
            case HP_MAIN:

                break;
            case HP_SCREEN: {
                 if (hpchange.calc!=nullptr) {
                    hp_ScreenShot scrn;
                    scrn = hpchange.calc->getScreenShot();
                    monitorAddImage(scrn);
                 }
                }
            break;
            default:;
    }
}

//Add screen shots to the message window
void MainWindow::monitorAddImage(hp_ScreenShot  scrnshot) {

      QPixmap * pic;
      int col;
      int row;
      int count;
      int maxcol=2; //number of images in a column

     if (scrnshot.image!=nullptr) {

         //Todo fix default image
         pic=new QPixmap(*scrnshot.image);
//         pic->scaled(200, 200,  Qt::KeepAspectRatio,Qt::SmoothTransformation);

         QLabel * label = new QLabel("Screenshot");
         label->setPixmap(*pic);
         row = ui->wMonitorGrid->rowCount();
         col = ui->wMonitorGrid->columnCount();
         count = ui->wMonitorGrid->count();

         col=count%maxcol;
         row=count/maxcol;

//         qDebug()<<"Row set"<<row;
//         qDebug()<<"Column set"<<col;
         ui->wMonitorGrid->addWidget(label,row,col,Qt::AlignTop);
     }
     else
     {
         log("Could not load image");
     }
     ui->dwMonitor->show();
}

void MainWindow::setTimerStopped() {

    qDebug()<<"MainWindow:: set timerStopped Flag";
    timerStopped=1;
    eventThread->exit();
    close();
}


void MainWindow::writeSettings()
{
    QSettings appSettings("IRGP","QtHPconnect");
}

void MainWindow::readSettings()
{
    QSettings appSettings("IRGP","QtHPconnect");
    const QByteArray geometry = appSettings.value("geometry", QByteArray()).toByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
}

hpusb * MainWindow::getAPI() {
    return hpapi;
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

//action on refresh button
void MainWindow::refresh(bool clicked) {
    qDebug()<<"MainWindow:: Refresh";

    QString key;
    key=hpTreeModel->getLastDataKey();
    hpCalcData * hpdata;
    hpdata=hpTreeModel->getHpCalcData(key);
    if(hpdata)
        hpdata->refresh();
}

//handle hotplug actions
void MainWindow::hotplug_handler(int status) {
    qDebug()<<"MainWindow::hotplugin_handler";

    if (status==HP_OPEN_DEVICE) {
        openHP();
    }

    if (status==HP_CLOSE_DEVICE) {

        //TODO
    }
}

void MainWindow::setTreeMenu() {

    treeMenu = new QMenu(ui->tvCalculators); // add menu items
    treeMenu->addAction(ui->actionSettings);
    treeMenu->addAction(ui->actionRefresh);
    ui->tvCalculators->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tvContent->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->actionSettings, SIGNAL(triggered(bool)),
        this, SLOT(treeMenuAction(bool)));
    connect(ui->actionRefresh, SIGNAL(triggered(bool)),
        this, SLOT(refresh(bool)));


    treeMenu1 = new QMenu(ui->tvCalculators);
    treeMenu1->addAction(ui->actionCalcOpen);
    treeMenu1->addAction(ui->actionCalcDelete);
    treeMenu1->addAction(ui->actionCalcRename);

    treeMenu2 = new QMenu(ui->tvCalculators);
    treeMenu2->addAction(ui->actionCalcOpen);
    treeMenu2->addAction(ui->actionCalcDelete);

    connect(ui->actionCalcOpen, SIGNAL(triggered(bool)),
        this, SLOT(treeOpenAction(bool)));
    connect(ui->actionCalcDelete, SIGNAL(triggered(bool)),
        this, SLOT(treeDeleteAction(bool)));
    connect(ui->actionCalcRename, SIGNAL(triggered(bool)),
        this, SLOT(treeRenameAction(bool)));


    fileMenu1 = new QMenu(ui->tvContent);
    fileMenu1->addAction(ui->actionFileOpen);
    fileMenu1->addAction(ui->actionFileDelete);
    fileMenu1->addAction(ui->actionFileRename);

    fileMenu2 = new QMenu(ui->tvContent);
    fileMenu2->addAction(ui->actionFileOpen);
    fileMenu2->addAction(ui->actionFileDelete);

    connect(ui->actionFileOpen, SIGNAL(triggered(bool)),
        this, SLOT(contentOpenAction(bool)));
    connect(ui->actionFileRename, SIGNAL(triggered(bool)),
        this, SLOT(contentRenameAction(bool)));
    connect(ui->actionFileDelete, SIGNAL(triggered(bool)),
        this, SLOT(contentDeleteAction(bool)));

}

void MainWindow::treeMenuAction(bool clicked) {

    qDebug()<<"MainWindow::treeMenuAction";

    QPoint pos;
    pos=ui->actionSettings->data().toPoint();

    QModelIndex index = ui->tvCalculators->indexAt(pos);
    if (index.isValid()) {
        hpTreeItem * treeItem = dynamic_cast<hpTreeItem *>(hpTreeModel->itemFromIndex(index));
        if(treeItem) {
          hp_DataType treetype;
          treetype=treeItem->getType();
          switch (treetype) {
           case HP_MAIN:
               treeItem->contextAction(getMdi(),CT_PREFERENCE);
              break;
           default:;
          }
        }
        else
        {
            log(QStringLiteral("treeItem is null"));
        }
    }
}


void MainWindow::treeOpenAction(bool clicked) {
      qDebug()<<"MainWindow::treeOpenAction";

      QPoint pos;
      pos=ui->actionCalcOpen->data().toPoint();
      QModelIndex index = ui->tvCalculators->indexAt(pos);

      if (index.isValid()) {
            hpTreeModel->openFile(getMdi(),index);
      }
       else {
       }
       return;

}

void MainWindow::treeRenameAction(bool clicked) {
  qDebug()<<"MainWindow::treeRenameAction";

  QPoint pos;
  pos=ui->actionCalcRename->data().toPoint();
  QModelIndex index = ui->tvCalculators->indexAt(pos);

  QString newName=QStringLiteral("New");


  if (index.isValid()) {

  //    QFileInfo fileinfo = contentModel.fileInfo(index);

  //    newName=fileinfo.fileName();

      bool ok;
      newName = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                                       tr("File name:"), QLineEdit::Normal,
                                                       newName, &ok);

     if (ok && !newName.isEmpty()) {
        hpTreeModel->renameFile(index,newName);
     }
   }
   else {
   }
   return;

}

void MainWindow::treeDeleteAction(bool clicked) {
  qDebug()<<"MainWindow::treeDeleteAction";

  QPoint pos;
  pos=ui->actionCalcDelete->data().toPoint();
  QModelIndex index = ui->tvCalculators->indexAt(pos);
  QMessageBox::StandardButton reply;

  if (index.isValid()) {

      QString msg("Are you sure you want to delete file ");

      reply = QMessageBox::question(this, "Delete File", msg,
                                     QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
            hpTreeModel->deleteFile(index);
      }
  }
   else {
   }
   return;

}

void MainWindow::contentMenuAction(bool clicked) {
   qDebug()<<"MainWindow::contentMenuAction";

}

void MainWindow::contentOpenAction(bool clicked) {
   qDebug()<<"MainWindow::contentOpenAction";

   QPoint pos;
   pos=ui->actionFileOpen->data().toPoint();
   QModelIndex index = ui->tvContent->indexAt(pos);

   if (index.isValid()) {
        contentModel.clickAction(getMdi(),index);
   }
    else {
    }
    return;
}

void MainWindow::contentRenameAction(bool clicked) {
   qDebug()<<"MainWindow::contentRenameAction";

   QPoint pos;
   pos=ui->actionFileRename->data().toPoint();
   QModelIndex index = ui->tvContent->indexAt(pos);

   QString newName;

   if (index.isValid()) {
   QFileInfo fileinfo = contentModel.fileInfo(index);

   newName=fileinfo.fileName();

   bool ok;
   newName = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                                    tr("File name:"), QLineEdit::Normal,
                                                    newName, &ok);
   if (ok && !newName.isEmpty()) {
           contentModel.renameFile(index,newName);
        }
    }
    else {
    }

    return;

}

void MainWindow::contentDeleteAction(bool clicked) {
   qDebug()<<"MainWindow::contentDeleteAction";

   QPoint pos;
   pos=ui->actionFileDelete->data().toPoint();
   QModelIndex index = ui->tvContent->indexAt(pos);

   QMessageBox::StandardButton reply;

   if (index.isValid()) {
    QFileInfo fileinfo = contentModel.fileInfo(index);
    QString msg("Are you sure you want to delete file ");

    msg=msg+fileinfo.fileName()+".";

    reply = QMessageBox::question(this, "Delete File", msg,
                                   QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
           contentModel.deleteFile(index);
       }
   }
   else {
     }

   return;
}

void MainWindow::on_tvCalculators_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->tvCalculators->indexAt(pos);
    if (index.isValid()) {

        ui->actionCalcOpen->setData(QVariant(pos));
        ui->actionCalcDelete->setData(QVariant(pos));
        ui->actionCalcRename->setData(QVariant(pos));

        hpTreeItem * treeItem = dynamic_cast<hpTreeItem *>(hpTreeModel->itemFromIndex(index));
        if(treeItem) {
          hp_DataType treetype;
          treetype=treeItem->getType();
          switch (treetype) {
         //only show menu on main
          case HP_MAIN: {
              if(treeMenu) {
                  ui->actionPreferences->setData(QVariant(pos));
                  treeMenu->exec(ui->tvCalculators->viewport()->mapToGlobal(pos));
                }
              }
              break;
          case HP_PROG:
          case HP_NOTE:{
              if(treeMenu1) {
                  ui->actionPreferences->setData(QVariant(pos));
                  treeMenu1->exec(ui->tvCalculators->viewport()->mapToGlobal(pos));
                  treeMenu1->hide();
              }
              }
              break;
          case HP_MATRIX:
          case HP_LIST:{
              if(treeMenu2) {
                  ui->actionPreferences->setData(QVariant(pos));
                  treeMenu2->exec(ui->tvCalculators->viewport()->mapToGlobal(pos));
                }
              }
              break;
          default:;
          }
          }
    }
}

void MainWindow::on_tvContent_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->tvContent->indexAt(pos);

    hp_DataStruct filedata;
    if (index.isValid()) {

        QFileInfo info = contentModel.fileInfo(index);
        ui->actionFileOpen->setData(QVariant(pos));
        ui->actionFileDelete->setData(QVariant(pos));
        ui->actionFileRename->setData(QVariant(pos));
//        qDebug()<<"MainWindow::on_tvContent_customContextMenuRequested";
        filedata=contentModel.getFileType(info);
        switch (filedata.type) {
            case HP_PROG:
            case HP_APP:
            case HP_NOTE:
            case HP_VAR: {
                 fileMenu1->exec(ui->tvContent->viewport()->mapToGlobal(pos));
                 fileMenu1->hide();
                 break;
            }
            case HP_CAS:
            case HP_MAIN:
            case HP_MATRIX:
            case HP_LIST:
            case HP_COMPLEX:
            case HP_SCREEN:
            case HP_REAL: {
                  fileMenu2->exec(ui->tvContent->viewport()->mapToGlobal(pos));
            }
        }

    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{

    qDebug()<<"MainWindow:: closeEvent Step 1";
    writeSettings();

    //stop the timer pulse
    emit stopTimer();

    eventThread->quit();
    eventThread->terminate();
    eventThread->wait();

    event->accept();

    qDebug()<<"MainWindow:: closeEvent Step 2";
}


//destructor
MainWindow::~MainWindow()
{

 //   qDebug()<<"MainWindow:: closeEvent Step 4";

   if (myModel!=nullptr) {
        delete myModel;
        myModel=nullptr;
    }

   //   qDebug()<<"MainWindow:: closeEvent Step 3";

      ui->tvCalculators->close();
      ui->tvContent->close();
      ui->dwContent->close();
      ui->dwMonitor->close();
      ui->dwMessenger->close();
      ui->dwCalculator->close();

      //  qDebug()<<"MainWindow:: closeEvent Step 5";
      if (main_err!=nullptr) {
          delete main_err;
          main_err=nullptr;
      }

    qDebug()<<"MainWindow:: closing";
}
