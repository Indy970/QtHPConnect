/********************************************************************************
** Form generated from reading UI file 'getnumber.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GETNUMBER_H
#define UI_GETNUMBER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_getNumber
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *lineEdit;
    QPushButton *pushButton;

    void setupUi(QDialog *getNumber)
    {
        if (getNumber->objectName().isEmpty())
            getNumber->setObjectName(QString::fromUtf8("getNumber"));
        getNumber->resize(427, 118);
        buttonBox = new QDialogButtonBox(getNumber);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(0, 70, 411, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        lineEdit = new QLineEdit(getNumber);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(30, 20, 381, 32));
        pushButton = new QPushButton(getNumber);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(30, 70, 88, 34));

        retranslateUi(getNumber);
        QObject::connect(buttonBox, SIGNAL(accepted()), getNumber, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), getNumber, SLOT(reject()));

        QMetaObject::connectSlotsByName(getNumber);
    } // setupUi

    void retranslateUi(QDialog *getNumber)
    {
        getNumber->setWindowTitle(QApplication::translate("getNumber", "Dialog", nullptr));
        pushButton->setText(QApplication::translate("getNumber", "Test", nullptr));
    } // retranslateUi

};

namespace Ui {
    class getNumber: public Ui_getNumber {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GETNUMBER_H
