#include "options.h"
#include "ui_options.h"
#include "global.h"
#include <QSettings>


Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
    QString path;

    QSettings appSettings("IRGP","QtHPconnect");
    path=appSettings.value("contentPath").toString();
    ui->filePath->setText(path);
}

void Options::accept() {

    QString path;
    path=ui->filePath->text();
    QSettings appSettings("IRGP","QtHPconnect");
    appSettings.setValue("contentPath",path);
    QDialog::accept();
}

void Options::reject() {
    QDialog::reject();
}

Options::~Options()
{
    qDebug()<<"~Options()";
    delete ui;
}
