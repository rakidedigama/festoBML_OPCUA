INCLUDEPATH +=          C:/dev/jsoncpp/include/json



HEADERS     +=           "C:/dev/jsoncpp/include/json/json.h"    \
                         "C:/dev/jsoncpp/include/json/forwards.h"

SOURCES     +=          C:/dev/jsoncpp/dist/jsoncpp.cpp

#CONFIG( debug, debug|release ) {
 #   LIBS+=              C:/dev/jsoncpp/src/lib_json/Debug/jsoncpp.lib
#}else {
 #   LIBS+=             C:/dev#-------------------------------------------------
#
# Project created by QtCreator 2014-02-18T09:52:55
#
#-------------------------------------------------

QT       -= gui

TARGET = GrabberLib
TEMPLATE = lib

DEFINES +=              GRABBERLIB_LIBRARY #_PT_64_

INCLUDEPATH +=          ../../../CPPCommon \
                        ../../../Libraries




SOURCES +=             ../../../Libraries/GrabberLibPylon/grabber.cpp


HEADERS +=              ../../../Libraries/GrabberLibPylon/grabber.h\
                        LedController.h \
                        ../../../Libraries/GrabberLibPylon/GrabberLib_global.h
§
/jsoncpp/src/lib_json/Release/jsoncpp.lib
#}
