#ifndef CAMERAMESSAGEHANDLER_H
#define CAMERAMESSAGEHANDLER_H

#include "qmqtopicinterface.h"
#include "qobject.h"

class CameraMessageHandler : public QObject
{
    Q_OBJECT
protected:
    QMQTopicInterface* m_topic;

public:
    CameraMessageHandler(QObject *parent=0);

public slots:
    void newMessage(QString, QByteArray);
    void writeToFesto(uint value);
signals:
    void zConfirmed();
};

#endif // CAMERAMESSAGEHANDLER_H
