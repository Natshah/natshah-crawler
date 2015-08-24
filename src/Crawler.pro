#-------------------------------------------------
#
# Project created by QtCreator 2010-08-26T11:59:12
#
#-------------------------------------------------

QT       += core\
            sql\
            network\
            xml\
            xmlpatterns\
            gui


TARGET = Crawler-1-10-12
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    qcrawler.cpp \
    configurations.cpp \
    qfilters.cpp \
    qextensions.cpp

HEADERS += \
    qcrawler.h \
    configurations.h \
    qfilters.h \
    qextensions.h
