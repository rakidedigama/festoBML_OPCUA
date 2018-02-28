#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include "qmqtopicinterface.h"
#include <QObject>

class MessageHandler : public QObject
{
    Q_OBJECT
protected:
    QMQTopicInterface* m_topic;



public:
    MessageHandler(QObject *parent=0);

signals:
    void moveCamToFocus(double zPos);

private slots:
   void writeToFocusser();


public slots:
   void newMessage(QString, QByteArray); // from msg queue

};

#endif // MESSAGEHANDLER_H
