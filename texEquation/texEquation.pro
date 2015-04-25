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
    importform.cpp \
    mySyntaxHighlighter.cpp \
    textTree.cpp \
    textTreeNode.cpp \
    blockHighlighter.cpp

HEADERS  += mainwindow.h \
    converter.h \
    myPlainTextEdit.h \
    importform.h \
    mySyntaxHighlighter.h \
    textTree.h \
    textTreeNode.h \
    blockHighlighter.h

FORMS    += mainwindow.ui \
    importform.ui
