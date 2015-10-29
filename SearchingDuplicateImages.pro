#-------------------------------------------------
#
# Project created by QtCreator 2015-10-26T09:17:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SearchingDuplicateImages
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    duplicate.cpp \
    tableitemdelegate.cpp

HEADERS  += mainwidget.h \
    duplicate.h \
    tableitemdelegate.h

FORMS    += mainwidget.ui
