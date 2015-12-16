#-------------------------------------------------
#
# Project created by QtCreator 2015-12-03T02:04:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pauzer
TEMPLATE = app

CONFIG += resources_big

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
    master.cpp \
    song.cpp \
    qdraggableframe.cpp \

HEADERS  += minipauzer.h \
    manager.h \
    player.h \
    stable.h \
    sliderbar.h \
    folderdialog.h \
    checkablefilesystemmodel.h \
    autodetector.h \
    playlist.h \
    master.h \
    song.h \
    qdraggableframe.h \

FORMS    += minipauzer.ui \
    folderdialog.ui \

INCLUDEPATH += C:\Libraries\bass24\c \
                C:\Libraries\taglib\include \

debug_and_release {
    CONFIG -= debug_and_release
    CONFIG += debug_and_release
}

CONFIG(debug, debug|release) {
    CONFIG -= debug release
    CONFIG += debug
    LIBS += -LC:\Libraries\taglib\lib\win32-v110 -ltaglibd \
}

CONFIG(release, debug|release) {
    CONFIG -= debug release
    CONFIG += release
    LIBS += -LC:\Libraries\taglib\lib\win32-v110 -ltaglib \
}

LIBS += -LC:\Libraries\bass24\c -lbass \


win32:LIBS += -lOle32 \

DISTFILES +=

RESOURCES += \
    resources.qrc \
