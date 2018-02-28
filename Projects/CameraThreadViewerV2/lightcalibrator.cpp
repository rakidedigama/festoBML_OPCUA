#include "lightcalibrator.h"

#include <iostream>

#ifndef Q_MOC_RUN
#include <boost/math/special_functions/fpclassify.hpp>
#endif

#include "PWImage.H"

LightCalibrator::LightCalibrator(QObject *parent) :
    QThread(parent),m_imageBuffer(3)
{
    m_bCancelled = false;
    logger = new Logger("log_lightCalibration");

    m_uBayerPattern = 2;
    m_uBitDepth = 10;
}

LightCalibrator::~LightCalibrator()
{
    quit();
    wait();
}

void LightCalibrator::calibrate(unsigned targetValue, unsigned bayerPattern,unsigned bitDepth, unsigned uStartValue)
{
    m_uTarget = targetValue;
    m_uStart = uStartValue;
    m_uBitDepth = bitDepth;
    m_uBayerPattern = bayerPattern;
    start();
}

QVector<std::pair<QString, float> > LightCalibrator::test()
{
    QVector<std::pair<QString, float>> ret;


    emit calibrationOn(true);

    LedController garda;
    if (!garda.initialize("LedController.ini"))
    {
        log("Could not initialize gardasoft");
        emit calibrationOn(false);
        return ret;
    }

    unsigned deadTrigger = garda.getChannelCount(); // 4 tai 8

    QVector<unsigned> triggers;
    for (int i = 0; i < garda.getChannelCount(); i++)
    {
        triggers.push_back(garda.getTrigger(i+1));
        garda.setTrigger(i+1, deadTrigger);
    }

    for (int i = 0; i < garda.getChannelCount() - 1; i++)
    {
        garda.setTrigger(i+1,triggers.at(i));
        Sleep(100);
        bool bok = false;
        std::string img = m_imageBuffer.receive(500,&bok);
        // laske keskiarvo jna
        // TAMA ON KESKEN
    }

    emit calibrationOn(false);
    return ret;
}

void LightCalibrator::calibrateChannels(unsigned ch1, unsigned ch2, unsigned targetValue, unsigned uStartValue, LedController *garda)
{
    // punaiset sataseen

    using namespace std;
    bool bok;


    bok = garda->setPulsedBrigthnessAbsolute(ch1,uStartValue);
    bok = garda->setPulsedBrigthnessAbsolute(ch2,uStartValue);



    double imgMean = 0;
    int step = 20;

    unsigned uFailedTries = 0;
    do
    {
        // tan vois tehda toisin...valissa ei mitata vaikka -20
        bok = garda->setPulsedBrigthnessAbsolute(ch1,garda->getChannelPulsedBrightness(ch1) - step);
        bok = garda->setPulsedBrigthnessAbsolute(ch2,garda->getChannelPulsedBrightness(ch2) - step);
        Sleep(100);
        bool bok = false;
        string img = m_imageBuffer.receive(500,&bok);
        if (bok)
        {
            PWImage8::PW_IMAGE_HEADER *pHeader = reinterpret_cast <PWImage8::PW_IMAGE_HEADER*> (&img[0]);
            log("Get Img " + QDateTime::fromMSecsSinceEpoch(pHeader->m_dTime*1000).toString("hh:mm:ss.zzz") + " , " + QString::number(pHeader->m_uWidth) + "," + QString::number(pHeader->m_uHeight));

            PWImage16 pImg(PWImage16::PW_RAW_10BIT, pHeader->m_uWidth, pHeader->m_uHeight);
            pImg.setImage(reinterpret_cast<unsigned short*>(&img[0] + sizeof(PWImage8::PW_IMAGE_HEADER)),pHeader->m_uWidth,pHeader->m_uHeight,false);

            PWImage16 imgDeb(PWImage16::PW_RAW_10BIT,pImg.m_pHeader->m_uSizeBytes * 3);
            BayerFilter::MalvarHeCutler(pImg.getImagePointer(),imgDeb.getImagePointer(),pImg.m_pHeader->m_uWidth,pImg.m_pHeader->m_uHeight,
                                        (BayerFilter::BAYER_FILTER_PATTERN)pImg.m_pHeader->m_uBayerPattern,pImg.m_pHeader->m_uBitDepth);
                                        //(BayerFilter::BAYER_FILTER_PATTERN)m_uBayerPattern,m_uBitDepth);
            PWImage16 imgRed(PWImage16::PW_RAW_10BIT,pImg.m_pHeader->m_uWidth,pImg.m_pHeader->m_uHeight);
            unsigned channel = 0;
            if (3 == ch1)
                channel = 2;
            if (5 == ch1)
                channel = 1;

            imgDeb.copyChannel(imgRed.getImagePointer(),channel,imgRed.m_pHeader->m_uSizeBytes,false);

            unsigned uCutSize = 300;
            PWImage16 imgRedCut(PWImage16::PW_RAW_10BIT,uCutSize,uCutSize);
            imgRed.SubImage(imgRedCut.getImagePointer(),uCutSize,uCutSize,(imgRed.m_pHeader->m_uWidth - uCutSize)/2,(imgRed.m_pHeader->m_uHeight-uCutSize)/2);

//           int imgMeanPrevious = imgMean;
//           imgMean = imgRed.getMean();

//            if (imgMean < 20) // blankko kuva...
//            {
//                imgMean = imgMeanPrevious;
//                log("Blank image"); // pitaa tutkia puskurin toimintaa paremmalla aikaa
//            }
//            else
//                log("Image mean for channels " + QString::number(ch1) + " and " + QString::number(ch2) + " is " + QString::number(imgMean) + " at " + QString::number(garda->getChannelPulsedBrightness(ch1)) + "%");

//            if (imgMean - (double)targetValue < 30)// && channel == 0)
//                step = 2;
        }
        else
            uFailedTries++;
    }while (imgMean - (double)targetValue > 0  && m_bCancelled == false && bok && garda->getChannelPulsedBrightness(ch1) > 100 & uFailedTries < 10);

    if (uFailedTries != 0)
        log("failed image tries " + QString::number(uFailedTries));

}

