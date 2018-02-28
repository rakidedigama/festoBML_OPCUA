#ifndef LIGHTCALIBRATOR_H
#define LIGHTCALIBRATOR_H

#include <QThread>
#include <QDateTime>
#include <QVector>


#include "RingBuffer.h"
#include "LedController.h"
#include "BayerFilter.H"
#include "Logger.h"

class LightCalibrator : public QThread
{
    Q_OBJECT
protected:
    RingBuffer<std::string> m_imageBuffer;
    std::string getLastImage();

    void calibrateChannels(unsigned, unsigned, unsigned,unsigned, LedController*);
    void calibrateLights(unsigned targetValue,unsigned uStartValue);
    bool    m_bCancelled;
    virtual void run();
    Logger *logger;
    void log(QString);
    unsigned m_uTarget;
    unsigned m_uStart;

    unsigned m_uBayerPattern;
    unsigned m_uBitDepth;

public:
    explicit LightCalibrator(QObject *parent = 0);
    ~LightCalibrator();
    void calibrate(unsigned targetValue,  unsigned bayerPattern = 2,unsigned bitDepth = 10,unsigned uStartValue = 999);

    QVector<std::pair<QString,float>> test();

    
signals:
    void calibrationOn(bool);
public slots:


    void updateImage(std::string);
    void cancel();


    
};

#endif // LIGHTCALIBRATOR_H
