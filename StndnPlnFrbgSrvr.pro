#-------------------------------------------------
#
# Project created by QtCreator 2013-10-03T08:36:03
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = StndnPlnFrbgSrvr
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    handler.cpp \
    httpclientworker.cpp

HEADERS += \
    handler.h \
    httpclientworker.h

LIBS += -L../qhttpserver/lib/ -lqhttpserver
