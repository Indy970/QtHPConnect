/********************************************************************************
** Form generated from reading UI file 'hp_infodialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HP_INFODIALOG_H
#define UI_HP_INFODIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_hp_infoDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QFormLayout *formLayout;
    QLabel *labelapp;
    QLabel *appver;
    QLabel *labelopp;
    QLabel *oppver;
    QLabel *labelser;
    QLabel *serialnum;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *hp_infoDialog)
    {
        if (hp_infoDialog->objectName().isEmpty())
            hp_infoDialog->setObjectName(QString::fromUtf8("hp_infoDialog"));
        hp_infoDialog->resize(411, 143);
        verticalLayout = new QVBoxLayout(hp_infoDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(hp_infoDialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelapp = new QLabel(frame);
        labelapp->setObjectName(QString::fromUtf8("labelapp"));

        formLayout->setWidget(0, QFormLayout::LabelRole, labelapp);

        appver = new QLabel(frame);
        appver->setObjectName(QString::fromUtf8("appver"));

        formLayout->setWidget(0, QFormLayout::FieldRole, appver);

        labelopp = new QLabel(frame);
        labelopp->setObjectName(QString::fromUtf8("labelopp"));

        formLayout->setWidget(1, QFormLayout::LabelRole, labelopp);

        oppver = new QLabel(frame);
        oppver->setObjectName(QString::fromUtf8("oppver"));

        formLayout->setWidget(1, QFormLayout::FieldRole, oppver);

        labelser = new QLabel(frame);
        labelser->setObjectName(QString::fromUtf8("labelser"));

        formLayout->setWidget(2, QFormLayout::LabelRole, labelser);

        serialnum = new QLabel(frame);
        serialnum->setObjectName(QString::fromUtf8("serialnum"));

        formLayout->setWidget(2, QFormLayout::FieldRole, serialnum);


        horizontalLayout->addLayout(formLayout);


        verticalLayout->addWidget(frame);

        buttonBox = new QDialogButtonBox(hp_infoDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(hp_infoDialog);
        QObject::connect(buttonBox, SIGNAL(rejected()), hp_infoDialog, SLOT(reject()));
        QObject::connect(buttonBox, SIGNAL(accepted()), hp_infoDialog, SLOT(accept()));

        QMetaObject::connectSlotsByName(hp_infoDialog);
    } // setupUi

    void retranslateUi(QDialog *hp_infoDialog)
    {
        hp_infoDialog->setWindowTitle(QApplication::translate("hp_infoDialog", "HP Information", nullptr));
        labelapp->setText(QApplication::translate("hp_infoDialog", "Application version:", nullptr));
        appver->setText(QApplication::translate("hp_infoDialog", "-", nullptr));
        labelopp->setText(QApplication::translate("hp_infoDialog", "Operational system version:", nullptr));
        oppver->setText(QApplication::translate("hp_infoDialog", "-", nullptr));
        labelser->setText(QApplication::translate("hp_infoDialog", "Serial number:", nullptr));
        serialnum->setText(QApplication::translate("hp_infoDialog", "-", nullptr));
    } // retranslateUi

};

namespace Ui {
    class hp_infoDialog: public Ui_hp_infoDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HP_INFODIALOG_H
