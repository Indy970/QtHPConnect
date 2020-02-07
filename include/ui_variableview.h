/********************************************************************************
** Form generated from reading UI file 'variableview.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VARIABLEVIEW_H
#define UI_VARIABLEVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_variableView
{
public:
    QVBoxLayout *verticalLayout;
    QTableView *variableList;

    void setupUi(QWidget *variableView)
    {
        if (variableView->objectName().isEmpty())
            variableView->setObjectName(QString::fromUtf8("variableView"));
        variableView->resize(347, 358);
        verticalLayout = new QVBoxLayout(variableView);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        variableList = new QTableView(variableView);
        variableList->setObjectName(QString::fromUtf8("variableList"));
        variableList->setEnabled(true);

        verticalLayout->addWidget(variableList);


        retranslateUi(variableView);

        QMetaObject::connectSlotsByName(variableView);
    } // setupUi

    void retranslateUi(QWidget *variableView)
    {
        variableView->setWindowTitle(QApplication::translate("variableView", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class variableView: public Ui_variableView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VARIABLEVIEW_H
