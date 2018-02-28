#include "Focuser.h"

using namespace std;

Focuser::Focuser(QObject *parent) :
    QObject(parent),m_imageBuffer(5),logFocus(std::cout,"log_focusser")
{
    qRegisterMetaType<std::string>("std::string");
    m_bStopRequested = false;
//    m_bSendMessage = false;

    cout<<"Focussing constructor" <<endl;


    IniFile ini("CameraThreadViewer.ini");


//    unsigned uImagesToShoot = ini.GetSetValue("Focusing","ImagesToShoot", 10,"Monestako kuvasta lasketaan");
//    unsigned uImageTimeout = ini.GetSetValue("Focusing","ImagesAcquisitionTimeout", 500,"Virhe jos ei nain moneen ms ole tullut kuvaa");
//    bool bSaveFocusImages = ini.GetSetValue("Focusing","SaveFocusImages", false,"");

//    unsigned uFirgelliValueStart = ini.GetSetValue("Focusing","FirgelliStart",10000,"Firgelli start point");
//    unsigned uFirgelliValueEnd = ini.GetSetValue("Focusing","FirgelliEnd",16000,"Firgelli end point");
//    unsigned uFirgelliInterval = ini.GetSetValue("Focusing","FirgelliInterval",1000,"Firgelli move interval");
//    unsigned uFirgelliSteps = ((uFirgelliValueEnd - uFirgelliValueStart)/uFirgelliInterval) + 1;

//    unsigned uFirgelliSleep = ini.GetSetValue("Focusing","FirgelliSleep",1000,"Firgelli sleep ms");

//    bool bMetricFirgelli = ini.GetSetValue("Focusing", "MetricValues", false, "New focusing system mm");

    /*
    double uMetricValueStart = ini.GetSetValue("MetricFocusing","Start",100,"Focus start point mm*10");
    double uFirgelliValueEnd = ini.GetSetValue("MetricFocusing","End",450,"Focus end point mm*10");
    double uFirgelliInterval = ini.GetSetValue("MetricFocusing","Interval",10,"Focus move inteval");
    unsigned uFirgelliSteps = ((uFirgelliValueEnd - uFirgelliValueStart)/uFirgelliInterval) + 1;*/

    bool bReversed = ini.GetSetValue("Focusing","Reversed",false,"Kumpi suunta");

    m_sSaveFolder = ini.GetSetValue("Focusing","SaveFolder", "C:\\temp","");

    if (ini.IsDirty())
        ini.Save();



    m_topic = new QMQTopicInterface("CamThreadViewer", "PW","CameraThreadViewer.ini");
//    QObject::connect(m_topic,SIGNAL(msg(QString,QByteArray)),this,SLOT(newMessage(QString,QByteArray)));
//    std::cout<<"Subsribing to ZAXIS_CONFIRMS" << std::endl;
//    m_topic->subscribe("ZAXIS_CONFIRMS");
//    m_topic->start();

    QObject::connect(this,SIGNAL(calculateFocus()),this,SLOT(focusCamera()));
}


void Focuser::sendMessage(){ // focus pressed
    emit focusOn(true);
    QTime timer;
    cout<< "Sending message Z axis"<<endl;
    timer.start();
    Json::Value msg;
    //Json::Value array;
    msg["type"] = "JSON";
    msg["name"] = "ZAXIS";
    msg["pos"] = "25";

    m_topic->writeMessage("ZAXIS_COMMANDS","",false,"JSON",msg.toStyledString().c_str(),msg.toStyledString().length(),100);
    int eTime = timer.elapsed();
    cout << "Message sent: " << eTime <<endl;

    Sleep(10000); // sleep until z moves
    cout<<"starting Grabber from Focuser" <<endl;

    emit startImaging(); // start grabber

//    m_bSendMessage = false;

}


void Focuser::newMessage(QString type, QByteArray data){

    using namespace std;
    cout<<"Got new message in Focuser. Reading" <<endl;

    if (type.toStdString() == "ZAXIS_CONFIRMS")
    {
        Json::Value msg;
        Json::Reader reader;
        cout<<"Reading Z Axis CONFIRMS" <<endl;
        if(reader.parse(data.data(), msg))
        {
            //cout << data.data() << endl;
            cout << msg["type"] << endl;
            cout << msg["name"].asString() <<endl;
            if (msg["name"].asString() == "ZMOTION_COMPLETED")   {
                cout<<"Z Focus Motion confirmed" << endl;
            }
        }
        else
        {
            QMessageBox msgBox;
                msgBox.setText("Could not parse message." );
                msgBox.exec();
        }

    }
    else
        cout<<"Type unknown" <<endl;
}

//void Focuser::grabberStarted(){
//    cout<<"grabberStatus TRUE" <<endl;
//    grabberStatus = true;
//     focusCamera();//emit calculateFocus(); // connects to focusCamera method
//}