void LightCalibrator::run()
{
    calibrateLights(m_uTarget, m_uStart);
    m_bCancelled = false;
}

void LightCalibrator::log(QString q)
{
    logger->Log(q.toStdString());
}

void LightCalibrator::calibrateLights(unsigned targetValue, unsigned uStartValue)
{
    using namespace std;

    emit calibrationOn(true);

    LedController garda;
    if (!garda.initialize("LedController.ini"))
    {
        log("Could not initialize gardasoft");
        emit calibrationOn(false);
        return;
    }

//    garda.readCurrentValuesFromDevice();
    garda.readAndSetValuesFromIni("LedController.ini");




    // punaiset, muut pois
    garda.setTrigger(1,1);
    garda.setTrigger(2,1);

    garda.setTrigger(3,2);
    garda.setTrigger(4,2);
    garda.setTrigger(5,6); // kuutostrigger
    garda.setTrigger(6,6);
    calibrateChannels(1,2,targetValue, uStartValue,&garda);



    // siniset, muut pois
    garda.setTrigger(3,1);
    garda.setTrigger(4,1);

    garda.setTrigger(1,2);
    garda.setTrigger(2,2);
    garda.setTrigger(5,6);
    garda.setTrigger(6,6);
    calibrateChannels(3,4,targetValue,uStartValue,&garda);

    // vihreät, muut pois
    garda.setTrigger(5,5);
    garda.setTrigger(6,5);

    garda.setTrigger(1,2);
    garda.setTrigger(2,2);
    garda.setTrigger(3,2);
    garda.setTrigger(4,2);
    calibrateChannels(5,6,targetValue,uStartValue,&garda);

    garda.setTrigger(1,1);
    garda.setTrigger(2,1);
    garda.setTrigger(3,1);
    garda.setTrigger(4,1);
    garda.setTrigger(5,5);
    garda.setTrigger(6,5);

    if (m_bCancelled)
    {
        garda.readAndSetValuesFromIni("LedController.ini");
        garda.setTrigger(1,1);
        garda.setTrigger(2,1);
        garda.setTrigger(3,1);
        garda.setTrigger(4,1);
        garda.setTrigger(5,5);
        garda.setTrigger(6,5);
    }
    else
    {
        QString qsFile = "LedController_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".ini";
        garda.saveCurrentValuesToFile(qsFile.toStdString());
    }

    emit calibrationOn(false);
}

void LightCalibrator::updateImage(std::string img)
{
    using namespace std;
    PWImage8::PW_IMAGE_HEADER *pHeader = reinterpret_cast <PWImage8::PW_IMAGE_HEADER*> (&img[0]);
    if (boost::math::isnan<double>(pHeader->m_dTime))
    {
        log("Insert NAN date img to buffer");
        return;
    }
    else if (pHeader->m_dTime == 0)
    {
        log("Zero date in image");
        //return;
    }


    m_imageBuffer.send(img);
}

void LightCalibrator::cancel()
{
    this->m_bCancelled = true;
}





