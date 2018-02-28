

include(rabbitmq.pri)

SRCFOLDER += C:/dev/Messaging/SimpleAmqpClient
DEFINES     +=  SimpleAmqpClient_EXPORTS #_WINSOCKAPI_

INCLUDEPATH +=          $$SRCFOLDER/src\
                        C:/dev/Messaging/rabbitmq-c/librabbitmq


HEADERS +=               C:/dev/Messaging/rabbitmq-c/librabbitmq/amqp.h \


SOURCES +=              $$SRCFOLDER/src/AmqpException.cpp \
                        $$SRCFOLDER/src/AmqpLibraryException.cpp \
                        $$SRCFOLDER/src/AmqpResponseLibraryException.cpp \
                        $$SRCFOLDER/src/BasicMessage.cpp \
                        $$SRCFOLDER/src/Channel.cpp \
                        $$SRCFOLDER/src/ChannelImpl.cpp \
                        $$SRCFOLDER/src/Envelope.cpp\
                        $$SRCFOLDER/src/MessageReturnedException.cpp \
                        $$SRCFOLDER/src/Table.cpp \
                        $$SRCFOLDER/src/TableImpl.cpp



#CONFIG( debug, release ) {
#    message(Using MQLIB debug lib)
#    LIBS+=              D:/Builds/SE/Libraries/build-MQLib-Qt_4_8_7_64bit-Debug/debug/MQLib.lib

#}else {
#    message(Using MQLIB release lib)
#    LIBS+=              D:/Builds/SE/Libraries/build-MQLib-Qt_4_8_7_64bit-Release/release/MQLib.lib
#}

OTHER_FILES += \
    ../../../PQIS_ProjectIncludes/rabbitmq.pri