void Focuser::focusCamera(){


    cout <<"Grabber ok : Calculating focus method" <<endl;

        if (m_bStopRequested)
        {
            cout<<"stop requested : Exit focus" <<endl;
            emit focusOn(false);
            return;
        }
//        cout <<"Calculating focus method 2" <<endl;

        else{
            cout<<"Continue focus method" <<endl;

            bool bok = false;
            std::string img = m_imageBuffer.receive(500,&bok);
            cout<<"GOT IMAGE: " << img << endl;
           PWImage8::PW_IMAGE_HEADER *pHeader = reinterpret_cast <PWImage8::PW_IMAGE_HEADER*> (&img[0]);
            cout << pHeader->m_litImageType << endl;
            if (pHeader->m_litImageType != PWImage16::PW_RAW_10BIT){
                cout<<"Header Error" <<endl;
                //break; // hitto... :)
            }

            if (bok)
            {
                cout <<" bok true, got image" << endl;
                PWImage8::PW_IMAGE_HEADER *pHeader = reinterpret_cast <PWImage8::PW_IMAGE_HEADER*> (&img[0]);
                PWImage16 pImg(PWImage16::PW_RAW_10BIT,pHeader->m_uWidth,pHeader->m_uHeight);
                pImg.setImage(reinterpret_cast<unsigned short*>(&img[0] + sizeof(PWImage8::PW_IMAGE_HEADER)),pHeader->m_uWidth,pHeader->m_uHeight,false);

                PWImage16 imgDeb(PWImage16::PW_RAW_10BIT,pImg.m_pHeader->m_uSizeBytes * 3);
                BayerFilter::MalvarHeCutler(pImg.getImagePointer(),imgDeb.getImagePointer(),pImg.m_pHeader->m_uWidth,pImg.m_pHeader->m_uHeight,
                                            (BayerFilter::BAYER_FILTER_PATTERN)pHeader->m_uBayerPattern,pHeader->m_uBitDepth);

//                logFocus.Log(StringBuilder()<< "Bayer pattern " << (int)pHeader->m_uBayerPattern << " depth " << pHeader->m_uBitDepth);

                PWImage16 imgRed(PWImage16::PW_RAW_10BIT,pImg.m_pHeader->m_uWidth,pImg.m_pHeader->m_uHeight);
                imgDeb.copyChannel(imgRed.getImagePointer(),0,imgRed.m_pHeader->m_uSizeBytes,false);

                unsigned uCutSize = 300;
                PWImage16 imgRedCut(PWImage16::PW_CHANNEL_RED,uCutSize,uCutSize);
                imgRed.SubImage(imgRedCut.getImagePointer(),uCutSize,uCutSize,(imgRed.m_pHeader->m_uWidth - uCutSize)/2,(imgRed.m_pHeader->m_uHeight-uCutSize)/2);


                std::vector<uint16_t> vData;
                vData.assign(imgRedCut.getImagePointer(), imgRedCut.getImagePointer() + imgRedCut.m_pHeader->m_uSizeBytes);

                // nopeempi taap
                double sum = accumulate(imgRedCut.getImagePointer(), imgRedCut.getImagePointer() + imgRedCut.m_pHeader->m_uSizeBytes , 0, std::plus<int>());
                double mean = sum / imgRedCut.m_pHeader->m_uSizeBytes;

//                    double sum = std::accumulate(vData.begin(), vData.end(), 0.0);
//                    double mean = sum / vData.size();

                double sq_sum = std::inner_product(vData.begin(), vData.end(), vData.begin(), 0.0);
                double stdev = std::sqrt(sq_sum / vData.size() - mean * mean);
                //logFocus.Log(StringBuilder() << "FOCUS: Calculate stdev for image " << i << " "  << stdev << " at " << firgelliPositions.at(h).first);
                std::cout<< "FOCUS: Calculate stdev for image "  << " "  << stdev <<endl;


            }
            if(!bok)
                cout<<"could not get image" <<endl;

        }
    cout<<"Focus finishing" <<endl;
    emit focusOn(false);

}


