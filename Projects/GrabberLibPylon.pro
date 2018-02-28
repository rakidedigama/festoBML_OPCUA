#-------------------------------------------------
#
# Project created by QtCreator 2014-02-18T09:52:55
#
#-------------------------------------------------

QT       += core
QT       -= gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GrabberLib
TEMPLATE = lib



include(boost.pri)
include(jsonlib.pri)


DEFINES +=              GRABBERLIB_LIBRARY #_PT_64_

INCLUDEPATH +=          C:/GITROOT/CPPCommon \
                        C:/GITROOT/QTCommon \
                        C:/GITROOT/Libraries/GrabberLibPylon \
                    "C:/Program Files/Basler/pylon 5/Development/include" \

LIBS +=  "C:/Program Files/Basler/pylon 5/Development/lib/x64/GCBase_MD_VC120_v3_0_Basler_pylon_v5_0.lib" \
        "C:/Program Files/Basler/pylon 5/Development/lib/x64/GenApi_MD_VC120_v3_0_Basler_pylon_v5_0.lib" \
        "C:/Program Files/Basler/pylon 5/Development/lib/x64/PylonBase_MD_VC120_v5_0.lib" \
        "C:/Program Files/Basler/pylon 5/Development/lib/x64/PylonUtility_MD_VC120_v5_0.lib"






SOURCES +=              C:/GITROOT/Libraries/GrabberLibPylon/grabber.cpp \
                        C:/GITROOT/CPPCommon/IniFile.cpp \
                        C:/GITROOT/CPPCommon/Logger.cpp \
                        C:/GITROOT/CPPCommon/LTime.cpp \
                        C:/GITROOT/CPPCommon/LedController.cpp \
                       C:/GITROOT/CPPCommon/huffman.cpp \
                       C:/GITROOT/CPPCommon/BayerFilter.cpp

HEADERS +=              C:/GITROOT/Libraries/GrabberLibPylon/grabber.h\
                        C:/GITROOT/CPPCommon/LedController.h \
                        GrabberLib_global.h

