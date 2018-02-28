#ifndef QMQTOPICINTERFACE_H
#define QMQTOPICINTERFACE_H

#include <windows.h>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <amqp.h>

#include <QStringList>
#include <QThread>
#include <QDebug>
#include <QMutex>
#include <QMessageBox>

class QMQTopicInterface : public QThread
{
    Q_OBJECT
protected:


    QStringList	m_Subscriptions;

    AmqpClient::Channel::ptr_t	m_Channel;

    QString	m_sQueue;
    QString	m_sExchange;
    QString	m_sConsumerTag;
    QString  m_sID;


    bool	m_bConnected;
    AmqpClient::Envelope::ptr_t     m_env;
    AmqpClient::BasicMessage::ptr_t m_bmMsg;


    bool bind();

    QString m_sIniFile;
    QString m_sHostIP;
    int         m_iHostPort;
    QString readMessage(AmqpClient::BasicMessage::ptr_t &msg, unsigned uTimeout);

    bool m_bStopRequested;
    virtual void run();
    QMutex m_mutex;

public:
    // ei ole loppuunmietitty konstruktori
    QMQTopicInterface(QString sSubscriberID,QString sExchange,QString sIniFile, QString sHostIP ="" , int iPort=0, QObject* parent = NULL);
    ~QMQTopicInterface();
    bool subscribe(QString sTopic, bool bExclusive = false);
    bool unsubscribe(QString exchange,QString sTopic, bool bExclusive = false);
    bool writeMessage(QString sTopic,
                      QString sExclusiveSubscriber,bool bReplyTo,
                      QString,
                      const char * pMessage, // vois olla byte array
                      unsigned uSize,
                      unsigned uTimeout);


    inline bool isConnected(){return m_bConnected;};

    QString getConsumerTag()
    {
            return "";
    };
signals:
    void msg(QString type,QByteArray msg);
};

#endif // QMQTOPICINTERFACE_H
