#-------------------------------------------------
#
# Project created by QtCreator 2015-04-01T10:12:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MIPS-editor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    assembler.cpp \
    binarystringtranslator.cpp \
    coetranslator.cpp \
    disassembler.cpp

HEADERS  += mainwindow.h \
    assembler.h \
    binarystringtranslator.h \
    coetranslator.h \
    disassembler.h

FORMS    += mainwindow.ui