/*

void Focuser::run()
{
    using namespace std;
    cout<<"Started Focussing" <<endl;

    emit focusOn(true);
    IniFile ini("CameraThreadViewer.ini");


    unsigned uImagesToShoot = ini.GetSetValue("Focusing","ImagesToShoot", 10,"Monestako kuvasta lasketaan");
    unsigned uImageTimeout = ini.GetSetValue("Focusing","ImagesAcquisitionTimeout", 500,"Virhe jos ei nain moneen ms ole tullut kuvaa");
    bool bSaveFocusImages = ini.GetSetValue("Focusing","SaveFocusImages", false,"");

    unsigned uFirgelliValueStart = ini.GetSetValue("Focusing","FirgelliStart",10000,"Firgelli start point");
    unsigned uFirgelliValueEnd = ini.GetSetValue("Focusing","FirgelliEnd",16000,"Firgelli end point");
    unsigned uFirgelliInterval = ini.GetSetValue("Focusing","FirgelliInterval",1000,"Firgelli move interval");
    unsigned uFirgelliSteps = ((uFirgelliValueEnd - uFirgelliValueStart)/uFirgelliInterval) + 1;

    unsigned uFirgelliSleep = ini.GetSetValue("Focusing","FirgelliSleep",1000,"Firgelli sleep ms");

    bool bMetricFirgelli = ini.GetSetValue("Focusing", "MetricValues", false, "New focusing system mm");

    /*
    double uMetricValueStart = ini.GetSetValue("MetricFocusing","Start",100,"Focus start point mm*10");
    double uFirgelliValueEnd = ini.GetSetValue("MetricFocusing","End",450,"Focus end point mm*10");
    double uFirgelliInterval = ini.GetSetValue("MetricFocusing","Interval",10,"Focus move inteval");
    unsigned uFirgelliSteps = ((uFirgelliValueEnd - uFirgelliValueStart)/uFirgelliInterval) + 1;

    bool bReversed = ini.GetSetValue("Focusing","Reversed",false,"Kumpi suunta");

    m_sSaveFolder = ini.GetSetValue("Focusing","SaveFolder", "C:\\temp","");

  /*
   * Pitas olla jo kuvan headerissa
    unsigned uBitShift = ini.GetSetValue("Images", "Bitshift", 4, "Imperx 10bit = 2, Basler 12bit = 4");
    unsigned uBayerPattern = ini.GetSetValue("Images", "Bayer", 2, "2 = RGGB (Imperx), 1 = GRBG (Basler)");
    unsigned uBitDepth = ini.GetSetValue("Images", "Depth", 10, "Imperx 10, Basler 12");

    if (ini.IsDirty())
        ini.Save();

    std::vector<std::pair<unsigned,double>> firgelliPositions;

    if (!bReversed)
    {
        for (int i = 0; i < uFirgelliSteps; i++)
        {
            std::pair<unsigned,unsigned>  pari;
            pari.first = (uFirgelliInterval*i) + uFirgelliValueStart;
            pari.second = 0;
            firgelliPositions.push_back(pari);
        }
    }
    else
    {
        for (int i = 0; i < uFirgelliSteps; i++)
        {
            std::pair<unsigned,unsigned>  pari;
            pari.first = uFirgelliValueEnd - (uFirgelliInterval*i);
            pari.second = 0;
            firgelliPositions.push_back(pari);
        }
    }

    Logger logFocus("log_focus");


    QString dateString = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString logFile = "focus_" + dateString + ".log";
    QFile file(logFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            logFocus.Log(StringBuilder() << "Could not open " << logFile.toStdString() << " for writing");
    QTextStream ts(&file);

    //emit startImaging();


    for (int h = 0; h < firgelliPositions.size(); h++)
    {
        if (m_bStopRequested)
        {
            emit focusOn(false);
            return;
        }

        unsigned uImagesFailed = 0;
        std::vector<double> devs;


        if (0 == h)
        {
            if (!bMetricFirgelli)
            {
                if (!bReversed)
                    emit moveFirgelli(firgelliPositions.at(h).first - 2000);
                else
                    emit moveFirgelli(firgelliPositions.at(h).first + 2000);
                Sleep(uFirgelliSleep*2);
            }
            else //uusi systeemi
            {
                if (!bReversed)
                    emit moveFirgelli(firgelliPositions.at(h).first - 20);
                else
                    emit moveFirgelli(firgelliPositions.at(h).first + 20);
                Sleep(uFirgelliSleep*2);

            }
        }

        emit moveFirgelli(firgelliPositions.at(h).first);
        Sleep(uFirgelliSleep);

        for (int i = 0; i < uImagesToShoot; i++)
        {

            if (m_bStopRequested)
            {
                emit focusOn(false);
                return;
            }

            bool bok = false;
            std::string img = m_imageBuffer.receive(500,&bok);            
           PWImage8::PW_IMAGE_HEADER *pHeader = reinterpret_cast <PWImage8::PW_IMAGE_HEADER*> (&img[0]);

            if (pHeader->m_litImageType != PWImage16::PW_RAW_10BIT)
                break; // hitto... :)

            if (bok)
            {
                PWImage8::PW_IMAGE_HEADER *pHeader = reinterpret_cast <PWImage8::PW_IMAGE_HEADER*> (&img[0]);
                PWImage16 pImg(PWImage16::PW_RAW_10BIT,pHeader->m_uWidth,pHeader->m_uHeight);
                pImg.setImage(reinterpret_cast<unsigned short*>(&img[0] + sizeof(PWImage8::PW_IMAGE_HEADER)),pHeader->m_uWidth,pHeader->m_uHeight,false);

                PWImage16 imgDeb(PWImage16::PW_RAW_10BIT,pImg.m_pHeader->m_uSizeBytes * 3);
                BayerFilter::MalvarHeCutler(pImg.getImagePointer(),imgDeb.getImagePointer(),pImg.m_pHeader->m_uWidth,pImg.m_pHeader->m_uHeight,
                                            (BayerFilter::BAYER_FILTER_PATTERN)pHeader->m_uBayerPattern,pHeader->m_uBitDepth);

                logFocus.Log(StringBuilder()<< "Bayer pattern " << (int)pHeader->m_uBayerPattern << " depth " << pHeader->m_uBitDepth);

                PWImage16 imgRed(PWImage16::PW_RAW_10BIT,pImg.m_pHeader->m_uWidth,pImg.m_pHeader->m_uHeight);
                imgDeb.copyChannel(imgRed.getImagePointer(),0,imgRed.m_pHeader->m_uSizeBytes,false);

                unsigned uCutSize = 300;
                PWImage16 imgRedCut(PWImage16::PW_CHANNEL_RED,uCutSize,uCutSize);
                imgRed.SubImage(imgRedCut.getImagePointer(),uCutSize,uCutSize,(imgRed.m_pHeader->m_uWidth - uCutSize)/2,(imgRed.m_pHeader->m_uHeight-uCutSize)/2);

                // hajonta / keskiarvo
                if (bSaveFocusImages)
                {
                    PWImage8 imgRed8(PWImage8::PW_CHANNEL_RED,pImg.m_pHeader->m_uWidth,pImg.m_pHeader->m_uHeight);
                    imgRed.copyTo8Bit(imgRed8.getImagePointer(),imgRed.m_pHeader->m_uSizeBytes);
                    std::string sFileName = StringBuilder() << m_sSaveFolder << "\\" << dateString.toStdString() << "\\"<< Time(Time::Local).ToExtendedISO() << "_FOCUS_" << firgelliPositions.at(h).first  << "_" << i <<".lvt";
                    imgRed.m_pHeader->m_iFirgelli = firgelliPositions.at(h).first;
                    logFocus.Log(StringBuilder() << "Save file " << sFileName);
                    QString qsFolder = QString::fromStdString(StringBuilder() << m_sSaveFolder << "\\" << dateString.toStdString());
                    if (!QDir().exists(qsFolder))
                        QDir().mkpath(qsFolder);
                    imgRed8.save(sFileName.c_str());

                }

                std::vector<unsigned short> vData;
                vData.assign(imgRedCut.getImagePointer(), imgRedCut.getImagePointer() + imgRedCut.m_pHeader->m_uSizeBytes);

                // nopeempi taap
                double sum = std::accumulate(vData.begin(), vData.end(), 0.0);
                double mean = sum / vData.size();

                double sq_sum = std::inner_product(vData.begin(), vData.end(), vData.begin(), 0.0);
                double stdev = std::sqrt(sq_sum / vData.size() - mean * mean);
                logFocus.Log(StringBuilder() << "FOCUS: Calculate stdev for image " << i << " "  << stdev << " at " << firgelliPositions.at(h).first);
                ts << firgelliPositions.at(h).first << ";" <<  i << ";" << stdev<<"\n";
                devs.push_back(stdev);


            }
          /*  else
            {
                if (pHeader->m_dTime >= 0)
                    logFocus.Log(StringBuilder()  << "Focuser: Image too old..."  << Time(pHeader->m_dTime).ToExtendedISO());
                else
                    logFocus.Log(StringBuilder()  << "Focuser: Image too old... dtime nan");
                uImagesFailed++;
            }
        }

        if (uImagesFailed >= uImagesToShoot-1)
        {
            logFocus.Log(StringBuilder() << "ERROR: failed to get " << uImagesToShoot << " images for focusing " << uImagesFailed);
            logFocus.Log(StringBuilder() << "Exiting");
            emit focusOn(false);
            return;
        }
        else
        {
            double sum = std::accumulate(devs.begin(), devs.end(), 0.0);
            double mean = sum / devs.size();
            logFocus.Log(StringBuilder() << "FOCUS: Calculate mean " << mean << " at " << firgelliPositions.at(h).first);
            firgelliPositions.at(h).second = mean;
        }
    }

    std::pair <uint,double> best;
    best.second = 0;

    for (int h = 0; h < firgelliPositions.size(); h++)
    {
        if (firgelliPositions.at(h).second > best.second)
        {
            best = firgelliPositions.at(h);
        }
    }
    logFocus.Log(StringBuilder() << "Best focus found at " << best.first);
    if (!bMetricFirgelli)
        emit bestFocus(best.first - 2000);
    else
        emit bestFocus(best.first - 20);

    Sleep(uFirgelliSleep*2);
    emit bestFocus(best.first);
    emit focusOn(false);
    m_bStopRequested = false;
}
*/

