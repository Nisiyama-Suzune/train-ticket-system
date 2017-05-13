#-------------------------------------------------
#
# Project created by QtCreator 2017-05-04T22:00:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ticket3
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
    userlogin.cpp \
    adminlogin.cpp \
    userbuy.cpp \
    userreturn.cpp \
    adminregister.cpp \
    userregister.cpp \
    adminmainwindow.cpp \
    userticket.cpp \
    adminadd.cpp \
    usermainwindow.cpp \
    userhistory.cpp \
    admindelete.cpp

HEADERS  += mainwindow.h \
    userlogin.h \
    adminlogin.h \
    userbuy.h \
    userreturn.h \
    adminregister.h \
    userregister.h \
    adminmainwindow.h \
    userticket.h \
    adminadd.h \
    usermainwindow.h \
    userhistory.h \
    admindelete.h

FORMS    += mainwindow.ui \
    userlogin.ui \
    adminlogin.ui \
    userbuy.ui \
    userreturn.ui \
    adminregister.ui \
    userregister.ui \
    adminmainwindow.ui \
    userticket.ui \
    adminadd.ui \
    usermainwindow.ui \
    userhistory.ui \
    admindelete.ui
