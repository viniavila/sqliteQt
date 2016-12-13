#-------------------------------------------------
#
# Project created by QtCreator 2016-12-13T08:25:48
#
#-------------------------------------------------

QT       += sql
QT       -= gui

TARGET = sqliteQt
TEMPLATE = lib
CONFIG += c++11

DEFINES += SQLITEQT_LIBRARY

SOURCES += \
    qabstractsqlite.cpp \
    qtablesqlite.cpp \
    qtreesqlite.cpp

HEADERS +=\
    sqliteqt_global.h \
    qabstractsqlite.h \
    qtablesqlite.h \
    qtreesqlite.h

OTHER_FILES += \
    README.md

unix {
    target.path = /usr/lib
    INSTALLS += target
}
