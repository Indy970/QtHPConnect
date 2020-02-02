/*
 * This file is part of the QtHPConnect distribution (https://github.com/Indy970/QtHPConnect.git).
 * Copyright (c) 2020 Ian Gebbie.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 or later.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#include "texteditor.h"
#include <QtWidgets>
#include <QRect>
#include <QSettings>

textEditor::textEditor(QWidget *parent) :
    QTextEdit(parent)
{
    QSettings appSettings("IRGP","QtHPconnect");
    wParent = parent;
    setAttribute(Qt::WA_DeleteOnClose);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    isUntitled = true;

    defaultPath=QDir(appSettings.value("contentPath").toString());
    qDebug()<<defaultPath;
}

void textEditor::newFile()
{
    static int sequenceNumber = 1;

    isUntitled = true;
    curFile = tr("document%1.txt").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");

    connect(document(), &QTextDocument::contentsChanged,
            this, &textEditor::documentWasModified);
}

bool textEditor::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("MDI"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);

    connect(document(), &QTextDocument::contentsChanged,
            this, &textEditor::documentWasModified);

    return true;
}

bool textEditor::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool textEditor::saveAs()
{
    QFileInfo fileinfo(defaultPath,curFile);

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    fileinfo.absoluteFilePath());
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool textEditor::saveFile(const QString &fileName)
{
    QFileInfo fileinfo(defaultPath,fileName);
    QFile file(fileinfo.absoluteFilePath());
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("MDI"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << toPlainText();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    return true;
}

QString textEditor::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}

void textEditor::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void textEditor::documentWasModified()
{
    setWindowModified(document()->isModified());
}

bool textEditor::maybeSave()
{
    if (!document()->isModified())
        return true;

    const QMessageBox::StandardButton ret
            = QMessageBox::warning(this, tr("MDI"),
                                   tr("'%1' has been modified.\n"
                                      "Do you want to save your changes?")
                                   .arg(userFriendlyCurrentFile()),
                                   QMessageBox::Save | QMessageBox::Discard
                                   | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

void textEditor::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString textEditor::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

textEditor::~textEditor() {
    qDebug()<<"textEditor:: delete";
}
