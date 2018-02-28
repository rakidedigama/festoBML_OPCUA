#include "qmqtopicinterface.h"


using namespace AmqpClient;
using namespace std;

#include "IniFile.h"


QMQTopicInterface::QMQTopicInterface(
        QString sSubscriberID,QString sExchange,QString sIniFile, QString sHostIP , int iPort, QObject* parent) :
            QThread(parent)
{
    m_sExchange = sExchange;
    m_bConnected = false;
    m_bmMsg =  AmqpClient::BasicMessage::Create("");

    m_sID = sSubscriberID;
    m_sIniFile = sIniFile;
    m_sHostIP = sHostIP;
    m_iHostPort = iPort;

    m_bStopRequested = false;

}


QMQTopicInterface::~QMQTopicInterface()
{
    m_bStopRequested = true;
    wait();
}


bool QMQTopicInterface::bind()
{
    using namespace AmqpClient;
    bool bCreatedChannel = false;
    if (!m_bConnected)
    {
        try
        {
            if (m_sIniFile.length() > 1)
            {

                IniFile ini(m_sIniFile.toStdString().c_str());
                std::string sBrokerIP = ini.GetSetValue("Broker", "Host","127.0.0.1", "MQ broker host ip");
                int iBrokerPort = (int)ini.GetSetValue("Broker", "Port",5672, "MQ broker port");


                if (ini.IsDirty())
                    ini.Save();

                m_Channel = Channel::Create(sBrokerIP,iBrokerPort);

                bCreatedChannel = true;
            }
            if (m_sHostIP.length() > 4 && 0 != m_iHostPort)
            {
                m_Channel = Channel::Create(m_sHostIP.toStdString(),m_iHostPort);
                bCreatedChannel = true;
            }

            if (!bCreatedChannel)
            {
                m_Channel = Channel::Create();
            }


            m_sQueue = QString::fromStdString(m_Channel->DeclareQueue(
                    m_sQueue.toStdString(),
                    false,false,true,true));

            m_sConsumerTag = m_sQueue + "_" + m_sID;
            m_sConsumerTag = QString::fromStdString(m_Channel->BasicConsume(m_sQueue.toStdString(),m_sConsumerTag.toStdString()));//true,false,true);
            m_Channel->DeclareExchange(m_sExchange.toStdString(),Channel::EXCHANGE_TYPE_TOPIC,false,false,false);

            m_bConnected = true;
        }


        catch (MessageReturnedException& e)
        {
            qDebug() << "Message got returned: " << e.what();
            std::cout << "\nMessage body: " << e.message()->Body();
            return false;
        }
        catch (ResourceLockedException& e)
        {
            std::cout << "Locked ex returned: " << e.what() << endl;;
            return false;
        }
        catch (ContentTooLargeException& e)
        {
            std::cout << "Content too large: " << e.what() << endl;;
            return false;
        }
        catch (NoRouteException& e)
        {
            std::cout << "No route: " << e.what() << endl;;
            return false;
        }
        catch (NoConsumersException& e)
        {

            std::cout << "No consumers: " << e.what() << endl;;
            return false;
        }
        catch (AccessRefusedException& e)
        {

            std::cout << "Access refused: " << e.what() << endl;;
            return false;
        }
        catch (NotFoundException& e)
        {

            std::cout << "Not found: " << e.what() << endl;;
            return false;
        }
        catch (PreconditionFailedException& e)
        {

            std::cout << "Precondition failed: " << e.what() << endl;;
            return false;
        }
        catch (std::exception &e)
        {
            std::cout << "MQTopicInterface catch " <<e.what() <<  endl;;

            std::cout << "Failed to connect to Message queue. Make shure RabbitMQ service is running on target host" << endl;
        }

        catch (std::runtime_error &e)
        {
            std::cout << "Runtimer error "     << e.what() << endl;
        }

    }
    if (!m_bConnected)
    {
        //::MessageBox(NULL, L"Yhteys RabbitMQ-palvelimeen on menetetty. Ole hyva ja kaynnista ohjelma uudestaan kun palvelin on taas kaynnissa", L"TwinMapProfiler: Yhteysvirhe",MB_SYSTEMMODAL);
        ::exit(0);
    }

    return m_bConnected;
}

bool QMQTopicInterface::subscribe(QString sTopic, bool bExclusive)
{
    if (!bind())
        return false;

    QString sFinalTopic = m_sExchange + "." + sTopic;
    if (bExclusive)
        sFinalTopic.append("." + m_sConsumerTag.right(m_sConsumerTag.length()-4)); // amq.pois. VOI OLLA BUGI, EI TESTATTU

    m_Channel->BindQueue(m_sQueue.toStdString(),m_sExchange.toStdString(),sFinalTopic.toStdString());
    m_Subscriptions.push_back(sFinalTopic);
    return true;
}

bool QMQTopicInterface::unsubscribe(QString exchange,QString sTopic, bool bExclusive)
{
    foreach(QString s, m_Subscriptions)
    {
        if (sTopic == s)
        {
            m_Channel->UnbindQueue(m_sQueue.toStdString(),exchange.toStdString(),s.toStdString()); // KOVAKOODAUS KELE...
            m_Subscriptions.removeOne(s);// EI TESTATTU
            return true;
        }
        break;

    }
    return false;
}



