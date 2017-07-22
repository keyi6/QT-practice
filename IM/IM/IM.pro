#-------------------------------------------------
#
# Project created by QtCreator 2017-07-13T15:21:39
#
#-------------------------------------------------

QT       += core gui
QT	 += network
QT	 += script
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IM
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        index.cpp \
    chatwidget.cpp \
    logindialog.cpp \
    registerdialog.cpp \
    networkrequest.cpp \
    inputwidget.cpp \
    friendlist.cpp \
    warningdialog.cpp \
    client.cpp \
    sever.cpp \
    historywidget.cpp \
    grounpchatwidget.cpp \
    groupinputwidget.cpp


HEADERS += \
        index.h \
    chatwidget.h \
    logindialog.h \
    registerdialog.h \
    networkrequest.h \
    inputwidget.h \
    friendlist.h \
    warningdialog.h \
    client.h \
    sever.h \
    historywidget.h \
    grounpchatwidget.h \
    groupinputwidget.h

FORMS += \
        index.ui \
    chatwidget.ui \
    logindialog.ui \
    registerdialog.ui \
    inputwidget.ui \
    friendlist.ui \
    friendlist.ui \
    sendfiledialog.ui \
    registerdialog.ui \
    decideconnectiondialog.ui \
    warningdialog.ui \
    client.ui \
    sever.ui \
    historywidget.ui \
    grounpchatwidget.ui \
    groupinputwidget.ui

RESOURCES += \
    icon.qrc
