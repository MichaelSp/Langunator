
//#define HANDY


#ifdef HANDY
#include "qtquick2applicationviewer.h"
#include <QtQml/QQmlEngine>
#endif
#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationName("");
    QApplication::setApplicationName("Langunator");

#ifdef HANDY
    QtQuick2ApplicationViewer viewer;
#ifdef Q_OS_ANDROID
    viewer.addImportPath("/imports/");
    viewer.engine()->addPluginPath(QDir::homePath()+"/../lib");
#endif
    viewer.setMainQmlFile(QStringLiteral("src/QML/MainWindow.qml"));
    viewer.showExpanded();
#else
    MainWindow w;
    w.show();
#endif

    return a.exec();
}
