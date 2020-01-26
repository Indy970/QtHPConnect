#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QTextEdit>


class textEditor : public QTextEdit
{
    Q_OBJECT

public:
    explicit textEditor(QWidget *parent = nullptr);
    ~textEditor() override;
    void newFile();
    bool loadFile(const QString &fileName);
    bool save();
    bool saveAs();
    bool saveFile(const QString &fileName);
    QString userFriendlyCurrentFile();
    QString currentFile() { return curFile; }


protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void documentWasModified();

private:
    QWidget * wParent;
    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QString curFile;
    bool isUntitled;
};

#endif // TEXTEDITOR_H
