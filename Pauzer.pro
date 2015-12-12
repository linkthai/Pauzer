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

SOURCES += main.cpp \
        minipauzer.cpp \
    manager.cpp \
    player.cpp \
    sliderbar.cpp \
    folderdialog.cpp \
    checkablefilesystemmodel.cpp \
    autodetector.cpp \
    playlist.cpp \

HEADERS  += minipauzer.h \
    manager.h \
    player.h \
    stable.h \
    sliderbar.h \
    folderdialog.h \
    checkablefilesystemmodel.h \
    autodetector.h \
    playlist.h \

FORMS    += minipauzer.ui \
    folderdialog.ui \

INCLUDEPATH += C:\bass24\c \
                C:\Libraries\taglib\include\taglib \

LIBS += -LC:\bass24\c -lbass \
        -LC:\Libraries\taglib\lib -ltag \

win32:LIBS += -lOle32 \

DISTFILES += \
