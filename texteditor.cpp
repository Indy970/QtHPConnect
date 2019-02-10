#include "texteditor.h"
#include "ui_texteditor.h"

textEditor::textEditor(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::textEditor)
{
    ui->setupUi(this);
}

textEditor::~textEditor()
{
    delete ui;
}
