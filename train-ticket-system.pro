#-------------------------------------------------
#
# Project created by QtCreator 2017-05-12T16:24:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = train-ticket-system
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


SOURCES += main.cpp\
        mainwindow.cpp \
    tts_server/source/server.cpp \
    tts_server/source/train_manager.cpp \
    tts_server/source/account_manager.cpp \

HEADERS  += mainwindow.h \
    vector.hpp \
    utility.hpp \
    smart_ptr.hpp \
    map.hpp \
    deque.hpp \
    memory.hpp \
    exceptions.hpp \
    tts_server/header/train_manager.h \
    tts_server/header/server.h \
    tts_server/header/account_manager.h \
    tts_server/header/forward_declaration.h

FORMS    += mainwindow.ui

DISTFILES += \
    log.txt
