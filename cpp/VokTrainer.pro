#-------------------------------------------------
#
# Project created by QtCreator 2013-05-08T13:41:58
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VokTrainer
TEMPLATE = app

PRECOMPILED_HEADER = ./src/pch.h
INCLUDEPATH += ./src/ ./dquest-0.2/src/

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/db.cpp \
    src/category.cpp \
    src/vokabel.cpp \
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
    src/box.cpp \
    src/factory.cpp

HEADERS  += src/mainwindow.h \
    src/pch.h \
    src/db.h \
    src/category.h \
    src/vokabel.h \
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
    src/box.h \
    src/factory.h

FORMS    += src/mainwindow.ui

RESOURCES += \
    img.qrc

OTHER_FILES += \
    dquest-0.2/src/dquest-install.pri \
    dquest-0.2/src/dquest.pro \
    dquest-0.2/src/dquest.pri