void Focuser::doTargetedFocusTest()
{
    IniFile ini("CameraThreadViewer.ini");
    bool bMetricFirgelli = ini.GetSetValue("Focusing", "MetricValues", false, "New focusing system mm");
    if (bMetricFirgelli)
        doTargetedFocusTestMetric();
    else
        doTargetedFocusTestFirgelli();

}


void Focuser::cancelFocusing()
{
    m_bStopRequested = true;
}

void Focuser::updateImage(std::string img)
{
    cout<<"Image buffer updated in Focuser: " << img << " size" << endl;
    /*if (mutex->tryLock(100))
    {
        unsigned uNextSlot = m_uLastImageSlot + 1;
        if (m_uLastImageSlot > 4)
            uNextSlot = 0;

        if (uNextSlot > m_imageBuffer.size() - 1)
            uNextSlot = 0;

        std::cout << "Write image " << uNextSlot << std::endl;
        m_imageBuffer[uNextSlot] = img;
        m_uLastImageSlot = uNextSlot;
        m_dLastImgUpdate = QDateTime::currentMSecsSinceEpoch();
        mutex->unlock();
        ::SetEvent(eventHandle);

    }*/

   /* PW_IMAGE_HEADER *pHeader = reinterpret_cast <PW_IMAGE_HEADER*> (&img[0]);
    if (boost::math::isnan<double>(pHeader->m_dTime))
    {
        std::cout << "Focuser: Insert NAN date img to buffer" << std::endl;
        return;
    }*/
    using namespace std;
    PWImage8::PW_IMAGE_HEADER *pHeader = reinterpret_cast <PWImage8::PW_IMAGE_HEADER*> (&img[0]);
    if (boost::math::isnan<double>(pHeader->m_dTime))
    {
        cout << "Insert NAN date img to buffer" << endl;
        return;
    }
    else if (pHeader->m_dTime == 0)
    {
        cout << "Zero date in image" << endl;
        return;
    }

    qDebug() << "Push image to buffer";
    m_imageBuffer.send(img);
}

void Focuser::setSaveFolder(QString qs)
{
    this->m_sSaveFolder = qs.toStdString();
}

void Focuser::setLaser(QString s, qint16 v)
{
    if (s == ".g_axis2_i_iLaserValue")
        m_iLaser = v;
}


std::string Focuser::getLastImage()
{
   /* unsigned uWaitTime = 0;
    while ((*dDate) == m_dLastImgUpdate && uWaitTime < 1000 && *dDate != 0)
    {
        Sleep(100);
        uWaitTime += 100;
    }
    if (*dDate == m_dLastImgUpdate && uWaitTime > 1000 )
    {
        *dDate = 0;
        return "";
    }*/

    /*if (WaitForSingleObject(eventHandle,1000) == WAIT_OBJECT_0)
    {
        if (mutex->tryLock(100))
        {
            std::cout << "Read "<<m_uLastImageSlot << std::endl;
            *dDate = m_dLastImgUpdate;
            return m_imageBuffer[m_uLastImageSlot];
            mutex->unlock();
        }
    }*/
    //
    return "";
}




//void Focuser::doTargetedFocusTestFirgelli()
//{
//    emit focusOn(true);
//    IniFile ini("CameraThreadViewer.ini");


//    unsigned uImagesToShoot = ini.GetSetValue("Focusing","ImagesToShoot", 10,"Monestako kuvasta lasketaan");
//    unsigned uImageTimeout = ini.GetSetValue("Focusing","ImagesAcquisitionTimeout", 500,"Virhe jos ei nain moneen ms ole tullut kuvaa");
//    bool bSaveFocusImages = ini.GetSetValue("Focusing","SaveFocusImages", false,"");

//    //unsigned uFirgelliValueStart = ini.GetSetValue("Focusing","FirgelliStart",10000,"Firgelli start point");
//    //unsigned uFirgelliValueEnd = ini.GetSetValue("Focusing","FirgelliEnd",16000,"Firgelli end point");
//    //unsigned uFirgelliInterval = ini.GetSetValue("Focusing","FirgelliInterval",1000,"Firgelli move interval");
//    //unsigned uFirgelliSteps = ((uFirgelliValueEnd - uFirgelliValueStart)/uFirgelliInterval) + 1;

