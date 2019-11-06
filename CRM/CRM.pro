#-------------------------------------------------
#
# Project created by QtCreator 2017-06-18T13:36:28
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CRM
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
    client.cpp \
    mainwin.cpp \
        mainwindow.cpp \
    dlgitem.cpp \
    registerdialog.cpp \
    server.cpp

HEADERS  += mainwindow.h \
    client.h \
    dlgitem.h \
    mainwin.h \
    registerdialog.h \
    server.h

FORMS    += mainwindow.ui \
    client.ui \
    dlgitem.ui \
    mainwin.ui \
    registerdialog.ui \
    server.ui

RC_FILE += myapp.rc

RESOURCES += \
    rc.qrc
QT += network
