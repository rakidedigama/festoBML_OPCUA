#-------------------------------------------------
#
# Project created by QtCreator 2016-03-23T10:04:00
#
#-------------------------------------------------

QT       += core gui concurrent printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(boost.pri)
include(jsonlib.pri)
include(messaginglibs.pri)

TARGET = CastProTool
TEMPLATE = app


INCLUDEPATH +=   C:/PINTAWORKS/FESTO/CastProTool \
                C:/PINTAWORKS/FESTO/FestoLibrary \
                C:/dev/open62541/include\

debug {
    LIBS += C:/dev/open62541/build/Debug/open62541.lib
}

release {
    LIBS += C:/dev/open62541/build/Release/open62541.lib
}


SOURCES += main.cpp\
        mainwindow.cpp \    
    ../QMQTopicInterface.cpp \
    qasyncserial.cpp \
    asyncserial.cpp \
    coordinategrid.cpp \
    ../inifile.cpp\
     ../qcustomplot.cpp \
     C:/PINTAWORKS/FESTO/FestoLibrary/festomotionlibrary.cpp

HEADERS  += mainwindow.h \    
    ../QMQTopicInterface.h \
    ../inifile.h\
    qasyncserial.h \
    asyncserial.h \
    coordinategrid.h \
    ../qcustomplot.h \
    C:/PINTAWORKS/FESTO/FestoLibrary/festomotionlibrary.h

FORMS    += mainwindow.ui
