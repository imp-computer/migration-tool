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
    treebuilder.cpp \
    account.cpp \
    networkjobs.cpp \
    creds/abstractcredentials.cpp \
    cookiejar.cpp \
    3rdparty/qjson/json.cpp \
    creds/httpcredentials.cpp \
    mirallaccessmanager.cpp

HEADERS  += mainwindow.h \
    filetree.h \
    qwebdav/QWebDAV.h \
    migrationassistant.h \
    SyncQtOwnCloud.h \
    sqlite3_util.h \
    treebuilder.h \
    account.h \
    networkjobs.h \
    creds/abstractcredentials.h \
    cookiejar.h \
    3rdparty/qjson/json.h \
    creds/httpcredentials.h \
    mirallaccessmanager.h

FORMS    += mainwindow.ui

unix {
    macx {
        LIBS += -L/usr/local/Cellar/qtkeychain/0.3/lib -lqt5keychain \
                -L/usr/local/Cellar/sqlite/3.8.6/lib/lib -lsqlite3
        INCLUDEPATH += /usr/local/Cellar/qtkeychain/0.3/include
    }
    else {
        INCLUDEPATH += /usr/include/qt5keychain
        LIBS += -lqt5keychain -lsqlite3
    }
}

RESOURCES += \
    queries.qrc
