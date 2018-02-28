#ifndef FOCUSER_H
#define FOCUSER_H


#ifndef Q_MOC_RUN
#include <boost/math/special_functions/fpclassify.hpp>
#endif

#include <QMetaType>
#include <QMutex>
#include <QDateTime>
#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QVector>


#include <vector>
#include <math.h>
#include <numeric>
#include <iostream>



#include "IniFile.h"
#include "PWImage.H"
#include "BayerFilter.H"
#include "Logger.h"
#include "LTime.h"
#include "RingBuffer.h"
#include "QConsoleDebugStream.h"

#include "qmqtopicinterface.h"


class Focuser : public QObject
{
    Q_OBJECT

protected:


    //std::string m_lastImage;
    QMQTopicInterface* m_topic;
     Q_ConsoleDebugStream   logFocus;
    RingBuffer<std::string> m_imageBuffer;
    std::string getLastImage();
    std::string m_sSaveFolder;
    bool    m_bStopRequested;
    bool    m_bSendMessage;
    unsigned m_iLaser;

    bool grabberStatus;

public:
    explicit Focuser(QObject *parent = 0);


    void doTargetedFocusTest();
    void doTargetedFocusTestFirgelli();
    void doTargetedFocusTestMetric();


signals:
    void calculateFocus();
    void moveFirgelli(uint value);
    void startImaging();
    void focusOn(bool);

    void bestFocus(uint);

public slots:

    void cancelFocusing();
    void updateImage(std::string);
    void setSaveFolder(QString);
    void setLaser(QString,qint16);
 void focusCamera();
//    void initiateFocus();
    void sendMessage();
     void newMessage(QString type, QByteArray data);
     void grabberStarted();


};

#endif // FOCUSER_H
