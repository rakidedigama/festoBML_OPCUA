#include "CameraBufferWrapper.h"

#include <QTimer>
#include <QMessageBox>
#include <iostream>
#include "LTime.h"
#include "grabber.h"
#include "StringBuilder.h"
#include "PWImage.H"







void CameraBufferWrapper::testFunc()
{
    PWImage16 pw(PWImage16::PW_RAW_10BIT,0,0);
    if (pw.load("D:\\temp\\2015-11-27T10_30_11_722.lvt")>2)
    {
        PWImage16 rgbImg(
                    PWImage16::PW_RGB_10BIT,
                    pw.m_pHeader->m_uSizeBytes*3);


        BayerFilter::MalvarHeCutler(
                pw.getImagePointer(),
                rgbImg.getImagePointer(),
                pw.m_pHeader->m_uWidth,
                pw.m_pHeader->m_uHeight,
                BayerFilter::BAYER_PATTERN_RGGB,//(BayerFilter::BAYER_FILTER_PATTERN)m_uBayerPattern,//(ImageFilter::FILTER_PATTERN)uBayerTiling,//BAYER_PATTERN_RGGB,
                12//12//   KORJATTAVA PARAMETRIKSI
                );


        unsigned uRGBLiveMsgSize = sizeof(PWImage16::PW_IMAGE_HEADER) + pw.m_pHeader->m_uSizeBytes*sizeof(unsigned char)*3;
        std::string sRGBLive(uRGBLiveMsgSize ,char(0));
        PWPixelRGB10BIT* pRgb = reinterpret_cast <PWPixelRGB10BIT*>( rgbImg.getImagePointer());
        rgbImg.copyTo8Bit(reinterpret_cast<unsigned char*>(&sRGBLive[0+sizeof(PWImage8::PW_IMAGE_HEADER)]),pw.m_pHeader->m_uSizeBytes*3);//rawImg.m_pHeader->m_uBitDepth-4);



        memcpy(&sRGBLive[0],rgbImg.m_pHeader, sizeof(PWImage8::PW_IMAGE_HEADER));
        emit newImageAvailable(sRGBLive);

    }
}

CameraBufferWrapper::CameraBufferWrapper()

{

    m_is = RAW;
    m_bCameraInited = false;

    //moveToThread(this);

    m_bStopRequested = false;
    m_bPaused = false;
    m_bSaving = false;
    m_bSaveOne = false;
    m_iFirgelliVal = 0;


    m_dSaveX = 0;
    m_dSaveY = 0;
}


CameraBufferWrapper::~CameraBufferWrapper()
{   
    m_bStopRequested = true;
    quit();
    wait();

}
void CameraBufferWrapper::setIS(IMAGE_SELECTION is)
{
    QString qsIniFile = "CameraThreadViewer.ini";
    IniFile ini(qsIniFile.toStdString().c_str());
    m_is = is;
    ini.SetValue("Image", "Selection", (int)is);
    ini.Save();
}

CameraBufferWrapper::IMAGE_SELECTION  CameraBufferWrapper::getIS()
{
    using namespace std;
    QString qsIniFile = "CameraThreadViewer.ini";
    IniFile ini(qsIniFile.toStdString().c_str());
    unsigned d= ini.GetSetValue("Image", "Selection",(int)CameraBufferWrapper::RAW,",,");    
    if (d == (int)CameraBufferWrapper::RAW)
        m_is = RAW;
    if (d == (int)CameraBufferWrapper::RGB)
        m_is = RAW;
    if (d == (int)CameraBufferWrapper::R)
        m_is = R;
    if (d == (int)CameraBufferWrapper::G)
        m_is = G;
    if (d == (int)CameraBufferWrapper::B)
        m_is = B;

    return m_is;
}

void CameraBufferWrapper::setPause(bool bPause)
{
    this->m_bPaused = bPause;
}

void CameraBufferWrapper::setSaving(bool bSaving)
{
    this->m_bSaving = bSaving;
}

bool CameraBufferWrapper::isSaving()
{
    return m_bSaving;
}

void CameraBufferWrapper::setSaveFolder(QString qsFolder)
{
    this->qSaveFolder = qsFolder;
}

void CameraBufferWrapper::saveOne()
{
    m_bSaveOne = true;
}


void CameraBufferWrapper::saveOne(double x, double y,QString savetag, QString saveClass )
{
    m_dSaveX = x;
    m_dSaveY = y;
    m_qsSaveTag = savetag;
    m_qsSaveType = saveClass;
    m_bSaveOne = true;
}



