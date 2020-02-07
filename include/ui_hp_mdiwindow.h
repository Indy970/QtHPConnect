/********************************************************************************
** Form generated from reading UI file 'hp_mdiwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HP_MDIWINDOW_H
#define UI_HP_MDIWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_hp_MdiWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit;

    void setupUi(QMainWindow *hp_MdiWindow)
    {
        if (hp_MdiWindow->objectName().isEmpty())
            hp_MdiWindow->setObjectName(QString::fromUtf8("hp_MdiWindow"));
        hp_MdiWindow->setEnabled(true);
        hp_MdiWindow->resize(575, 320);
        hp_MdiWindow->setMinimumSize(QSize(400, 200));
        centralwidget = new QWidget(hp_MdiWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        verticalLayout->addWidget(textEdit);

        hp_MdiWindow->setCentralWidget(centralwidget);

        retranslateUi(hp_MdiWindow);

        QMetaObject::connectSlotsByName(hp_MdiWindow);
    } // setupUi

    void retranslateUi(QMainWindow *hp_MdiWindow)
    {
        hp_MdiWindow->setWindowTitle(QApplication::translate("hp_MdiWindow", "LogWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class hp_MdiWindow: public Ui_hp_MdiWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HP_MDIWINDOW_H