//    unsigned uFirgelliSleep = ini.GetSetValue("Focusing","FirgelliSleep",1000,"Firgelli sleep ms");

//    unsigned uFirgelliSetTarget = ini.GetSetValue("Focusing","FirgelliFocusPeak",10000,"Fokuspiikki");

//    bool bReversed = false;//ini.GetSetValue("Focusing","Reversed",false,"Kumpi suunta");
//    bool bSymmetric = true;

//    m_sSaveFolder = ini.GetSetValue("Focusing","SaveFolder", "C:\\temp","");

//  /*
//   * Pitas olla jo kuvan headerissa
//    unsigned uBitShift = ini.GetSetValue("Images", "Bitshift", 4, "Imperx 10bit = 2, Basler 12bit = 4");
//    unsigned uBayerPattern = ini.GetSetValue("Images", "Bayer", 2, "2 = RGGB (Imperx), 1 = GRBG (Basler)");
//    unsigned uBitDepth = ini.GetSetValue("Images", "Depth", 10, "Imperx 10, Basler 12");
//*/
//    if (ini.IsDirty())
//        ini.Save();

//    Logger logFocus("log_focus");

//    for (int step = 250; step < 501; step+=250)
//    {
//        if (m_bStopRequested)
//            return;

//        unsigned uFirgelliSteps = 16;
//        if (bSymmetric)
//        {
//            uFirgelliSteps = (double)((((double)uFirgelliSetTarget+2000) - ((double)uFirgelliSetTarget-2000))/(double)step) + 1;
//        }
//        else
//        {
//            uFirgelliSteps = (((uFirgelliSetTarget+2000) - (uFirgelliSetTarget-4000))/step) + 1;
//        }
//        logFocus.Log(StringBuilder()<< uFirgelliSteps << " steps from " << uFirgelliSetTarget-2000);

//        QVector<std::pair<unsigned,double>> firgelliPositions;

//        for (int i = 0; i < uFirgelliSteps; i++)
//        {
//            std::pair<unsigned,unsigned>  pari;
//            if (bSymmetric)
//                pari.first = (uFirgelliSetTarget-2000)  + (step*i);
//            else
//                pari.first = (uFirgelliSetTarget-4000)  + (step*i);
//            pari.second = 0;
//            if (!bReversed)
//                firgelliPositions.push_back(pari);
//            else
//                firgelliPositions.push_front(pari);
//        }




//        QString direction = "fw";
//        if (bReversed)
//            direction = "rw";

//        QString start = QString::number(firgelliPositions.at(0).first);
//        QString end = QString::number(firgelliPositions.at(firgelliPositions.size() - 1).first);
//        if (bReversed)
//        {
//            start = QString::number(firgelliPositions.at(firgelliPositions.size() - 1).first);
//            end = QString::number(firgelliPositions.at(0).first);
//        }


//        QString subFolder = QString::number(uFirgelliSetTarget) + "_" + direction + "_" + QString::number(step) +
//                "_" + start + "_" + end;

//        QString dateString = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");

//        QDir().mkpath(QDir::currentPath() + "\\" + subFolder);
//        QString logFile = QDir::currentPath() + "\\" + subFolder + "\\focus_" + dateString + ".log";
//        QFile file(logFile);
//        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//                logFocus.Log(StringBuilder() << "Could not open " << logFile.toStdString() << " for writing");
//        QTextStream ts(&file);

//        //emit startImaging();


//        for (int h = 0; h < firgelliPositions.size(); h++)
//        {
//            if (m_bStopRequested)
//            {
//                emit focusOn(false);
//                return;
//            }

//            unsigned uImagesFailed = 0;
//            std::vector<double> devs;


//            if (0 == h)
//            {
//                if (!bReversed)
//                    emit moveFirgelli(firgelliPositions.at(h).first - 2000);
//                else
//                    emit moveFirgelli(firgelliPositions.at(h).first + 2000);
//                Sleep(uFirgelliSleep*2);
//            }

//            emit moveFirgelli(firgelliPositions.at(h).first);
//            Sleep(uFirgelliSleep);

//            for (int i = 0; i < uImagesToShoot; i++)
//            {

//                if (m_bStopRequested)
//                {
//                    emit focusOn(false);
//                    return;
//                }

//                bool bok = false;
//                std::string img = m_imageBuffer.receive(500,&bok);
//               PWImage8::PW_IMAGE_HEADER *pHeader = reinterpret_cast <PWImage8::PW_IMAGE_HEADER*> (&img[0]);

//                if (pHeader->m_litImageType != PWImage16::PW_RAW_10BIT)
//                    break; // hitto... :)

//                if (bok)
//                {
//                    PWImage8::PW_IMAGE_HEADER *pHeader = reinterpret_cast <PWImage8::PW_IMAGE_HEADER*> (&img[0]);
//                    PWImage16 pImg(PWImage16::PW_RAW_10BIT,pHeader->m_uWidth,pHeader->m_uHeight);
//                    pImg.setImage(reinterpret_cast<unsigned short*>(&img[0] + sizeof(PWImage8::PW_IMAGE_HEADER)),pHeader->m_uWidth,pHeader->m_uHeight,false);

//                    PWImage16 imgDeb(PWImage16::PW_RAW_10BIT,pImg.m_pHeader->m_uSizeBytes * 3);
//                    BayerFilter::MalvarHeCutler(pImg.getImagePointer(),imgDeb.getImagePointer(),pImg.m_pHeader->m_uWidth,pImg.m_pHeader->m_uHeight,
//                                                (BayerFilter::BAYER_FILTER_PATTERN)pHeader->m_uBayerPattern,pHeader->m_uBitDepth);

