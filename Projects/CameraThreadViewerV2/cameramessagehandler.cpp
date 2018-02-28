#include "cameramessagehandler.h"
#include "json.h"
#include "stdio.h"
#include "iostream"
#include "QTime"


CameraMessageHandler::CameraMessageHandler(QObject *parent) : QObject(parent)
{
    m_topic = new QMQTopicInterface("CamMsgHandler", "PW","CameraThreadViewer.ini");
    QObject::connect(m_topic,SIGNAL(msg(QString,QByteArray)),this,SLOT(newMessage(QString,QByteArray)));
    m_topic->subscribe("ZAXIS_CONFIRMS");
    m_topic->start();
}



void CameraMessageHandler::newMessage(QString type, QByteArray data){

    using namespace std;
    cout<<"got new message in CastPro MessageHandler. Reading type:" << type.toStdString() <<endl;

    if (type.toStdString() == "ZAXIS_CONFIRMS")
    {
        Json::Value msg;
        Json::Reader reader;
        cout<<"Reading Z Axis CONFIRMS" <<endl;
        if(reader.parse(data.data(), msg))
        {
            cout << data.data() << endl;
            if(msg["name"].asString() == "ZMOTION_COMPLETED"){
                    cout<<"Z Focus Motion confirmed" << endl;
                    qDebug()<< "Z Motion Confirmation RECIEVED";
                    emit zConfirmed();
                }
            if(msg["name"].asString() == "PTP_SHOOT"){
                    cout<<"Got SHOOT msg" << endl;
                    qDebug()<< "Got SHOOT msg";
                    QStringList shootList;

                    double x = msg["x"].asDouble();
                    double y = msg["y"].asDouble();
                    QString tag = QString::fromStdString(msg["tag"].asString());
                    QString cLass = QString::fromStdString(msg["class"].asString());

                    shootList.append(QString::number(x));
                    shootList.append(QString::number(y));
                    shootList.append(tag);
                    shootList.append(cLass);



                    emit shoot(shootList);
                }
        }
    }
}

void CameraMessageHandler::writeToFesto(uint value){
//    QTime timer;
//    std::cout<< "Sending message Z axis"<<std::endl;
//    timer.start();
//    Json::Value msg;
//    //Json::Value array;
//    msg["type"] = "JSON";
//    msg["name"] = "ZAXIS";
//    msg["pos"] = value;
//    qDebug()<<"Writing message to FESTO";
//    m_topic->writeMessage("ZAXIS_COMMANDS","",false,"JSON",msg.toStyledString().c_str(),msg.toStyledString().length(),100);
//    int eTime = timer.elapsed();
//    std::cout << "Message sent: " << eTime <<std::endl;
}
