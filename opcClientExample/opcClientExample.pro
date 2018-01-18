#-------------------------------------------------
#
# Project created by QtCreator 2018-01-08T12:44:23
#
#-------------------------------------------------

QT       += core


QT       -= gui

TARGET = opcClientExample
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += C:/dev/open62541/include

LIBS += Ws2_32.lib


debug {
    LIBS += C:/dev/open62541/build/Debug/open62541.lib
}

release {
    LIBS += C:/dev/open62541/build/Release/open62541.lib
}



SOURCES +=  \
    festomotionlibrary.cpp \
    main.cpp



HEADERS += \
    festomotionlibrary.h