//                    logFocus.Log(StringBuilder()<< "Bayer pattern " << (int)pHeader->m_uBayerPattern << " depth " << pHeader->m_uBitDepth);

//                    PWImage16 imgRed(PWImage16::PW_RAW_10BIT,pImg.m_pHeader->m_uWidth,pImg.m_pHeader->m_uHeight);
//                    imgDeb.copyChannel(imgRed.getImagePointer(),0,imgRed.m_pHeader->m_uSizeBytes,false);

//                    unsigned uCutSize = 300;
//                    PWImage16 imgRedCut(PWImage16::PW_CHANNEL_RED,uCutSize,uCutSize);
//                    imgRed.SubImage(imgRedCut.getImagePointer(),uCutSize,uCutSize,(imgRed.m_pHeader->m_uWidth - uCutSize)/2,(imgRed.m_pHeader->m_uHeight-uCutSize)/2);

//                    // hajonta / keskiarvo
//                    if (bSaveFocusImages)
//                    {
//                        PWImage8 imgRed8(PWImage8::PW_CHANNEL_RED,pImg.m_pHeader->m_uWidth,pImg.m_pHeader->m_uHeight);
//                        imgRed.CopyAndScaleTopoTo8Bit(imgRed8.getImagePointer(),imgRed.m_pHeader->m_uSizeBytes);
//                                //(imgRed8.getImagePointer(),imgRed.m_pHeader->m_uSizeBytes, pHeader->m_uBitDepth - 8);
//                        std::string sFileName = StringBuilder() << m_sSaveFolder << "\\" << dateString.toStdString() << "\\"<< Time(Time::Local).ToExtendedISO() << "_FOCUS_" << firgelliPositions.at(h).first  << "_" << i <<".lvt";
//                        imgRed8.m_pHeader->m_iFirgelli = firgelliPositions.at(h).first;
//                        logFocus.Log(StringBuilder() << "Save file " << sFileName);
//                        QString qsFolder = QString::fromStdString(StringBuilder() << m_sSaveFolder << "\\" << dateString.toStdString());
//                        if (!QDir().exists(qsFolder))
//                            QDir().mkpath(qsFolder);
//                        imgRed8.save(sFileName.c_str());

//                    }

//                    std::vector<uint16_t> vData;
//                    vData.assign(imgRedCut.getImagePointer(), imgRedCut.getImagePointer() + imgRedCut.m_pHeader->m_uSizeBytes);

//                    // nopeempi taap
//                    double sum = accumulate(imgRedCut.getImagePointer(), imgRedCut.getImagePointer() + imgRedCut.m_pHeader->m_uSizeBytes , 0, std::plus<int>());
//                    double mean = sum / imgRedCut.m_pHeader->m_uSizeBytes;

////                    double sum = std::accumulate(vData.begin(), vData.end(), 0.0);
////                    double mean = sum / vData.size();

//                    double sq_sum = std::inner_product(vData.begin(), vData.end(), vData.begin(), 0.0);
//                    double stdev = std::sqrt(sq_sum / vData.size() - mean * mean);
//                    logFocus.Log(StringBuilder() << "FOCUS: Calculate stdev for image " << i << " "  << stdev << " at " << firgelliPositions.at(h).first);
//                    ts << firgelliPositions.at(h).first  << ";" << m_iLaser << ";" <<  i << ";" << stdev<<"\n";
//                    devs.push_back(stdev);


//                }
//              /*  else
//                {
//                    if (pHeader->m_dTime >= 0)
//                        logFocus.Log(StringBuilder()  << "Focuser: Image too old..."  << Time(pHeader->m_dTime).ToExtendedISO());
//                    else
//                        logFocus.Log(StringBuilder()  << "Focuser: Image too old... dtime nan");
//                    uImagesFailed++;
//                }*/
//            }

//            if (uImagesFailed >= uImagesToShoot-1)
//            {
//                logFocus.Log(StringBuilder() << "ERROR: failed to get " << uImagesToShoot << " images for focusing " << uImagesFailed);
//                logFocus.Log(StringBuilder() << "Exiting");
//                emit focusOn(false);
//                return;
//            }
//            else
//            {
//                double sum = std::accumulate(devs.begin(), devs.end(), 0.0);
//                double mean = sum / devs.size();
//                logFocus.Log(StringBuilder() << "FOCUS: Calculate mean " << mean << " at " << firgelliPositions.at(h).first);
//                //firgelliPositions.at(h).second = mean;
//                firgelliPositions[h].second = mean;
//            }
//        }

//        std::pair <uint,double> best;
//        best.second = 0;

//        for (int h = 0; h < firgelliPositions.size(); h++)
//        {
//            if (firgelliPositions.at(h).second > best.second)
//            {
//                best = firgelliPositions.at(h);
//            }
//        }
//        logFocus.Log(StringBuilder() << "Best focus found at " << best.first);

//        // homman loppu
//        if (!bSymmetric && step == 500 && bReversed)
//        {
//            m_bStopRequested = false;
//            emit focusOn(false);
//            return;
//        }

//        // ajetaan toinen laaki epasymmetrisesti
//        if (bSymmetric && step == 500)
//        {
//            bSymmetric = false;
//            step = 0;
//        }

//        // vaihdetaan reverseen
//        if(!bSymmetric && step == 500)
//        {
//            bSymmetric = true;
//            step = 0;
//            bReversed = true;
//        }


//    }

//    m_bStopRequested = false;

//}


//void Focuser::doTargetedFocusTestMetric()
//{
//    emit focusOn(true);
//    IniFile ini("CameraThreadViewer.ini");