bool CameraBufferWrapper::stopThread()
{
    m_bStopRequested = true;
    wait();
    //m_CameraGrab->StopGrabbing();
    return true;
}

void CameraBufferWrapper::run()
{
    using namespace std;
    QString qsIniFile = "CameraThreadViewer.ini";
    IniFile ini(qsIniFile.toStdString().c_str());

    unsigned uTimeOut = ini.GetSetValue("Images", "WaitTimeout", 1000, "wait timeout in ms");
    unsigned uThreshold = ini.GetSetValue("Images", "DarkImgThreshold", 30, "");

    unsigned uBayerPattern = ini.GetSetValue("Images", "Bayer", 2, "2 = RGGB (Imperx), 1 = GRBG (Basler)");
    unsigned uBitDepth = ini.GetSetValue("Images", "Depth", 10, "Imperx 10, Basler 12");




    Grabber grabber(qsIniFile.toStdString());

    grabber.start();

    grabber.startGrabbing();




    cout << "Sleep before getting" << endl;
    Sleep(5000);

    unsigned uWidth = grabber.getImageWidth();
    unsigned uHeight = grabber.getImageHeight();


    cout << "Grabber says width and height are " << uWidth << ", " << uHeight << endl;

    if (0 == uWidth || 0 == uHeight)
    {
        cout << "Error, camera image size incorrect, reading values from " << qsIniFile.toStdString() << endl;
        uWidth = ini.GetSetValue("Images", "Width", 600,"");
        uHeight = ini.GetSetValue("Images", "Height", 600,"");
        emit imageSize(uWidth, uWidth, qsIniFile);
    }
    else
        emit imageSize(uWidth, uWidth, "Camera");
    cout << "Image width " << uWidth << endl;
    cout << "Image height " << uHeight << endl;

    if (ini.IsDirty())
        ini.Save();



    PWImage16 rawImg(
                PWImage16::PW_RAW_10BIT,
                uWidth,
                uHeight);
    PWImage16 rgbImg(
                PWImage16::PW_RGB_10BIT,
                uWidth*uHeight*3);

    PWImage8 imgUC(
                PWImage8::PW_RAW_8BIT,
                uWidth,
                uHeight);

    unsigned uLiveMsgSize = sizeof(PWImage16::PW_IMAGE_HEADER) + imgUC.m_pHeader->m_uSizeBytes*sizeof(unsigned char);
    string sLive(uLiveMsgSize ,char(0));

    unsigned uRGBLiveMsgSize = sizeof(PWImage16::PW_IMAGE_HEADER) + imgUC.m_pHeader->m_uSizeBytes*sizeof(unsigned char)*3;
    string sRGBLive(uRGBLiveMsgSize ,char(0));

    unsigned uImagesFailed = 0;

    rawImg.m_pHeader->m_uBayerPattern = uBayerPattern;
    rawImg.m_pHeader->m_uBitDepth = uBitDepth;

    std::string logFile = StringBuilder() << qSaveFolder.toStdString()  << "\\" << "log";
    Logger log(logFile.c_str());

    //emit grabberStarted(); // signal to focusser

    while (!m_bStopRequested)
    {        
        grabber.setTrigger(bTrigger);
        grabber.setTrigger(!bTrigger);

        if (!m_bPaused)
        {
            cout << "Get new image, buffer size " << rawImg.m_pHeader->m_uSizeBytes << endl;

            int iRet = grabber.getImage(rawImg.getImagePointer(),rawImg.m_pHeader->m_uSizeBytes,uTimeOut);
            if (-1 != iRet)
            {


                if (m_bSaving || m_bSaveOne)
                {
                    std::string sPath = StringBuilder() << qSaveFolder.toStdString();
                    if ( m_dSaveX !=  0 || m_dSaveY != 0)
                    {
                        if (m_qsSaveTag.length() == 0 && m_qsSaveType.length() == 0)
                            sPath = StringBuilder() << sPath <<"\\"<< m_dSaveX << "_" << m_dSaveY <<"_" <<  Time(Time::Local).ToExtendedISO() << ".lvt";
                        else
                        {
                            QString subPath = QString::fromStdString(StringBuilder() << sPath <<"\\" << m_qsSaveType.toStdString() << "\\" << m_qsSaveTag.toStdString()<< "\\");
                            if (!QDir(subPath).exists())
                                QDir().mkpath(subPath);

                            sPath = StringBuilder() << sPath <<"\\" << m_qsSaveType.toStdString() << "\\" << m_qsSaveTag.toStdString()<< "\\"<< m_dSaveX << "_" << m_dSaveY <<"_" <<  Time(Time::Local).ToExtendedISO() << ".lvt";
                        }
                    }
                    else
                        sPath = StringBuilder() << sPath << "\\" << Time(Time::Local).ToExtendedISO() << ".lvt";

                    if (!QDir(qSaveFolder).exists())
                        QDir().mkpath(qSaveFolder);

                    if (iRet == sizeof(unsigned short)) // ??
                    {
                        rawImg.m_pHeader->m_iFirgelli = m_iFirgelliVal;
                        rawImg.save(sPath.c_str());
                        log.Log(StringBuilder() << sPath);
                        emit savedImg();
                    }
                    else
                    {
                        PWImage8 imgRawUC(
                                    PWImage8::PW_RAW_8BIT,
                                    uWidth,
                                    uHeight);
                        rawImg.CopyAndScaleTopoTo8Bit(imgRawUC.getImagePointer(),imgRawUC.m_pHeader->m_uSizeBytes);
                        imgRawUC.m_pHeader->m_iFirgelli = m_iFirgelliVal;
                        imgRawUC.save(sPath.c_str());
                        log.Log(StringBuilder() << sPath);
                        emit savedImg();
                    }




                    if (m_bSaveOne)
                        emit saveSnap("Snapped " + QString::fromStdString(StringBuilder() << Time(Time::Local).ToExtendedISO() << ".lvt"));
                    m_bSaveOne = false;
                    m_dSaveX = 0;
                    m_dSaveY = 0;
                }

                // check if black
                {
                    double sum = accumulate( rawImg.getImagePointer(), rawImg.getImagePointer() + rawImg.m_pHeader->m_uSizeBytes , 0, plus<int>());
                    double mean = sum / rawImg.m_pHeader->m_uSizeBytes;
                    //cout << "Img mean " << mean << endl;
                    if (mean < uThreshold)
                        emit darkImg();
                }



                BayerFilter::MalvarHeCutler(
                        rawImg.getImagePointer(),
                        rgbImg.getImagePointer(),
                        uWidth,
                        uHeight,
                        (BayerFilter::BAYER_FILTER_PATTERN)rawImg.m_pHeader->m_uBayerPattern,//(BayerFilter::BAYER_FILTER_PATTERN)m_uBayerPattern,//(ImageFilter::FILTER_PATTERN)uBayerTiling,//BAYER_PATTERN_RGGB,
                        rawImg.m_pHeader->m_uBitDepth//12//   KORJATTAVA PARAMETRIKSI
                        );


                BayerFilter::IMAGE_PROPERTIES props;
                BayerFilter::CalculateChannelMeans(
                        rawImg.getImagePointer(),
                        uWidth,uHeight,(BayerFilter::BAYER_FILTER_PATTERN)rawImg.m_pHeader->m_uBayerPattern,&props);
                imgUC.m_pHeader->m_uRedMean = props.m_uRedMean;
                imgUC.m_pHeader->m_uGreenMean = props.m_uGreenMean;
                imgUC.m_pHeader->m_uBlueMean = props.m_uBlueMean;

                rgbImg.m_pHeader->m_uRedMean = props.m_uRedMean;
                rgbImg.m_pHeader->m_uGreenMean = props.m_uGreenMean;
                rgbImg.m_pHeader->m_uBlueMean = props.m_uBlueMean;
                rgbImg.m_pHeader->m_uWidth = uWidth;
                rgbImg.m_pHeader->m_uHeight = uHeight;



                PWPixelRGB10BIT* pRgb = reinterpret_cast <PWPixelRGB10BIT*>( rgbImg.getImagePointer());

                switch (m_is)
                {
                        case RAW:
                            // ImageViewerWidgetia varten
                            if (false == rawImg.copyTo8Bit(imgUC.getImagePointer(),uWidth*uHeight))
                            {
                                cout << "Could not copy raw to 8 bit" << endl;
                            }

                            break;
                        case RGB:

                          /*  for (int i = 0; i < rawImg.m_pHeader->m_uSizeBytes; i++)
                            {
                                pRgb[i].cRed= pRgb[i].cRed>>(rawImg.m_pHeader->m_uBitDepth-4);
                                pRgb[i].cGreen= pRgb[i].cGreen>>(rawImg.m_pHeader->m_uBitDepth-4);
                                pRgb[i].cBlue= pRgb[i].cBlue>>(rawImg.m_pHeader->m_uBitDepth-4);
                            }*/
                            rgbImg.copyTo8Bit(reinterpret_cast<unsigned char*>(&sRGBLive[0+sizeof(PWImage8::PW_IMAGE_HEADER)]),uWidth*uHeight*3);
                            memcpy(&sRGBLive[0],rgbImg.m_pHeader, sizeof(PWImage8::PW_IMAGE_HEADER));
                            break;
                        case R:
                                rgbImg.copyChannelTo8Bit(imgUC.getImagePointer(),0,imgUC.m_pHeader->m_uSizeBytes, rawImg.m_pHeader->m_uBitDepth - 8);
                            break;
                        case G:
                                rgbImg.copyChannelTo8Bit(imgUC.getImagePointer(),1,imgUC.m_pHeader->m_uSizeBytes, rawImg.m_pHeader->m_uBitDepth - 8);
                            break;
                        case B:
                                rgbImg.copyChannelTo8Bit(imgUC.getImagePointer(),2,imgUC.m_pHeader->m_uSizeBytes, rawImg.m_pHeader->m_uBitDepth - 8);
                         break;
                } // switch

                {
                    unsigned uRawMsgSize = sizeof(PWImage8::PW_IMAGE_HEADER) + rawImg.m_pHeader->m_uSizeBytes*sizeof(unsigned short);
                    string sRaw(uRawMsgSize ,char(0));
                    rawImg.m_pHeader->m_dTime = Time(Time::Local).GetTime();

                    memcpy(&sRaw[0],rawImg.m_pHeader, sizeof(PWImage8::PW_IMAGE_HEADER));
                    memcpy(&sRaw[0 + sizeof(PWImage8::PW_IMAGE_HEADER)],rawImg.getImagePointer(),rawImg.m_pHeader->m_uSizeBytes*sizeof(unsigned short));
                    emit newRawImg(sRaw);                    
                }

                if (m_is != RGB)
                {
                    memcpy(&sLive[0],imgUC.m_pHeader, sizeof(PWImage8::PW_IMAGE_HEADER));
                    memcpy(&sLive[0+sizeof(PWImage8::PW_IMAGE_HEADER)],imgUC.getImagePointer(),imgUC.m_pHeader->m_uSizeBytes);
                    emit newImageAvailable(sLive);
                }
                else
                {
                    emit newImageAvailable(sRGBLive);
                }



            } // if
            else
            {
                cout << "Grab failed! " << iRet << endl;
                emit error(grabber.getLastError());
            }

        } // if not paused
    } // if not stop request
    grabber.stopGrabbing();
}

