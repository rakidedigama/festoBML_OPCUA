#RBMQSRC += C:/dev/Messaging/rabbitmq-c/ibrabbitmq

INCLUDEPATH += C:/BUILDS/Messaging/rabbitmq_vc14/librabbitmq


LIBS +=         ws2_32.lib kernel32.lib user32.lib


CONFIG( debug, release ) { #debug|release
    message(Using RabbitMQ DEBUG libs)
    LIBS+=              "C:/BUILDS/Messaging/rabbitmq_vc14/librabbitmq/Debug/rabbitmq.4.lib"
}else {
    message(Using RabbitMQ RELEASE libs)
    LIBS+=             "C:/BUILDS/Messaging/rabbitmq_vc14/librabbitmq/Release/rabbitmq.4.lib"
}