//    unsigned uImagesToShoot = ini.GetSetValue("Focusing","ImagesToShoot", 10,"Monestako kuvasta lasketaan");
//    unsigned uImageTimeout = ini.GetSetValue("Focusing","ImagesAcquisitionTimeout", 500,"Virhe jos ei nain moneen ms ole tullut kuvaa");
//    bool bSaveFocusImages = ini.GetSetValue("Focusing","SaveFocusImages", false,"");

//    //unsigned uFirgelliValueStart = ini.GetSetValue("Focusing","FirgelliStart",10000,"Firgelli start point");
//    //unsigned uFirgelliValueEnd = ini.GetSetValue("Focusing","FirgelliEnd",16000,"Firgelli end point");
//    //unsigned uFirgelliInterval = ini.GetSetValue("Focusing","FirgelliInterval",1000,"Firgelli move interval");
//    //unsigned uFirgelliSteps = ((uFirgelliValueEnd - uFirgelliValueStart)/uFirgelliInterval) + 1;

//    unsigned uFirgelliSleep = ini.GetSetValue("Focusing","FirgelliSleep",1000,"Firgelli sleep ms");

//    unsigned uFirgelliSetTarget = ini.GetSetValue("Focusing","FirgelliFocusPeak",10000,"Fokuspiikki");

//    bool bReversed = false;//ini.GetSetValue("Focusing","Reversed",false,"Kumpi suunta");
//    bool bSymmetric = true;

//    m_sSaveFolder = ini.GetSetValue("Focusing","SaveFolder", "C:\\temp","");

//  /*
//   * Pitas olla jo kuvan headerissa
//    unsigned uBitShift = ini.GetSetValue("Images", "Bitshift", 4, "Imperx 10bit = 2, Basler 12bit = 4");
//    unsigned uBayerPattern = ini.GetSetValue("Images", "Bayer", 2, "2 = RGGB (Imperx), 1 = GRBG (Basler)");
//    unsigned uBitDepth = ini.GetSetValue("Images", "Depth", 10, "Imperx 10, Basler 12");
//*/
//    if (ini.IsDirty())
//        ini.Save();

//    Logger logFocus("log_focus");

//    for (int step = 1; step < 3; step+=1)
//    {
//        if (m_bStopRequested)
//            return;

//        unsigned uFirgelliSteps = 16;
//        if (bSymmetric)
//        {
//            uFirgelliSteps = (double)((((double)uFirgelliSetTarget+20) - ((double)uFirgelliSetTarget-20))/(double)step) + 1;
//        }
//        else
//        {
//            uFirgelliSteps = (((uFirgelliSetTarget+20) - (uFirgelliSetTarget-40))/step) + 1;
//        }
//        logFocus.Log(StringBuilder()<< uFirgelliSteps << " steps from " << uFirgelliSetTarget-20);

//        QVector<std::pair<unsigned,double>> firgelliPositions;

//        for (int i = 0; i < uFirgelliSteps; i++)
//        {
//            std::pair<unsigned,unsigned>  pari;
//            if (bSymmetric)
//                pari.first = (uFirgelliSetTarget-20)  + (step*i);
//            else
//                pari.first = (uFirgelliSetTarget-40)  + (step*i);
//            pari.second = 0;
//            if (!bReversed)
//                firgelliPositions.push_back(pari);
//            else
//                firgelliPositions.push_front(pari);
//        }




//        QString direction = "fw";
//        if (bReversed)
//            direction = "rw";

//        QString start = QString::number(firgelliPositions.at(0).first);
//        QString end = QString::number(firgelliPositions.at(firgelliPositions.size() - 1).first);
//        if (bReversed)
//        {
//            start = QString::number(firgelliPositions.at(firgelliPositions.size() - 1).first);
//            end = QString::number(firgelliPositions.at(0).first);
//        }


//        QString subFolder = QString::number(uFirgelliSetTarget) + "_" + direction + "_" + QString::number(step) +
//                "_" + start + "_" + end;

//        QString dateString = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");

//        QDir().mkpath(QDir::currentPath() + "\\" + subFolder);
//        QString logFile = QDir::currentPath() + "\\" + subFolder + "\\focus_" + dateString + ".log";
//        QFile file(logFile);
//        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//                logFocus.Log(StringBuilder() << "Could not open " << logFile.toStdString() << " for writing");
//        QTextStream ts(&file);

//        //emit startImaging();


//        for (int h = 0; h < firgelliPositions.size(); h++)
//        {
//            if (m_bStopRequested)
//            {
//                emit focusOn(false);
//                return;
//            }

//            unsigned uImagesFailed = 0;
//            std::vector<double> devs;


//            if (0 == h)
//            {
//                if (!bReversed)
//                    emit moveFirgelli(firgelliPositions.at(h).first - 20);
//                else
//                    emit moveFirgelli(firgelliPositions.at(h).first + 20);
//                Sleep(uFirgelliSleep*2);
//            }

//            emit moveFirgelli(firgelliPositions.at(h).first);
//            Sleep(uFirgelliSleep);

//            for (int i = 0; i < uImagesToShoot; i++)
//            {

//                if (m_bStopRequested)
//                {
//                    emit focusOn(false);
//                    return;
//                }

//                bool bok = false;
//                std::string img = m_imageBuffer.receive(500,&bok);
//               PWImage8::PW_IMAGE_HEADER *pHeader = reinterpret_cast <PWImage8::PW_IMAGE_HEADER*> (&img[0]);

//                if (pHeader->m_litImageType != PWImage16::PW_RAW_10BIT)
//                    break; // hitto... :)

//                if (bok)
//                {
//                    PWImage8::PW_IMAGE_HEADER *pHeader = reinterpret_cast <PWImage8::PW_IMAGE_HEADER*> (&img[0]);
//                    PWImage16 pImg(PWImage16::PW_RAW_10BIT,pHeader->m_uWidth,pHeader->m_uHeight);
//                    pImg.setImage(reinterpret_cast<unsigned short*>(&img[0] + sizeof(PWImage8::PW_IMAGE_HEADER)),pHeader->m_uWidth,pHeader->m_uHeight,false);

