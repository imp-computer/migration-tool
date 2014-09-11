#-------------------------------------------------
#
# Project created by QtCreator 2014-09-07T09:07:04
#
#-------------------------------------------------

QT       += core gui xmlpatterns xml sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = impPcClient
TEMPLATE = app
CONFIG -= x86_64
#CONFIG=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    filetree.cpp \
    qwebdav/QWebDAV.cpp \
    migrationassistant.cpp \
    SyncQtOwnCloud.cpp \
    sqlite3_util.cpp \
    treebuilder.cpp

HEADERS  += mainwindow.h \
    filetree.h \
    qwebdav/QWebDAV.h \
    migrationassistant.h \
    SyncQtOwnCloud.h \
    sqlite3_util.h \
    treebuilder.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/Cellar/qtkeychain/0.3/include
LIBS += /usr/local/Cellar/qtkeychain/0.3/lib/libqt5keychain.dylib
LIBS += /usr/local/Cellar/sqlite/3.8.6/lib/libsqlite3.dylib

RESOURCES += \
    queries.qrc
