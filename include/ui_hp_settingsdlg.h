/********************************************************************************
** Form generated from reading UI file 'hp_settingsdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HP_SETTINGSDLG_H
#define UI_HP_SETTINGSDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_hp_SettingsDlg
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *hpSettings;
    QLabel *label;
    QComboBox *cb_angle;
    QCheckBox *ch_signed;
    QComboBox *cb_format;
    QComboBox *cb_grouping;
    QLabel *label_2;
    QLabel *label_3;
    QComboBox *cb_entry;
    QLabel *label_4;
    QComboBox *cb_integers;
    QLabel *label_5;
    QLabel *label_6;
    QComboBox *cb_complex;
    QComboBox *cb_language;
    QLabel *label_7;
    QLabel *label_8;
    QComboBox *cb_font;
    QCheckBox *ch_textbook;
    QCheckBox *ch_menu;
    QComboBox *cb_theme;
    QLabel *label_17;
    QComboBox *cb_precision;
    QLineEdit *le_integer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *hp_SettingsDlg)
    {
        if (hp_SettingsDlg->objectName().isEmpty())
            hp_SettingsDlg->setObjectName(QString::fromUtf8("hp_SettingsDlg"));
        hp_SettingsDlg->setEnabled(true);
        hp_SettingsDlg->resize(415, 455);
        hp_SettingsDlg->setFocusPolicy(Qt::StrongFocus);
        verticalLayout = new QVBoxLayout(hp_SettingsDlg);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(hp_SettingsDlg);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        hpSettings = new QWidget();
        hpSettings->setObjectName(QString::fromUtf8("hpSettings"));
        label = new QLabel(hpSettings);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(17, 20, 101, 30));
        label->setLayoutDirection(Qt::RightToLeft);
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        cb_angle = new QComboBox(hpSettings);
        cb_angle->addItem(QString());
        cb_angle->addItem(QString());
        cb_angle->addItem(QString());
        cb_angle->setObjectName(QString::fromUtf8("cb_angle"));
        cb_angle->setGeometry(QRect(130, 20, 131, 32));
        ch_signed = new QCheckBox(hpSettings);
        ch_signed->setObjectName(QString::fromUtf8("ch_signed"));
        ch_signed->setGeometry(QRect(310, 150, 41, 22));
        ch_signed->setLayoutDirection(Qt::RightToLeft);
        cb_format = new QComboBox(hpSettings);
        cb_format->addItem(QString());
        cb_format->addItem(QString());
        cb_format->addItem(QString());
        cb_format->addItem(QString());
        cb_format->addItem(QString());
        cb_format->addItem(QString());
        cb_format->setObjectName(QString::fromUtf8("cb_format"));
        cb_format->setGeometry(QRect(130, 50, 131, 32));
        cb_grouping = new QComboBox(hpSettings);
        cb_grouping->addItem(QString());
        cb_grouping->addItem(QString());
        cb_grouping->addItem(QString());
        cb_grouping->addItem(QString());
        cb_grouping->addItem(QString());
        cb_grouping->addItem(QString());
        cb_grouping->addItem(QString());
        cb_grouping->addItem(QString());
        cb_grouping->addItem(QString());
        cb_grouping->addItem(QString());
        cb_grouping->addItem(QString());
        cb_grouping->setObjectName(QString::fromUtf8("cb_grouping"));
        cb_grouping->setGeometry(QRect(130, 81, 131, 32));
        label_2 = new QLabel(hpSettings);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 50, 101, 30));
        label_2->setLayoutDirection(Qt::RightToLeft);
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_3 = new QLabel(hpSettings);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 80, 101, 30));
        label_3->setLayoutDirection(Qt::RightToLeft);
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        cb_entry = new QComboBox(hpSettings);
        cb_entry->addItem(QString());
        cb_entry->addItem(QString());
        cb_entry->addItem(QString());
        cb_entry->setObjectName(QString::fromUtf8("cb_entry"));
        cb_entry->setGeometry(QRect(130, 112, 131, 32));
        label_4 = new QLabel(hpSettings);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 111, 101, 30));
        label_4->setLayoutDirection(Qt::RightToLeft);
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        cb_integers = new QComboBox(hpSettings);
        cb_integers->addItem(QString());
        cb_integers->addItem(QString());
        cb_integers->addItem(QString());
        cb_integers->addItem(QString());
        cb_integers->setObjectName(QString::fromUtf8("cb_integers"));
        cb_integers->setGeometry(QRect(130, 144, 131, 32));
        label_5 = new QLabel(hpSettings);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 143, 101, 30));
        label_5->setLayoutDirection(Qt::RightToLeft);
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_6 = new QLabel(hpSettings);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 175, 101, 30));
        label_6->setLayoutDirection(Qt::RightToLeft);
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        cb_complex = new QComboBox(hpSettings);
        cb_complex->addItem(QString());
        cb_complex->addItem(QString());
        cb_complex->setObjectName(QString::fromUtf8("cb_complex"));
        cb_complex->setGeometry(QRect(130, 176, 131, 32));
        cb_language = new QComboBox(hpSettings);
        cb_language->addItem(QString());
        cb_language->addItem(QString());
        cb_language->addItem(QString());
        cb_language->addItem(QString());
        cb_language->addItem(QString());
        cb_language->addItem(QString());
        cb_language->addItem(QString());
        cb_language->addItem(QString());
        cb_language->setObjectName(QString::fromUtf8("cb_language"));
        cb_language->setGeometry(QRect(130, 208, 131, 32));
        label_7 = new QLabel(hpSettings);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(20, 207, 101, 30));
        label_7->setLayoutDirection(Qt::RightToLeft);
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_8 = new QLabel(hpSettings);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(20, 239, 101, 30));
        label_8->setLayoutDirection(Qt::RightToLeft);
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        cb_font = new QComboBox(hpSettings);
        cb_font->addItem(QString());
        cb_font->addItem(QString());
        cb_font->addItem(QString());
        cb_font->setObjectName(QString::fromUtf8("cb_font"));
        cb_font->setGeometry(QRect(130, 240, 131, 32));
        ch_textbook = new QCheckBox(hpSettings);
        ch_textbook->setObjectName(QString::fromUtf8("ch_textbook"));
        ch_textbook->setGeometry(QRect(8, 277, 141, 22));
        ch_textbook->setLayoutDirection(Qt::RightToLeft);
        ch_menu = new QCheckBox(hpSettings);
        ch_menu->setObjectName(QString::fromUtf8("ch_menu"));
        ch_menu->setGeometry(QRect(151, 277, 141, 22));
        ch_menu->setLayoutDirection(Qt::RightToLeft);
        cb_theme = new QComboBox(hpSettings);
        cb_theme->addItem(QString());
        cb_theme->addItem(QString());
        cb_theme->setObjectName(QString::fromUtf8("cb_theme"));
        cb_theme->setGeometry(QRect(130, 311, 131, 32));
        label_17 = new QLabel(hpSettings);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(20, 310, 101, 30));
        label_17->setLayoutDirection(Qt::RightToLeft);
        label_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        cb_precision = new QComboBox(hpSettings);
        cb_precision->addItem(QString());
        cb_precision->addItem(QString());
        cb_precision->addItem(QString());
        cb_precision->addItem(QString());
        cb_precision->addItem(QString());
        cb_precision->addItem(QString());
        cb_precision->addItem(QString());
        cb_precision->addItem(QString());
        cb_precision->addItem(QString());
        cb_precision->addItem(QString());
        cb_precision->addItem(QString());
        cb_precision->addItem(QString());
        cb_precision->setObjectName(QString::fromUtf8("cb_precision"));
        cb_precision->setGeometry(QRect(270, 50, 51, 32));
        le_integer = new QLineEdit(hpSettings);
        le_integer->setObjectName(QString::fromUtf8("le_integer"));
        le_integer->setGeometry(QRect(266, 144, 41, 32));
        le_integer->setInputMethodHints(Qt::ImhDigitsOnly);
        tabWidget->addTab(hpSettings, QString());

        verticalLayout->addWidget(tabWidget);

        buttonBox = new QDialogButtonBox(hp_SettingsDlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok|QDialogButtonBox::Reset);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(hp_SettingsDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), hp_SettingsDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), hp_SettingsDlg, SLOT(reject()));

        tabWidget->setCurrentIndex(0);
        cb_angle->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(hp_SettingsDlg);
    } // setupUi

    void retranslateUi(QDialog *hp_SettingsDlg)
    {
        hp_SettingsDlg->setWindowTitle(QApplication::translate("hp_SettingsDlg", "Settings", nullptr));
        label->setText(QApplication::translate("hp_SettingsDlg", "Angle Measure:", nullptr));
        cb_angle->setItemText(0, QApplication::translate("hp_SettingsDlg", "Radians", nullptr));
        cb_angle->setItemText(1, QApplication::translate("hp_SettingsDlg", "Degrees", nullptr));
        cb_angle->setItemText(2, QApplication::translate("hp_SettingsDlg", "Gradians", nullptr));

        cb_angle->setCurrentText(QApplication::translate("hp_SettingsDlg", "Radians", nullptr));
        ch_signed->setText(QApplication::translate("hp_SettingsDlg", "\302\261:", nullptr));
        cb_format->setItemText(0, QApplication::translate("hp_SettingsDlg", "Standard", nullptr));
        cb_format->setItemText(1, QApplication::translate("hp_SettingsDlg", "Fixed", nullptr));
        cb_format->setItemText(2, QApplication::translate("hp_SettingsDlg", "Scientific", nullptr));
        cb_format->setItemText(3, QApplication::translate("hp_SettingsDlg", "Engineering", nullptr));
        cb_format->setItemText(4, QApplication::translate("hp_SettingsDlg", "Floating", nullptr));
        cb_format->setItemText(5, QApplication::translate("hp_SettingsDlg", "Rounded", nullptr));

        cb_format->setCurrentText(QApplication::translate("hp_SettingsDlg", "Standard", nullptr));
        cb_grouping->setItemText(0, QApplication::translate("hp_SettingsDlg", "123,456.789", nullptr));
        cb_grouping->setItemText(1, QApplication::translate("hp_SettingsDlg", "123'456.789", nullptr));
        cb_grouping->setItemText(2, QApplication::translate("hp_SettingsDlg", "123 456.789", nullptr));
        cb_grouping->setItemText(3, QApplication::translate("hp_SettingsDlg", "123456.789", nullptr));
        cb_grouping->setItemText(4, QApplication::translate("hp_SettingsDlg", "123.456,789", nullptr));
        cb_grouping->setItemText(5, QApplication::translate("hp_SettingsDlg", "123'456,789", nullptr));
        cb_grouping->setItemText(6, QApplication::translate("hp_SettingsDlg", "123 456,789", nullptr));
        cb_grouping->setItemText(7, QApplication::translate("hp_SettingsDlg", "123\"456.789", nullptr));
        cb_grouping->setItemText(8, QApplication::translate("hp_SettingsDlg", "123456,789", nullptr));
        cb_grouping->setItemText(9, QApplication::translate("hp_SettingsDlg", "1234,5678.901", nullptr));
        cb_grouping->setItemText(10, QApplication::translate("hp_SettingsDlg", "1,23,456.789", nullptr));

        cb_grouping->setCurrentText(QApplication::translate("hp_SettingsDlg", "123,456.789", nullptr));
        label_2->setText(QApplication::translate("hp_SettingsDlg", "Number Format:", nullptr));
        label_3->setText(QApplication::translate("hp_SettingsDlg", "Digit Grouping:", nullptr));
        cb_entry->setItemText(0, QApplication::translate("hp_SettingsDlg", "Textbook", nullptr));
        cb_entry->setItemText(1, QApplication::translate("hp_SettingsDlg", "Algebraic", nullptr));
        cb_entry->setItemText(2, QApplication::translate("hp_SettingsDlg", "RPN", nullptr));

        cb_entry->setCurrentText(QApplication::translate("hp_SettingsDlg", "Textbook", nullptr));
        label_4->setText(QApplication::translate("hp_SettingsDlg", "Entry:", nullptr));
        cb_integers->setItemText(0, QApplication::translate("hp_SettingsDlg", "Binary", nullptr));
        cb_integers->setItemText(1, QApplication::translate("hp_SettingsDlg", "Octal", nullptr));
        cb_integers->setItemText(2, QApplication::translate("hp_SettingsDlg", "Decimal", nullptr));
        cb_integers->setItemText(3, QApplication::translate("hp_SettingsDlg", "Hex", nullptr));

        cb_integers->setCurrentText(QApplication::translate("hp_SettingsDlg", "Binary", nullptr));
        label_5->setText(QApplication::translate("hp_SettingsDlg", "Integers:", nullptr));
        label_6->setText(QApplication::translate("hp_SettingsDlg", "Complex:", nullptr));
        cb_complex->setItemText(0, QApplication::translate("hp_SettingsDlg", "a+b*i", nullptr));
        cb_complex->setItemText(1, QApplication::translate("hp_SettingsDlg", "(a,b)", nullptr));

        cb_complex->setCurrentText(QApplication::translate("hp_SettingsDlg", "a+b*i", nullptr));
        cb_language->setItemText(0, QApplication::translate("hp_SettingsDlg", "Deutsch", nullptr));
        cb_language->setItemText(1, QApplication::translate("hp_SettingsDlg", "English", nullptr));
        cb_language->setItemText(2, QApplication::translate("hp_SettingsDlg", "Espanol", nullptr));
        cb_language->setItemText(3, QApplication::translate("hp_SettingsDlg", "Francais", nullptr));
        cb_language->setItemText(4, QApplication::translate("hp_SettingsDlg", "Nederlands", nullptr));
        cb_language->setItemText(5, QApplication::translate("hp_SettingsDlg", "Portugues", nullptr));
        cb_language->setItemText(6, QApplication::translate("hp_SettingsDlg", "New Item", nullptr));
        cb_language->setItemText(7, QApplication::translate("hp_SettingsDlg", "New Item", nullptr));

        cb_language->setCurrentText(QApplication::translate("hp_SettingsDlg", "Deutsch", nullptr));
        label_7->setText(QApplication::translate("hp_SettingsDlg", "Language:", nullptr));
        label_8->setText(QApplication::translate("hp_SettingsDlg", "Font Size:", nullptr));
        cb_font->setItemText(0, QApplication::translate("hp_SettingsDlg", "Small Font", nullptr));
        cb_font->setItemText(1, QApplication::translate("hp_SettingsDlg", "Medium Font", nullptr));
        cb_font->setItemText(2, QApplication::translate("hp_SettingsDlg", "Large Font", nullptr));

        cb_font->setCurrentText(QApplication::translate("hp_SettingsDlg", "Small Font", nullptr));
        ch_textbook->setText(QApplication::translate("hp_SettingsDlg", "Textbook Display:", nullptr));
        ch_menu->setText(QApplication::translate("hp_SettingsDlg", "Menu Display:", nullptr));
        cb_theme->setItemText(0, QApplication::translate("hp_SettingsDlg", "Light", nullptr));
        cb_theme->setItemText(1, QApplication::translate("hp_SettingsDlg", "Dark", nullptr));

        cb_theme->setCurrentText(QApplication::translate("hp_SettingsDlg", "Light", nullptr));
        label_17->setText(QApplication::translate("hp_SettingsDlg", "Color Theme:", nullptr));
        cb_precision->setItemText(0, QApplication::translate("hp_SettingsDlg", "0", nullptr));
        cb_precision->setItemText(1, QApplication::translate("hp_SettingsDlg", "1", nullptr));
        cb_precision->setItemText(2, QApplication::translate("hp_SettingsDlg", "2", nullptr));
        cb_precision->setItemText(3, QApplication::translate("hp_SettingsDlg", "3", nullptr));
        cb_precision->setItemText(4, QApplication::translate("hp_SettingsDlg", "4", nullptr));
        cb_precision->setItemText(5, QApplication::translate("hp_SettingsDlg", "5", nullptr));
        cb_precision->setItemText(6, QApplication::translate("hp_SettingsDlg", "6", nullptr));
        cb_precision->setItemText(7, QApplication::translate("hp_SettingsDlg", "7", nullptr));
        cb_precision->setItemText(8, QApplication::translate("hp_SettingsDlg", "8", nullptr));
        cb_precision->setItemText(9, QApplication::translate("hp_SettingsDlg", "9", nullptr));
        cb_precision->setItemText(10, QApplication::translate("hp_SettingsDlg", "10", nullptr));
        cb_precision->setItemText(11, QApplication::translate("hp_SettingsDlg", "11", nullptr));

        cb_precision->setCurrentText(QApplication::translate("hp_SettingsDlg", "0", nullptr));
        le_integer->setText(QApplication::translate("hp_SettingsDlg", "32", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(hpSettings), QApplication::translate("hp_SettingsDlg", "Home Settings", nullptr));
    } // retranslateUi

};

namespace Ui {
    class hp_SettingsDlg: public Ui_hp_SettingsDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HP_SETTINGSDLG_H
