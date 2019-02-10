#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qthpconnect);

    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationName("Linux HP Connect");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());

    //REWORK
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(a);

    MainWindow w;
//    if (!parser.positionalArguments().isEmpty())
//        w.loadFile(parser.positionalArguments().first());
    w.show();

    return a.exec();
}
