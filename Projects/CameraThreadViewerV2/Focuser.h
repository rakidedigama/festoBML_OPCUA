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


class Focuser : public QThread
{
    Q_OBJECT

protected:
    bool zConfirmed;

    //std::string m_lastImage;
    RingBuffer<std::string> m_imageBuffer;
    std::string getLastImage();
    std::string m_sSaveFolder;
    bool    m_bStopRequested;

    unsigned m_iLaser;

public:
    explicit Focuser(QObject *parent = 0);
    ~Focuser();
    virtual void run();
//    void doTargetedFocusTest();
//    void doTargetedFocusTestFirgelli();
//    void doTargetedFocusTestMetric();
signals:
    void moveFirgelli(uint value);
    void startImaging();
    void focusOn(bool);

    void bestFocus(uint);
      void confirmZ();
public slots:

    void cancelFocusing();
    void updateImage(std::string);
    void setSaveFolder(QString);
    void setLaser(QString,qint16);


    
};

#endif // FOCUSER_H
