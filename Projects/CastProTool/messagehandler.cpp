#include "messagehandler.h"
#include "json.h"
#include "stdio.h"
#include "iostream"

using namespace std;

MessageHandler::MessageHandler(QObject *parent) : QObject(parent)
{
    m_topic = new QMQTopicInterface("CastProTool", "PW","CastProTool.ini");
    QObject::connect(m_topic,SIGNAL(msg(QString,QByteArray)),this,SLOT(newMessage(QString,QByteArray)));
    m_topic->subscribe("ZAXIS_COMMANDS");
    m_topic->start();
}



void MessageHandler::newMessage(QString type, QByteArray data){

    using namespace std;
    cout<<"got new message in CastPro MessageHandler. Reading type:" << type.toStdString() <<endl;

    if (type.toStdString() == "ZAXIS_COMMANDS")
    {
        Json::Value msg;
        Json::Reader reader;
        cout<<"Reading Z Axis commands" <<endl;
        if(reader.parse(data.data(), msg))
        {
            // from Focusser, trigger Festo to move
            if(msg["name"].asString() == "ZAXIS"){
                cout<< data.data() <<endl;
               std::string zposStr = msg["pos"].asString();
               double zPos = double( std::stod(zposStr,nullptr));
               cout<<"Message from Focusser, move Zaxis :" << zPos << endl;
               emit moveCamToFocus(zPos);
            }
            if(msg["name"].asString() == "ZAXIS_BEST"){
               std::string zposStr = msg["pos"].asString();
               double zPos = double( std::stod (zposStr,nullptr));
               cout<<"Got best focus from Focusser:" << zPos << endl;
               emit moveCamToFocus(zPos);
            }

            // from Festo,  write confirmation message to Focusser
//            if(msg["name"].asString() == "ZMOTION_COMPLETED"){
//                cout<<"Message from Festo, Zmotion completed. Relay Msg to Focusser" << endl;
//                writeToFocusser();

//            }
        }
    }



}

void MessageHandler::writeToFocusser(){
    std::cout<<"Sending confirmation message to FOCUSSER from MESSAGEHANDLER" <<endl;
    Json::Value msg;
    //Json::Value array;
    msg["type"] = "JSON";
    msg["name"] = "ZMOTION_COMPLETED";

    qDebug()<<"Sending confirmation message to FOCUSSER from MESSAGEHANDLER";
   m_topic->writeMessage("ZAXIS_CONFIRMS","",false,"JSON",msg.toStyledString().c_str(),msg.toStyledString().length(),100);
    std::cout << "Message sent from MESSAGEHANDLER " << std::endl;
}


