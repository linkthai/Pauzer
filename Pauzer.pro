#-------------------------------------------------
#
# Project created by QtCreator 2015-12-03T02:04:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pauzer
TEMPLATE = app

PRECOMPILED_HEADER  = stable.h

SOURCES += main.cpp\
        minipauzer.cpp \
    manager.cpp \
    player.cpp \
    sliderbar.cpp \
    folderdialog.cpp \
    checkablefilesystemmodel.cpp \
    autodetector.cpp \

HEADERS  += minipauzer.h \
    manager.h \
    player.h \
    stable.h \
    sliderbar.h \
    folderdialog.h \
    checkablefilesystemmodel.h \
    autodetector.h \

FORMS    += minipauzer.ui \
    folderdialog.ui

INCLUDEPATH += C:\bass24\c \

LIBS += -LC:\bass24\c -lbass \

DISTFILES += \
