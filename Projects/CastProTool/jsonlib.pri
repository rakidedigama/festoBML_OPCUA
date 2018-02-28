INCLUDEPATH +=          C:/dev/jsoncpp/include/json



HEADERS     +=           "C:/dev/jsoncpp/include/json/json.h"    \
                         "C:/dev/jsoncpp/include/json/forwards.h"

SOURCES     +=          C:/dev/jsoncpp/dist/jsoncpp.cpp

#CONFIG( debug, debug|release ) {
 #   LIBS+=              C:/dev/jsoncpp/src/lib_json/Debug/jsoncpp.lib
#}else {
 #   LIBS+=             C:/dev/jsoncpp/src/lib_json/Release/jsoncpp.lib
#}
