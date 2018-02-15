#-------------------------------------------------
#
# Project created by QtCreator 2012-03-06T13:40:01
#
#-------------------------------------------------

QT       += core gui network concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


include(../boost.pri)
include(../rabbitmq.pri)
include(../messaginglibs.pri)
include(../grabberlib.pri)
include(../jsonlib.pri)

TARGET = CameraThreadViewer
TEMPLATE = app

DEFINES += NOMINMAX _CRT_SECURE_NO_WARNINGS

INCLUDEPATH +=     ../../CPPCommon \
                    "C:/Program Files/Basler/pylon 5/Development/include" \
                      ../../Libraries/GrabberLibPylon


LIBS +=  "C:/Program Files/Basler/pylon 5/Development/lib/x64/GCBase_MD_VC120_v3_0_Basler_pylon_v5_0.lib" \
        "C:/Program Files/Basler/pylon 5/Development/lib/x64/GenApi_MD_VC120_v3_0_Basler_pylon_v5_0.lib" \
        "C:/Program Files/Basler/pylon 5/Development/lib/x64/PylonBase_MD_VC120_v5_0.lib" \
        "C:/Program Files/Basler/pylon 5/Development/lib/x64/PylonUtility_MD_VC120_v5_0.lib"

SOURCES +=          main.cpp \
                    camerathreadviewergui.cpp \
                    imageviewerwidget.cpp \
                    camerabufferwrapper.cpp \
                    ../../CPPCommon/Logger.cpp \
                   ../../CPPCommon/IniFile.cpp \
                   ../../CPPCommon/LTime.cpp \
                   ../../CPPCommon/huffman.cpp \
                    ../../CPPCommon/BayerFilter.cpp \
                        Focuser.cpp \
                     ../../CPPCommon/LedController.cpp \
                     lightcalibrator.cpp \
                     ../../CPPCommon/qmqtopicinterface.cpp \
                    ../../CPPCommon/FileDownloader.cpp



HEADERS  +=         camerathreadviewergui.h \
                    camerabufferwrapper.h \
                    imageviewerwidget.h \
                    ../../CPPCommon/Logger.h \
                   ../../CPPCommon/IniFile.h \
                   ../../CPPCommon/LTime.h \
                   ../../CPPCommon/huffman.cpp \
                   ../../CPPCommon/PWImage.h \
                    ../../Libraries/GrabberLibPylon/Grabber.h \
                     Focuser.h \
                     RingBuffer.h \
                    lightcalibrator.h \
                    ../../CPPCommon/LedController.h \
                    ../../CPPCommon/qmqtopicinterface.h \
                    ../../CPPCommon/BayerFilter.h \
                    ../../CPPCommon/FileDownloader.h


FORMS    +=         camerathreadviewergui.ui \
                    imageviewerwidget.ui

RESOURCES += \
    CameraThreadViewer.qrc
