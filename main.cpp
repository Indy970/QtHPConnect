#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qthpconnect);

    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("");
    QCoreApplication::setApplicationName("Linux QtHP Connect");
    QCoreApplication::setApplicationVersion(HP_VERSION_STRING);
    QCommandLineParser parser;

    parser.setApplicationDescription(QCoreApplication::applicationName());
    //REWORK
    parser.addHelpOption();
    parser.addVersionOption();
//    parser.addPositionalArgument("file", "The file to open.");
    parser.process(a);

    MainWindow w;
//    if (!parser.positionalArguments().isEmpty())
//        w.loadFile(parser.positionalArguments().first());
    w.show();

    return a.exec();
}
