#include "hp_settingsdlg.h"
#include "ui_hp_settingsdlg.h"

#include <QDebug>
#include <QDialogButtonBox>

hp_SettingsDlg::hp_SettingsDlg(QWidget *parent, hp_Settings * hpset) :
    QDialog(parent),
    ui(new Ui::hp_SettingsDlg)
{
    if (hpset) {
        dlgset=*hpset;
        retsettings=hpset;
    }
    ui->setupUi(this);
    setupSettings();
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(on_clicked(QAbstractButton*)));
}

void hp_SettingsDlg::setupSettings() {
    ui->cb_angle->setCurrentIndex(dlgset.angle_measure);
    ui->cb_format->setCurrentIndex(dlgset.number_format);
    ui->cb_precision->setCurrentIndex(dlgset.precision);
    ui->cb_grouping->setCurrentIndex(dlgset.digit_grouping);
    ui->cb_entry->setCurrentIndex(dlgset.entry);
    ui->cb_integers->setCurrentIndex(dlgset.integers);
    ui->cb_complex->setCurrentIndex(dlgset.complex);
    ui->cb_language->setCurrentIndex(dlgset.language);
    ui->cb_font->setCurrentIndex(dlgset.font_size);
    ui->cb_theme->setCurrentIndex(dlgset.theme);

    ui->le_integer->setText(QString().sprintf("%d",dlgset.bits));

    if (dlgset.textbook)
            ui->ch_textbook->setCheckState(Qt::Checked);
    else
            ui->ch_textbook->setCheckState(Qt::Unchecked);

    if (dlgset.menu)
            ui->ch_menu->setCheckState(Qt::Checked);
    else
            ui->ch_menu->setCheckState(Qt::Unchecked);

    if (dlgset.signed_int)
            ui->ch_signed->setCheckState(Qt::Checked);
    else
            ui->ch_signed->setCheckState(Qt::Unchecked);

}

void hp_SettingsDlg::putSettings() {
    dlgset.angle_measure=ui->cb_angle->currentIndex();
    dlgset.number_format=ui->cb_format->currentIndex();
    dlgset.precision=ui->cb_precision->currentIndex();
    dlgset.digit_grouping= ui->cb_grouping->currentIndex();
    dlgset.entry=ui->cb_entry->currentIndex();
    dlgset.integers=ui->cb_integers->currentIndex();
    dlgset.complex=ui->cb_complex->currentIndex();
    dlgset.language=ui->cb_language->currentIndex();
    dlgset.font_size=ui->cb_font->currentIndex();
    dlgset.theme=ui->cb_theme->currentIndex();

    dlgset.bits=ui->le_integer->text().toInt();

    if (ui->ch_menu->checkState()==Qt::Checked)
            dlgset.menu=true;
    else
            dlgset.menu=false;

    if (ui->ch_textbook->checkState()==Qt::Checked)
            dlgset.textbook=true;
    else
            dlgset.textbook=false;

    if (ui->ch_signed->checkState()==Qt::Checked)
            dlgset.signed_int=true;
    else
            dlgset.signed_int=false;
}

void hp_SettingsDlg::resetSettings() {
    hp_Settings newset;
    dlgset = newset;
    setupSettings();
}

void hp_SettingsDlg::on_clicked(QAbstractButton * button) {

    if(button==(QAbstractButton*)  ui->buttonBox->button(QDialogButtonBox::Reset) ){
        resetSettings();
     }
    if(button==(QAbstractButton*) ui->buttonBox->button(QDialogButtonBox::Ok) ){
        putSettings();
        if (retsettings) {
            *retsettings=dlgset;
        }
        close();
     }
    if(button==(QAbstractButton*)  ui->buttonBox->button(QDialogButtonBox::Cancel) ){
        close();
     }
}

hp_SettingsDlg::~hp_SettingsDlg()
{
    qDebug()<<"delete hp_SettingsDlf";
    delete ui;
}
