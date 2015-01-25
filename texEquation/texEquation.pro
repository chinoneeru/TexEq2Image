#-------------------------------------------------
#
# Project created by QtCreator 2014-09-23T16:53:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = texEquation
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    converter.cpp \
    myPlainTextEdit.cpp \
    importform.cpp

HEADERS  += mainwindow.h \
    converter.h \
    myPlainTextEdit.h \
    importform.h

FORMS    += mainwindow.ui \
    importform.ui
