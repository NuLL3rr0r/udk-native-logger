#-------------------------------------------------
#
# Project created by QtCreator 2012-11-18T12:19:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UdkLogServer
TEMPLATE = app

DEFINES += BOOST_LIB_DIAGNOSTIC
DEFINES += BOOST_ALL_DYN_LINK

# http://www.zachburlingame.com/2011/05/resolving-redefinition-errors-betwen-ws2def-h-and-winsock-h/
DEFINES += WIN32_LEAN_AND_MEAN
# or
# http://stackoverflow.com/questions/1372480/c-redefinition-header-files
# DEFINES += _WINSOCKAPI_

INCLUDEPATH += $$PWD/../../../_LIBS/Boost/include
INCLUDEPATH += $$PWD/../../../_LIBS/ZeroMQ/include

DEPENDPATH += $$PWD/../../../_LIBS/Boost/include
DEPENDPATH  += $$PWD/../../../_LIBS/ZeroMQ/include

LIBS += -L$$PWD/../../../_LIBS/Boost/lib/

# SetWindowPos()
LIBS += -lUser32

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../_LIBS/ZeroMQ/lib/ -llibzmq
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../_LIBS/ZeroMQ/lib/ -llibzmq_d


SOURCES += main.cpp\
        mainwindow.cpp\
        log.cpp\
        server.cpp

HEADERS  += log.hpp\
        mainwindow.hpp\
        server.hpp

FORMS    += mainwindow.ui

