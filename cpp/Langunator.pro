#-------------------------------------------------
#
# Project created by QtCreator 2013-05-08T13:41:58
#
#-------------------------------------------------

QT       += core gui sql
CONFIG   += precompile_header

folder_01.source = src/QML
folder_01.target = src
DEPLOYMENTFOLDERS = folder_01

greaterThan(QT_MAJOR_VERSION, 4){
  QT += widgets webkitwidgets
  include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
  qtcAddDeployment()
}
else: QT += webkit

win32:LIBS+= -lUser32

TARGET = Langunator
TEMPLATE = app

PRECOMPILED_HEADER = ./src/pch.h
INCLUDEPATH += ./src/ ./dquest-0.2/src/

ICON = $${PWD}/img/earth.ico
RC_FILE = Langunator.rc

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/db.cpp \
    src/category.cpp \
    dquest-0.2/src/dqwhere.cpp \
    dquest-0.2/src/dqstream.cpp \
    dquest-0.2/src/dqsqlstatement.cpp \
    dquest-0.2/src/dqsqlitestatement.cpp \
    dquest-0.2/src/dqsql.cpp \
    dquest-0.2/src/dqsharedquery.cpp \
    dquest-0.2/src/dqsharedlist.cpp \
    dquest-0.2/src/dqqueryrules.cpp \
    dquest-0.2/src/dqmodelmetainfo.cpp \
    dquest-0.2/src/dqmodel.cpp \
    dquest-0.2/src/dqlistwriter.cpp \
    dquest-0.2/src/dqindex.cpp \
    dquest-0.2/src/dqfield.cpp \
    dquest-0.2/src/dqexpression.cpp \
    dquest-0.2/src/dqconnection.cpp \
    dquest-0.2/src/dqclause.cpp \
    dquest-0.2/src/dqbasefield.cpp \
    dquest-0.2/src/dqabstractmodel.cpp \
    src/Backend.cpp \
    src/VocableList.cpp \
    src/vocabel.cpp \
    src/keyboardlayout.cpp

HEADERS  += src/mainwindow.h \
    src/pch.h \
    src/db.h \
    src/category.h \
    dquest-0.2/src/dqwhere_p.h \
    dquest-0.2/src/dqwhere.h \
    dquest-0.2/src/dquest.h \
    dquest-0.2/src/dqstream.h \
    dquest-0.2/src/dqsqlstatement.h \
    dquest-0.2/src/dqsqlitestatement.h \
    dquest-0.2/src/dqsql.h \
    dquest-0.2/src/dqsharedquery_p.h \
    dquest-0.2/src/dqsharedquery.h \
    dquest-0.2/src/dqsharedlist.h \
    dquest-0.2/src/dqqueryrules.h \
    dquest-0.2/src/dqquery.h \
    dquest-0.2/src/dqmodelmetainfo.h \
    dquest-0.2/src/dqmodel.h \
    dquest-0.2/src/dqmetainfoquery_p.h \
    dquest-0.2/src/dqlistwriter.h \
    dquest-0.2/src/dqlist.h \
    dquest-0.2/src/dqindex.h \
    dquest-0.2/src/dqforeignkey.h \
    dquest-0.2/src/dqfield.h \
    dquest-0.2/src/dqexpression.h \
    dquest-0.2/src/dqconnection.h \
    dquest-0.2/src/dqclause.h \
    dquest-0.2/src/dqbasefield.h \
    dquest-0.2/src/dqabstractmodel.h \
    src/Backend.h \
    src/VocableList.h \
    src/vocabel.h \
    src/keyboardlayout.h

FORMS    += src/mainwindow.ui

RESOURCES += \
    img.qrc

include(src/plugins/androidComponents.pri)

OTHER_FILES += installer.nsi \
    dquest-0.2/src/dquest-install.pri \
    dquest-0.2/src/dquest.pro \
    dquest-0.2/src/dquest.pri \
    android/AndroidManifest.xml \
    android/res/layout/splash.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/version.xml \
    android/res/layout/splash.xml \
    android/res/values/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/version.xml \
    android/AndroidManifest.xml \
    android/res/values/libs.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    android/res/layout/splash.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
    src/QML/EditWindow.qml \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
    android/src/org/qtproject/qt5/android/bindings/QtApplication.java