//                    PWImage16 imgDeb(PWImage16::PW_RAW_10BIT,pImg.m_pHeader->m_uSizeBytes * 3);
//                    BayerFilter::MalvarHeCutler(pImg.getImagePointer(),imgDeb.getImagePointer(),pImg.m_pHeader->m_uWidth,pImg.m_pHeader->m_uHeight,
//                                                (BayerFilter::BAYER_FILTER_PATTERN)pHeader->m_uBayerPattern,pHeader->m_uBitDepth);

//                    logFocus.Log(StringBuilder()<< "Bayer pattern " << (int)pHeader->m_uBayerPattern << " depth " << pHeader->m_uBitDepth);

//                    PWImage16 imgRed(PWImage16::PW_RAW_10BIT,pImg.m_pHeader->m_uWidth,pImg.m_pHeader->m_uHeight);
//                    imgDeb.copyChannel(imgRed.getImagePointer(),0,imgRed.m_pHeader->m_uSizeBytes,false);

//                    unsigned uCutSize = 300;
//                    PWImage16 imgRedCut(PWImage16::PW_CHANNEL_RED,uCutSize,uCutSize);
//                    imgRed.SubImage(imgRedCut.getImagePointer(),uCutSize,uCutSize,(imgRed.m_pHeader->m_uWidth - uCutSize)/2,(imgRed.m_pHeader->m_uHeight-uCutSize)/2);

//                    // hajonta / keskiarvo
//                    if (bSaveFocusImages)
//                    {
//                        PWImage8 imgRed8(PWImage8::PW_CHANNEL_RED,pImg.m_pHeader->m_uWidth,pImg.m_pHeader->m_uHeight);
//                        imgRed.copyTo8Bit(imgRed8.getImagePointer(),imgRed.m_pHeader->m_uSizeBytes);
//                        std::string sFileName = StringBuilder() << m_sSaveFolder << "\\" << dateString.toStdString() << "\\"<< Time(Time::Local).ToExtendedISO() << "_FOCUS_" << firgelliPositions.at(h).first  << "_" << i <<".lvt";
//                        imgRed8.m_pHeader->m_iFirgelli = firgelliPositions.at(h).first;
//                        logFocus.Log(StringBuilder() << "Save file " << sFileName);
//                        QString qsFolder = QString::fromStdString(StringBuilder() << m_sSaveFolder << "\\" << dateString.toStdString());
//                        if (!QDir().exists(qsFolder))
//                            QDir().mkpath(qsFolder);
//                        imgRed8.save(sFileName.c_str());

//                    }

//                    std::vector<uint16_t> vData;
//                    vData.assign(imgRedCut.getImagePointer(), imgRedCut.getImagePointer() + imgRedCut.m_pHeader->m_uSizeBytes);

//                    // nopeempi taap
//                    double sum = accumulate(imgRedCut.getImagePointer(), imgRedCut.getImagePointer() + imgRedCut.m_pHeader->m_uSizeBytes , 0, std::plus<int>());
//                    double mean = sum / imgRedCut.m_pHeader->m_uSizeBytes;

////                    double sum = std::accumulate(vData.begin(), vData.end(), 0.0);
////                    double mean = sum / vData.size();

//                    double sq_sum = std::inner_product(vData.begin(), vData.end(), vData.begin(), 0.0);
//                    double stdev = std::sqrt(sq_sum / vData.size() - mean * mean);
//                    logFocus.Log(StringBuilder() << "FOCUS: Calculate stdev for image " << i << " "  << stdev << " at " << firgelliPositions.at(h).first);
//                    ts << firgelliPositions.at(h).first  << ";" << m_iLaser << ";" <<  i << ";" << stdev<<"\n";
//                    devs.push_back(stdev);


//                }
//              /*  else
//                {
//                    if (pHeader->m_dTime >= 0)
//                        logFocus.Log(StringBuilder()  << "Focuser: Image too old..."  << Time(pHeader->m_dTime).ToExtendedISO());
//                    else
//                        logFocus.Log(StringBuilder()  << "Focuser: Image too old... dtime nan");
//                    uImagesFailed++;
//                }*/
//            }

//            if (uImagesFailed >= uImagesToShoot-1)
//            {
//                logFocus.Log(StringBuilder() << "ERROR: failed to get " << uImagesToShoot << " images for focusing " << uImagesFailed);
//                logFocus.Log(StringBuilder() << "Exiting");
//                emit focusOn(false);
//                return;
//            }
//            else
//            {
//                double sum = std::accumulate(devs.begin(), devs.end(), 0.0);
//                double mean = sum / devs.size();
//                logFocus.Log(StringBuilder() << "FOCUS: Calculate mean " << mean << " at " << firgelliPositions.at(h).first);
//                //firgelliPositions.at(h).second = mean;
//                firgelliPositions[h].second = mean;
//            }
//        }

//        std::pair <uint,double> best;
//        best.second = 0;

//        for (int h = 0; h < firgelliPositions.size(); h++)
//        {
//            if (firgelliPositions.at(h).second > best.second)
//            {
//                best = firgelliPositions.at(h);
//            }
//        }
//        logFocus.Log(StringBuilder() << "Best focus found at " << best.first);

//        // homman loppu
//        if (!bSymmetric && step == 2 && bReversed)
//        {
//            m_bStopRequested = false;
//            emit focusOn(false);
//            return;
//        }

//        // ajetaan toinen laaki epasymmetrisesti
//        if (bSymmetric && step == 2)
//        {
//            bSymmetric = false;
//            step = 0;
//        }

//        // vaihdetaan reverseen
//        if(!bSymmetric && step == 2)
//        {
//            bSymmetric = true;
//            step = 0;
//            bReversed = true;
//        }


//    }

//    m_bStopRequested = false;

//}