bool QMQTopicInterface::writeMessage(QString sTopic,QString sExclusiveSubscriber,bool bReplyTo,QString sDataType, const char * pMessage, unsigned uSize,unsigned uTimeout)
{
    using namespace AmqpClient;

    if (!bind())
        return false;


    std::string sMsg(uSize,char(0));
    memcpy(&sMsg[0],pMessage, uSize);

    AmqpClient::BasicMessage::ptr_t bm = BasicMessage::Create();
    bm->Body(sMsg);
    bm->Type(sDataType.toStdString());


    if (bReplyTo)
        m_bmMsg->ReplyTo(m_sConsumerTag.toStdString());



    QString sFinalTopic = m_sExchange + "." + sTopic;
    if (sExclusiveSubscriber.length() > 0)
        sFinalTopic.append("." + sExclusiveSubscriber);


    bool bSuccess = false;

        try
        {
            m_Channel->BasicPublish(m_sExchange.toStdString(),sFinalTopic.toStdString(),bm);
            bSuccess = true;
        }
        catch (MessageReturnedException& e)
            {
                std::cout << "Message got returned: " << e.what();
                std::cout << "\nMessage body: " << e.message()->Body();
            }
            catch (ResourceLockedException& e)
            {

                std::cout << "Locked ex returned: " << e.what() << endl;;
            }
            catch (ContentTooLargeException& e)
            {

                std::cout << "Content too large: " << e.what() << endl;;
            }
            catch (NoRouteException& e)
            {

                std::cout << "No route: " << e.what() << endl;;
            }
            catch (NoConsumersException& e)
            {

                std::cout << "No consumers: " << e.what() << endl;;
            }
            catch (AccessRefusedException& e)
            {

                std::cout << "Access refused: " << e.what() << endl;;
            }
            catch (NotFoundException& e)
            {

                std::cout << "Not found: " << e.what() << endl;;
            }
            catch (PreconditionFailedException& e)
            {

                std::cout << "Precondition failed: " << e.what() << endl;;
            }
            catch (std::runtime_error &e)
            {
                std::cout << "Runtimer error "     << e.what() << std::endl;
            }
            catch (std::exception &e)
            {
                std::cout << "Eerror "     << e.what() << std::endl;
            }





    return bSuccess;
}

QString QMQTopicInterface::readMessage(AmqpClient::BasicMessage::ptr_t &msg, unsigned uTimeout)
{
    using namespace AmqpClient;
    if (!bind())
        return "false";

    std::string sStrippedKey = "";
    try
    {
        if (m_Channel->BasicConsumeMessage(m_sConsumerTag.toStdString(),m_env,uTimeout))
        {
            msg = m_env->Message();

            unsigned uFirstDot = (unsigned)m_env->RoutingKey().find_first_of(".")+1;
            unsigned uLastDot = (unsigned)m_env->RoutingKey().find_last_of(".");
            if (uFirstDot == uLastDot)
                sStrippedKey = m_env->RoutingKey().substr(uFirstDot,m_env->RoutingKey().length() -uFirstDot);
            else
                sStrippedKey = m_env->RoutingKey().substr(uFirstDot,uLastDot - uFirstDot);
            return QString::fromStdString(sStrippedKey);
        }
        else
            return QString::fromStdString(sStrippedKey);
    }
    catch (MessageReturnedException& e)
        {
            std::cout << "Message got returned: " << e.what();
            std::cout << "\nMessage body: " << e.message()->Body();
        }
        catch (ResourceLockedException& e)
        {

            std::cout << "Locked ex returned: " << e.what() << endl;;
        }
        catch (ContentTooLargeException& e)
        {

            std::cout << "Content too large: " << e.what() << endl;;
        }
        catch (NoRouteException& e)
        {

            std::cout << "No route: " << e.what() << endl;;
        }
        catch (NoConsumersException& e)
        {

            std::cout << "No consumers: " << e.what() << endl;;
        }
        catch (AccessRefusedException& e)
        {

            std::cout << "Access refused: " << e.what() << endl;;
        }
        catch (NotFoundException& e)
        {

            std::cout << "Not found: " << e.what() << endl;;
        }
        catch (PreconditionFailedException& e)
        {

            std::cout << "Precondition failed: " << e.what() << endl;;
        }
        catch (std::runtime_error& e)
        {
            std::cout << "Runtime error most problably due to rabbitmq server connection breakdown" << endl;
            m_bConnected = false;
        }
        catch (std::exception &e)
        {
            std::cout << "Eerror "     << e.what() << std::endl;
            //::MessageBox(NULL, L"Yhteys RabbitMQ-palvelimeen on menetetty. Ole hyvä ja käynnistä ohjelma uudestaan kun serveri on taas käynnissä", L"Yhteysvirhe",MB_OK);
            return "false";
        }

    return QString::fromStdString(sStrippedKey);
}

void QMQTopicInterface::run()
{
    AmqpClient::BasicMessage::ptr_t bm = AmqpClient::BasicMessage::Create();
    while (!m_bStopRequested)
    {
        QString ret = readMessage(bm,20);
        if (ret.length() > 0)
        {
            emit msg(ret,QByteArray(bm->Body().c_str()));          
        }
    }
}