void CameraBufferWrapper::firgelli(int i)
{
    m_iFirgelliVal = i;
}

void CameraBufferWrapper::trigger(bool b)
{
    bTrigger = b;
}




void CameraBufferWrapper::calcFolderInThread(QString savetag, QString saveClass)
{
    using namespace std;
    Sleep(2000);

    QString imgFolder = "'" + qSaveFolder + "\\" + saveClass + "\\" + savetag + "'";
    QString resFolder = "'" + qSaveFolder + "\\" + saveClass + "\\" + "'";
    QString exe = "Labralaite_Wb_EXE_console.exe";//WbCalcResultsLabra.exe";
    cout << "About to run " << exe.toStdString() << endl;
    IniFile iniWbl("WbCalcResultsLabra.ini");  // workissa
    iniWbl.SetValue("Path", "images", imgFolder.toStdString());
    iniWbl.SetValue("Path", "results", resFolder.toStdString());
    if (0 == QString::compare(saveClass ,"PE", Qt::CaseInsensitive))
        iniWbl.SetValue("SampleType", "type", 1);
    if (0 == QString::compare(saveClass ,"Raw", Qt::CaseInsensitive))
        iniWbl.SetValue("SampleType", "type", 0);
    if (0 == QString::compare(saveClass ,"Printed", Qt::CaseInsensitive))
        iniWbl.SetValue("SampleType", "type", 2);

    iniWbl.Save();

    QProcess *p = new QProcess(this);
    p->setWorkingDirectory("C:/Pintaworks/bin");
    int i = 0;//
    bool b = p->startDetached(exe);

    if (!b)
        cout << "Error running exe " << exe.toStdString() << endl;

   //cout << "Returns " << i << endl;

}



