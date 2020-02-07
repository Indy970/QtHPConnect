/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionClose;
    QAction *actionClose_all;
    QAction *actionTile;
    QAction *actionCascade;
    QAction *actionNext;
    QAction *actionMessages;
    QAction *actionMonitor;
    QAction *actionCalculators;
    QAction *actionAbout_HP_Connect;
    QAction *actionPreferences;
    QAction *actionPrevious;
    QAction *actionContent;
    QAction *actionSave;
    QAction *actionSave_All;
    QAction *actionCreateNew;
    QAction *actionExit;
    QAction *actionOpen;
    QAction *actionLog;
    QAction *actionTest;
    QAction *actionTreePreference;
    QAction *actionTestSettings;
    QAction *actionTestScreen;
    QAction *actionRefresh;
    QAction *actionSettings;
    QAction *actionFileOpen;
    QAction *actionFileDelete;
    QAction *actionFileRename;
    QAction *actionCalcOpen;
    QAction *actionCalcDelete;
    QAction *actionCalcRename;
    QAction *actionNewFolder;
    QAction *actionNew_Note;
    QAction *actionNew_Program;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QMdiArea *mdiArea;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuWindow;
    QMenu *menuHelp;
    QDockWidget *dwCalculator;
    QWidget *wCalculator;
    QVBoxLayout *verticalLayout_3;
    QTreeView *tvCalculators;
    QToolBar *toolBar;
    QDockWidget *dwContent;
    QWidget *wContent;
    QVBoxLayout *verticalLayout_2;
    QTreeView *tvContent;
    QDockWidget *dwMessenger;
    QWidget *wMessage;
    QVBoxLayout *verticalLayout;
    QTextEdit *teMessage;
    QDockWidget *dwMonitor;
    QWidget *wMonitor;
    QVBoxLayout *verticalLayout_4;
    QGridLayout *wMonitorGrid;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(919, 584);
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("cut");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8(":/icons/cut_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        }
        actionCut->setIcon(icon);
        QFont font;
        actionCut->setFont(font);
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        QIcon icon1;
        iconThemeName = QString::fromUtf8("action_copy");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon1 = QIcon::fromTheme(iconThemeName);
        } else {
            icon1.addFile(QString::fromUtf8(":/icons/copy_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        }
        actionCopy->setIcon(icon1);
        actionCopy->setFont(font);
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/paste_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaste->setIcon(icon2);
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        actionClose_all = new QAction(MainWindow);
        actionClose_all->setObjectName(QString::fromUtf8("actionClose_all"));
        actionTile = new QAction(MainWindow);
        actionTile->setObjectName(QString::fromUtf8("actionTile"));
        actionCascade = new QAction(MainWindow);
        actionCascade->setObjectName(QString::fromUtf8("actionCascade"));
        actionNext = new QAction(MainWindow);
        actionNext->setObjectName(QString::fromUtf8("actionNext"));
        actionMessages = new QAction(MainWindow);
        actionMessages->setObjectName(QString::fromUtf8("actionMessages"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/chat_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMessages->setIcon(icon3);
        actionMonitor = new QAction(MainWindow);
        actionMonitor->setObjectName(QString::fromUtf8("actionMonitor"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/monitor_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMonitor->setIcon(icon4);
        actionCalculators = new QAction(MainWindow);
        actionCalculators->setObjectName(QString::fromUtf8("actionCalculators"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/calc_tree_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCalculators->setIcon(icon5);
        actionAbout_HP_Connect = new QAction(MainWindow);
        actionAbout_HP_Connect->setObjectName(QString::fromUtf8("actionAbout_HP_Connect"));
        QIcon icon6;
        iconThemeName = QString::fromUtf8("about");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon6 = QIcon::fromTheme(iconThemeName);
        } else {
            icon6.addFile(QString::fromUtf8(":/icons/about_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        }
        actionAbout_HP_Connect->setIcon(icon6);
        actionAbout_HP_Connect->setFont(font);
        actionPreferences = new QAction(MainWindow);
        actionPreferences->setObjectName(QString::fromUtf8("actionPreferences"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icons/preferences_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPreferences->setIcon(icon7);
        actionPreferences->setFont(font);
        actionPrevious = new QAction(MainWindow);
        actionPrevious->setObjectName(QString::fromUtf8("actionPrevious"));
        actionContent = new QAction(MainWindow);
        actionContent->setObjectName(QString::fromUtf8("actionContent"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/icons/content_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionContent->setIcon(icon8);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/icons/save_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon9);
        actionSave_All = new QAction(MainWindow);
        actionSave_All->setObjectName(QString::fromUtf8("actionSave_All"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/icons/save_all_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_All->setIcon(icon10);
        actionCreateNew = new QAction(MainWindow);
        actionCreateNew->setObjectName(QString::fromUtf8("actionCreateNew"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/icons/add_new_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCreateNew->setIcon(icon11);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/icons/delete_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon12);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/icons/add_icon_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon13);
        actionLog = new QAction(MainWindow);
        actionLog->setObjectName(QString::fromUtf8("actionLog"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/icons/note_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLog->setIcon(icon14);
        actionTest = new QAction(MainWindow);
        actionTest->setObjectName(QString::fromUtf8("actionTest"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/icons/clone_to_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTest->setIcon(icon15);
        actionTreePreference = new QAction(MainWindow);
        actionTreePreference->setObjectName(QString::fromUtf8("actionTreePreference"));
        actionTreePreference->setIcon(icon7);
        actionTestSettings = new QAction(MainWindow);
        actionTestSettings->setObjectName(QString::fromUtf8("actionTestSettings"));
        actionTestSettings->setIcon(icon7);
        actionTestScreen = new QAction(MainWindow);
        actionTestScreen->setObjectName(QString::fromUtf8("actionTestScreen"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/icons/add_background_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTestScreen->setIcon(icon16);
        actionRefresh = new QAction(MainWindow);
        actionRefresh->setObjectName(QString::fromUtf8("actionRefresh"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/icons/refresh_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRefresh->setIcon(icon17);
        actionSettings = new QAction(MainWindow);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        actionSettings->setIcon(icon7);
        actionFileOpen = new QAction(MainWindow);
        actionFileOpen->setObjectName(QString::fromUtf8("actionFileOpen"));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/icons/open_16x16.png"), QSize(), QIcon::Normal, QIcon::On);
        actionFileOpen->setIcon(icon18);
        actionFileDelete = new QAction(MainWindow);
        actionFileDelete->setObjectName(QString::fromUtf8("actionFileDelete"));
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/icons/delete_16x16.png"), QSize(), QIcon::Normal, QIcon::On);
        actionFileDelete->setIcon(icon19);
        actionFileRename = new QAction(MainWindow);
        actionFileRename->setObjectName(QString::fromUtf8("actionFileRename"));
        QIcon icon20;
        icon20.addFile(QString::fromUtf8(":/icons/rename_16x16.png"), QSize(), QIcon::Normal, QIcon::On);
        actionFileRename->setIcon(icon20);
        actionCalcOpen = new QAction(MainWindow);
        actionCalcOpen->setObjectName(QString::fromUtf8("actionCalcOpen"));
        actionCalcOpen->setIcon(icon18);
        actionCalcDelete = new QAction(MainWindow);
        actionCalcDelete->setObjectName(QString::fromUtf8("actionCalcDelete"));
        actionCalcDelete->setIcon(icon19);
        actionCalcRename = new QAction(MainWindow);
        actionCalcRename->setObjectName(QString::fromUtf8("actionCalcRename"));
        actionCalcRename->setIcon(icon20);
        actionNewFolder = new QAction(MainWindow);
        actionNewFolder->setObjectName(QString::fromUtf8("actionNewFolder"));
        QIcon icon21;
        icon21.addFile(QString::fromUtf8(":/icons/new_folder_16x16.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNewFolder->setIcon(icon21);
        actionNew_Note = new QAction(MainWindow);
        actionNew_Note->setObjectName(QString::fromUtf8("actionNew_Note"));
        actionNew_Program = new QAction(MainWindow);
        actionNew_Program->setObjectName(QString::fromUtf8("actionNew_Program"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        mdiArea = new QMdiArea(centralWidget);
        mdiArea->setObjectName(QString::fromUtf8("mdiArea"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mdiArea->sizePolicy().hasHeightForWidth());
        mdiArea->setSizePolicy(sizePolicy);
        mdiArea->setMinimumSize(QSize(500, 0));
        mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        horizontalLayout->addWidget(mdiArea);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 919, 23));
        QFont font1;
        font1.setPointSize(10);
        menuBar->setFont(font1);
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuWindow = new QMenu(menuBar);
        menuWindow->setObjectName(QString::fromUtf8("menuWindow"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        dwCalculator = new QDockWidget(MainWindow);
        dwCalculator->setObjectName(QString::fromUtf8("dwCalculator"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(dwCalculator->sizePolicy().hasHeightForWidth());
        dwCalculator->setSizePolicy(sizePolicy1);
        dwCalculator->setMinimumSize(QSize(200, 184));
        dwCalculator->setMaximumSize(QSize(524287, 524287));
        dwCalculator->setBaseSize(QSize(300, 200));
        wCalculator = new QWidget();
        wCalculator->setObjectName(QString::fromUtf8("wCalculator"));
        sizePolicy1.setHeightForWidth(wCalculator->sizePolicy().hasHeightForWidth());
        wCalculator->setSizePolicy(sizePolicy1);
        wCalculator->setMinimumSize(QSize(100, 150));
        verticalLayout_3 = new QVBoxLayout(wCalculator);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tvCalculators = new QTreeView(wCalculator);
        tvCalculators->setObjectName(QString::fromUtf8("tvCalculators"));
        tvCalculators->setEnabled(true);
        sizePolicy1.setHeightForWidth(tvCalculators->sizePolicy().hasHeightForWidth());
        tvCalculators->setSizePolicy(sizePolicy1);
        tvCalculators->setContextMenuPolicy(Qt::ActionsContextMenu);
        tvCalculators->setLineWidth(2);
        tvCalculators->setMidLineWidth(1);
        tvCalculators->header()->setVisible(false);

        verticalLayout_3->addWidget(tvCalculators);

        dwCalculator->setWidget(wCalculator);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dwCalculator);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        dwContent = new QDockWidget(MainWindow);
        dwContent->setObjectName(QString::fromUtf8("dwContent"));
        sizePolicy1.setHeightForWidth(dwContent->sizePolicy().hasHeightForWidth());
        dwContent->setSizePolicy(sizePolicy1);
        dwContent->setMinimumSize(QSize(300, 130));
        dwContent->setMaximumSize(QSize(524287, 524287));
        wContent = new QWidget();
        wContent->setObjectName(QString::fromUtf8("wContent"));
        sizePolicy1.setHeightForWidth(wContent->sizePolicy().hasHeightForWidth());
        wContent->setSizePolicy(sizePolicy1);
        verticalLayout_2 = new QVBoxLayout(wContent);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tvContent = new QTreeView(wContent);
        tvContent->setObjectName(QString::fromUtf8("tvContent"));
        tvContent->setSelectionBehavior(QAbstractItemView::SelectItems);
        tvContent->setIndentation(15);
        tvContent->header()->setCascadingSectionResizes(true);
        tvContent->header()->setMinimumSectionSize(47);
        tvContent->header()->setDefaultSectionSize(150);
        tvContent->header()->setHighlightSections(true);
        tvContent->header()->setStretchLastSection(false);

        verticalLayout_2->addWidget(tvContent);

        dwContent->setWidget(wContent);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dwContent);
        dwMessenger = new QDockWidget(MainWindow);
        dwMessenger->setObjectName(QString::fromUtf8("dwMessenger"));
        sizePolicy1.setHeightForWidth(dwMessenger->sizePolicy().hasHeightForWidth());
        dwMessenger->setSizePolicy(sizePolicy1);
        dwMessenger->setMaximumSize(QSize(150, 524287));
        wMessage = new QWidget();
        wMessage->setObjectName(QString::fromUtf8("wMessage"));
        sizePolicy1.setHeightForWidth(wMessage->sizePolicy().hasHeightForWidth());
        wMessage->setSizePolicy(sizePolicy1);
        verticalLayout = new QVBoxLayout(wMessage);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        teMessage = new QTextEdit(wMessage);
        teMessage->setObjectName(QString::fromUtf8("teMessage"));

        verticalLayout->addWidget(teMessage);

        dwMessenger->setWidget(wMessage);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dwMessenger);
        dwMonitor = new QDockWidget(MainWindow);
        dwMonitor->setObjectName(QString::fromUtf8("dwMonitor"));
        sizePolicy1.setHeightForWidth(dwMonitor->sizePolicy().hasHeightForWidth());
        dwMonitor->setSizePolicy(sizePolicy1);
        wMonitor = new QWidget();
        wMonitor->setObjectName(QString::fromUtf8("wMonitor"));
        sizePolicy1.setHeightForWidth(wMonitor->sizePolicy().hasHeightForWidth());
        wMonitor->setSizePolicy(sizePolicy1);
        verticalLayout_4 = new QVBoxLayout(wMonitor);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        wMonitorGrid = new QGridLayout();
        wMonitorGrid->setSpacing(6);
        wMonitorGrid->setObjectName(QString::fromUtf8("wMonitorGrid"));

        verticalLayout_4->addLayout(wMonitorGrid);

        dwMonitor->setWidget(wMonitor);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dwMonitor);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(100);
        sizePolicy2.setVerticalStretch(20);
        sizePolicy2.setHeightForWidth(statusBar->sizePolicy().hasHeightForWidth());
        statusBar->setSizePolicy(sizePolicy2);
        statusBar->setMinimumSize(QSize(100, 20));
        statusBar->setAutoFillBackground(true);
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuWindow->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuEdit->addSeparator();
        menuEdit->addAction(actionPreferences);
        menuWindow->addAction(actionClose);
        menuWindow->addAction(actionClose_all);
        menuWindow->addSeparator();
        menuWindow->addAction(actionTile);
        menuWindow->addAction(actionCascade);
        menuWindow->addSeparator();
        menuWindow->addAction(actionNext);
        menuWindow->addAction(actionPrevious);
        menuWindow->addSeparator();
        menuWindow->addAction(actionMessages);
        menuWindow->addAction(actionMonitor);
        menuWindow->addAction(actionCalculators);
        menuWindow->addAction(actionContent);
        menuWindow->addAction(actionLog);
        menuHelp->addAction(actionAbout_HP_Connect);
        toolBar->addAction(actionCalculators);
        toolBar->addAction(actionContent);
        toolBar->addAction(actionSave);
        toolBar->addAction(actionSave_All);
        toolBar->addAction(actionCreateNew);
        toolBar->addSeparator();
        toolBar->addAction(actionMonitor);
        toolBar->addAction(actionMessages);
        toolBar->addSeparator();
        toolBar->addAction(actionOpen);
        toolBar->addSeparator();
        toolBar->addAction(actionTest);
        toolBar->addAction(actionTestSettings);
        toolBar->addAction(actionTestScreen);
        toolBar->addAction(actionRefresh);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Linux HP Connect", nullptr));
        actionCut->setText(QApplication::translate("MainWindow", "&Cut", nullptr));
        actionCopy->setText(QApplication::translate("MainWindow", "C&opy", nullptr));
        actionPaste->setText(QApplication::translate("MainWindow", "&Paste", nullptr));
        actionClose->setText(QApplication::translate("MainWindow", "&Close", nullptr));
        actionClose_all->setText(QApplication::translate("MainWindow", "C&lose all", nullptr));
        actionTile->setText(QApplication::translate("MainWindow", "&Tile", nullptr));
        actionCascade->setText(QApplication::translate("MainWindow", "Ca&scade", nullptr));
        actionNext->setText(QApplication::translate("MainWindow", "&Next", nullptr));
        actionMessages->setText(QApplication::translate("MainWindow", "&Messages", nullptr));
        actionMonitor->setText(QApplication::translate("MainWindow", "M&onitor", nullptr));
        actionCalculators->setText(QApplication::translate("MainWindow", "Calc&ulators", nullptr));
        actionAbout_HP_Connect->setText(QApplication::translate("MainWindow", "&About HP Connect", nullptr));
        actionPreferences->setText(QApplication::translate("MainWindow", "P&references", nullptr));
        actionPrevious->setText(QApplication::translate("MainWindow", "&Previous", nullptr));
        actionContent->setText(QApplication::translate("MainWindow", "Cont&ent", nullptr));
        actionSave->setText(QApplication::translate("MainWindow", "Save", nullptr));
        actionSave_All->setText(QApplication::translate("MainWindow", "Save All", nullptr));
        actionCreateNew->setText(QApplication::translate("MainWindow", "CreateNew", nullptr));
#ifndef QT_NO_TOOLTIP
        actionCreateNew->setToolTip(QApplication::translate("MainWindow", "Create New ", nullptr));
#endif // QT_NO_TOOLTIP
        actionExit->setText(QApplication::translate("MainWindow", "&Exit", nullptr));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", nullptr));
        actionLog->setText(QApplication::translate("MainWindow", "Lo&g", nullptr));
        actionTest->setText(QApplication::translate("MainWindow", "Test", nullptr));
#ifndef QT_NO_TOOLTIP
        actionTest->setToolTip(QApplication::translate("MainWindow", "Test Button", nullptr));
#endif // QT_NO_TOOLTIP
        actionTreePreference->setText(QApplication::translate("MainWindow", "TreePreference", nullptr));
        actionTestSettings->setText(QApplication::translate("MainWindow", "TestSettings", nullptr));
        actionTestScreen->setText(QApplication::translate("MainWindow", "TestScreen", nullptr));
#ifndef QT_NO_TOOLTIP
        actionTestScreen->setToolTip(QApplication::translate("MainWindow", "Test get Screen Function", nullptr));
#endif // QT_NO_TOOLTIP
        actionRefresh->setText(QApplication::translate("MainWindow", "Refresh", nullptr));
        actionSettings->setText(QApplication::translate("MainWindow", "Settings", nullptr));
        actionFileOpen->setText(QApplication::translate("MainWindow", "&Open", nullptr));
#ifndef QT_NO_TOOLTIP
        actionFileOpen->setToolTip(QApplication::translate("MainWindow", "Open a file", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionFileOpen->setShortcut(QApplication::translate("MainWindow", "Alt+O", nullptr));
#endif // QT_NO_SHORTCUT
        actionFileDelete->setText(QApplication::translate("MainWindow", "&Delete", nullptr));
#ifndef QT_NO_TOOLTIP
        actionFileDelete->setToolTip(QApplication::translate("MainWindow", "Delete a file", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionFileDelete->setShortcut(QApplication::translate("MainWindow", "Alt+D", nullptr));
#endif // QT_NO_SHORTCUT
        actionFileRename->setText(QApplication::translate("MainWindow", "&Rename", nullptr));
#ifndef QT_NO_TOOLTIP
        actionFileRename->setToolTip(QApplication::translate("MainWindow", "Rename a file", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionFileRename->setShortcut(QApplication::translate("MainWindow", "Alt+R", nullptr));
#endif // QT_NO_SHORTCUT
        actionCalcOpen->setText(QApplication::translate("MainWindow", "&Open", nullptr));
#ifndef QT_NO_TOOLTIP
        actionCalcOpen->setToolTip(QApplication::translate("MainWindow", "Open a file", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionCalcOpen->setShortcut(QApplication::translate("MainWindow", "Alt+O", nullptr));
#endif // QT_NO_SHORTCUT
        actionCalcDelete->setText(QApplication::translate("MainWindow", "&Delete", nullptr));
#ifndef QT_NO_TOOLTIP
        actionCalcDelete->setToolTip(QApplication::translate("MainWindow", "Delete a file", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionCalcDelete->setShortcut(QApplication::translate("MainWindow", "Alt+D", nullptr));
#endif // QT_NO_SHORTCUT
        actionCalcRename->setText(QApplication::translate("MainWindow", "&Rename", nullptr));
#ifndef QT_NO_TOOLTIP
        actionCalcRename->setToolTip(QApplication::translate("MainWindow", "Rename a file", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionCalcRename->setShortcut(QApplication::translate("MainWindow", "Alt+R", nullptr));
#endif // QT_NO_SHORTCUT
        actionNewFolder->setText(QApplication::translate("MainWindow", "Folder", nullptr));
#ifndef QT_NO_TOOLTIP
        actionNewFolder->setToolTip(QApplication::translate("MainWindow", "Create a New Folder", nullptr));
#endif // QT_NO_TOOLTIP
        actionNew_Note->setText(QApplication::translate("MainWindow", "New Note", nullptr));
        actionNew_Program->setText(QApplication::translate("MainWindow", "New Program", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "Fi&le", nullptr));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", nullptr));
        menuWindow->setTitle(QApplication::translate("MainWindow", "Wi&ndow", nullptr));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", nullptr));
#ifndef QT_NO_TOOLTIP
        tvCalculators->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Calculator View</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", nullptr));
#ifndef QT_NO_TOOLTIP
        tvContent->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>File View</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        statusBar->setToolTip(QString());
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
