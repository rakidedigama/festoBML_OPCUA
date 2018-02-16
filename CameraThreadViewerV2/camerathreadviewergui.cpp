#include "camerathreadviewergui.h"
#include "ui_camerathreadviewergui.h"

#include <QFileDialog>
#include <QIcon>

#include "qmessagebox.h"
#include "IniFile.h"
#include <iostream>
#include "cameramessagehandler.h"



CameraThreadViewerGUI::CameraThreadViewerGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CameraThreadViewerGUI)
{
    ui->setupUi(this);

    ui->pbLights->setEnabled(false);
    ui->pbFocus->setEnabled(false);
    ui->btnFocusTest->setEnabled(false);

    m_bCalibratingLights = false;
    m_bFocusing = false;

    ui->btnStartSave->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));
    ui->btnEndSave->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaStop));
    ui->btnSnap->setIcon(QIcon(":/images/recbtn"));
    ui->btnEndSave->setDisabled(true);
    ui->pauseButton->setVisible(false);
    qRegisterMetaType<std::string>("std::string");

    msg_thread = new QThread(this);
    camMsgHandler = new CameraMessageHandler();
    camMsgHandler->moveToThread(msg_thread);
    msg_thread->start();





    m_uDarkImgCounter = 0;

    m_CameraBuffer = new CameraBufferWrapper();
    CameraBufferWrapper::IMAGE_SELECTION is = m_CameraBuffer->getIS();

    switch ((unsigned)is)
    {
        case (unsigned)CameraBufferWrapper::RAW:  ui->rawRadioButton->setChecked(true); break;
        case (unsigned)CameraBufferWrapper::RGB: ui->rgbRadioButton->setChecked(true);;break;
        case (unsigned)CameraBufferWrapper::R: ui->rRadioButton->setChecked(true);;break;
        case (unsigned)CameraBufferWrapper::G: ui->gRadioButton->setChecked(true);;break;
        case (unsigned)CameraBufferWrapper::B: ui->bRadioButton->setChecked(true);;break;
        //default: rawRadioChecked(true); break;
    }


    QObject::connect(ui->lblSaveFolder,SIGNAL(doubleClicked()),this,SLOT(saveFolderDoubleClick()));
    QObject::connect(ui->saveFolderLineEdit,SIGNAL(textChanged(QString)),this,SLOT(saveFolderLineEditChanged(QString)));


    QObject::connect(ui->pbFocus,SIGNAL(clicked()),this,SLOT(focusPressed()));
    QObject::connect(ui->btnFocusTest,SIGNAL(clicked()),this,SLOT(focusTestPressed()));



    QObject::connect(m_CameraBuffer,SIGNAL(newImageAvailable(std::string)),this,SLOT(updateImageViewer(std::string)));
    QObject::connect(ui->startButton, SIGNAL(clicked()),this,SLOT(startButtonPressed()));
    QObject::connect(ui->stopButton, SIGNAL(clicked()),this,SLOT(stopButtonPressed()));
    QObject::connect(ui->pauseButton,SIGNAL(clicked()),this,SLOT(pauseButtonPressed()));

    QObject::connect(ui->imageViewer,SIGNAL(FPSupdated()),this,SLOT(FPSUpdate()));


    ui->stopButton->setEnabled(false);


    QObject::connect(ui->rawRadioButton,SIGNAL(toggled(bool)),this, SLOT(rawRadioChecked(bool)));
    QObject::connect(ui->rgbRadioButton,SIGNAL(toggled(bool)),this, SLOT(rGBRadioChecked(bool)));
    QObject::connect(ui->rRadioButton,SIGNAL(toggled(bool)),this, SLOT(rRadioChecked(bool)));
    QObject::connect(ui->gRadioButton,SIGNAL(toggled(bool)),this, SLOT(gRadioChecked(bool)));
    QObject::connect(ui->bRadioButton,SIGNAL(toggled(bool)),this, SLOT(bRadioChecked(bool)));
    QObject::connect(ui->btnSaveFolder,SIGNAL(clicked()),this,SLOT(selectSaveFolder()));
    QObject::connect(ui->btnStartSave,SIGNAL(clicked()),this,SLOT(startSave()));
    QObject::connect(ui->btnEndSave,SIGNAL(clicked()),this,SLOT(endSave()));
    QObject::connect(ui->btnSnap,SIGNAL(clicked()),this,SLOT(snapPressed()));


    QObject::connect(this->m_CameraBuffer,SIGNAL(savedImg()),this,SLOT(saveCounter()));
    QObject::connect(this->m_CameraBuffer,SIGNAL(saveSnap(QString)),this,SLOT(snapMsg(QString)));


    m_uCounter = 0;
    m_bInitializedOnce = false;

    IniFile ini("CameraThreadViewer.ini");
    m_bUsingBeckhoff9050 = ini.GetSetValue("PLC", "Use9050",false,"plc type");

    m_qsSaveFolder = QString::fromStdString(ini.GetSetValue("Saving", "Folder", "C:\\temp", ""));        
    m_CameraBuffer->setSaveFolder(m_qsSaveFolder);
    ui->lblSaveFolder->setText(m_qsSaveFolder);

    m_bMetricFirgelli = ini.GetSetValue("Focusing", "MetricValues", false, "New focusing system mm");
    if (m_bMetricFirgelli)
    {
        ui->sbFirgelliVal->setMaximum(450);
        ui->sbFirgelliVal->setMinimum(0);
    }

    m_uFocusingStep = ini.GetSetValue("Focusing", "Step",250,"Step size");
    ui->sbFirgelliVal->setSingleStep(m_uFocusingStep);    

    m_qsFirgelliVariable = QString::fromStdString(ini.GetSetValue("Focusing", "Variable", ".g_axis3_o_iFirgelli", "Firgelli variable in PLC program"));
    //std::cout << "Firgelli variable " << m_qsFirgelliVariable.toStdString() << std::endl;

    m_sGardaIP = ini.GetSetValue("Led","ControllerIP","12.0.0.10", "LED IP address");

    if (ini.IsDirty())
        ini.Save();

    QTimer *timer = new QTimer(this);
    timer->setInterval(500);
    QObject::connect(timer,SIGNAL(timeout()),this, SLOT(blinkSaving()));
    timer->start();


    m_Focuser = new Focuser(this);
    QObject::connect(m_Focuser,SIGNAL(moveFirgelli(uint)),this,SLOT(moveFirgelli(uint)));
    //QObject::connect(this,SIGNAL(valueToFesto(uint)),camMsgHandler,SLOT(writeToFesto(uint)));
    QObject::connect(m_Focuser,SIGNAL(startImaging()),this,SLOT(startButtonPressed()));
    QObject::connect(m_Focuser,SIGNAL(focusOn(bool)),this,SLOT(focusingOn(bool)));
    QObject::connect(m_CameraBuffer,SIGNAL(newRawImg(std::string)),m_Focuser,SLOT(updateImage(std::string)));
    QObject::connect(m_Focuser,SIGNAL(bestFocus(uint)),this,SLOT(bestFocus(uint)));
   //QObject::connect(this,SIGNAL(zConfirmed()),m_Focuser,SIGNAL(confirmZ()));

    QObject::connect(camMsgHandler,SIGNAL(zConfirmed()),m_Focuser,SIGNAL(confirmZ()));




    QObject::connect(ui->sbFirgelliVal,SIGNAL(valueChanged(int)),this,SLOT(firgelliValueChanged(int)));
    unsigned uFocus = ini.GetSetValue("Focus", "Last",15000,"Last value used");
    ui->sbFirgelliVal->setValue(uFocus);

    QObject::connect(m_CameraBuffer,SIGNAL(imageSize(uint,uint,QString)),this,SLOT(imageSize(uint,uint,QString)));
    QObject::connect(m_CameraBuffer,SIGNAL(darkImg()),this,SLOT(darkImg()));
    QObject::connect(m_CameraBuffer,SIGNAL(error(QString)),this,SLOT(error(QString)));

    //QObject::connect(m_Focuser, SIGNAL(moveFirgelli(uint)),m_CameraBuffer,SLOT(firgelli(int)));
    //QObject::connect(ui->sbFirgelliVal,SIGNAL(valueChanged(int)),m_CameraBuffer,SLOT(firgelli(int)));

    m_lights = new LightCalibrator(this);
    QObject::connect(ui->pbLights,SIGNAL(clicked()), this,SLOT(lightsCalibPressed()));
    QObject::connect(this,SIGNAL(doLightsCalib(uint)),m_lights,SLOT(calibrateLights(uint)));

    QObject::connect(ui->pbLightsTest,SIGNAL(pressed()),this,SLOT(pbLightsTestPressed()));


    QObject::connect(m_CameraBuffer,SIGNAL(newRawImg(std::string)),m_lights,SLOT(updateImage(std::string)));
    QObject::connect(m_lights,SIGNAL(calibrationOn(bool)),this,SLOT(lightsCalibOn(bool)));
    QObject::connect(ui->cbTrigger,SIGNAL(toggled(bool)),m_CameraBuffer,SLOT(trigger(bool)));

    ui->cbTrigger->setChecked(ini.GetSetValue("Trigger", "Off",true,"Trigger setting"));

    bool bFocusAuto = ini.GetSetValue("Focusing", "ContiuneAutomatically", false, "Tama falselle jos ei halua etta fokustestia tehdaan automaagisesti");

    bool bExternalMessages = ini.GetSetValue("Messaging", "ListenToExternalMessage", false, "Jos otetaan vastaan kuvauspyyntoja robottiohjelmata");
    if (bExternalMessages)
//    {
//        m_topic = new QMQTopicInterface("CamThreadViewer", "PW","CameraThreadViewer.ini");
//        QObject::connect(m_topic,SIGNAL(msg(QString,QByteArray)),this,SLOT(newMessage(QString,QByteArray)));
//        //m_topic->subscribe("EXTERNAL_SHOOT_COMMANDS");
//        m_topic->subscribe("ZAXIS_CONFIRMS");
//        m_topic->start();
//    }

     m_topic = new QMQTopicInterface("CamThreadViewer", "PW","CameraThreadViewer.ini");
//    qDebug()<<"Starting messaging client";
//    QObject::connect(m_topic,SIGNAL(msg(QString,QByteArray)),this,SLOT(newMessage(QString,QByteArray)));
//    //m_topic->subscribe("EXTERNAL_SHOOT_COMMANDS");
//    m_topic->subscribe("ZAXIS_CONFIRMS");
//    m_topic->start();

    if (ini.IsDirty())
        ini.Save();

    //m_CameraBuffer->testFunc();

    m_timerFocus = new QTimer(this);
    QObject::connect(m_timerFocus,SIGNAL(timeout()),this,SLOT(doTimedFocus()));
    m_timerFocus->setInterval(1000*60*2);
    if (bFocusAuto)
        m_timerFocus->start();

//    QObject::connect(m_TwincatManager,SIGNAL(newValue(QString,qint16)),m_Focuser,SLOT(setLaser(QString,qint16)));
//    if (m_bUsingBeckhoff9050)
//    {
//        //QObject::connect(m_TwincatManager,SIGNAL(newValue(QString,qint16)),m_Focuser,SLOT(setLaser(QString,qint16)));
//        m_TwincatManager->add9050IntSymbol("Laser",61472,6,100);
//    }
//    else
//    {
//        m_TwincatManager->addIntSymbol(".g_axis2_i_iLaserValue",500);
//    }

    m_GardaPage = new FileDownloader(this);
    m_gardaTimer = new QTimer(this);
    m_gardaTimer->setInterval(5*60*1000);
    QObject::connect(m_GardaPage,SIGNAL(downloaded()),this,SLOT(gardaPageDownload()));
    QObject::connect(m_gardaTimer,SIGNAL(timeout()),this,SLOT(gardaPageRequest()));
    m_gardaTimer->start();

}

CameraThreadViewerGUI::~CameraThreadViewerGUI()
{

    IniFile ini("CameraThreadViewer.ini");
    ini.SetValue("Focus", "Last", ui->sbFirgelliVal->value());
    ini.SetValue("Trigger", "Off",ui->cbTrigger->isChecked());
    ini.Save();

    m_CameraBuffer->stopThread();
    delete m_CameraBuffer;
    delete ui;
}

void CameraThreadViewerGUI::resizeEvent(QResizeEvent *e)
{    
    double dWidth = QApplication::desktop()->screenGeometry().width();
    if ((double)ui->imageViewer->width()/dWidth < 0.7)
        ui->imageViewer->setMinimumWidth(dWidth*0.7);

    QMainWindow::resizeEvent(e);
}


void CameraThreadViewerGUI::updateImageViewer(std::string sImage)
{
    using namespace std;
//    switch (m_CameraBuffer->GetIS())
//    {
//        case RAW:case R: case G:case B:
//            m_CameraBuffer->GetImage(m_lvtChannel->GetImagePointer(),1392*1040,RAW);
//            ui->imageViewer->UpdateImage(m_lvtChannel->GetImagePointer(),1392,1040,false);
//            ui->label_2->setText(QString::number(m_uCounter++));
//            break;
//        case RGB:
//            m_CameraBuffer->GetImage(reinterpret_cast<unsigned char*>(m_lvtRGB->GetImagePointer()),1392*1040*3,RGB);
//            ui->imageViewer->UpdateImage(reinterpret_cast<unsigned char*>(m_lvtRGB->GetImagePointer()),1392,1040,true);
//            ui->label_2->setText(QString::number(m_uCounter++));
//            break;
//    }

    PWImage8::PW_IMAGE_HEADER *pHeader = reinterpret_cast <PWImage8::PW_IMAGE_HEADER*> (&sImage[0]);
    if (pHeader->m_litImageType != PWImage16::PW_RGB_10BIT)
        ui->imageViewer->UpdateImage(reinterpret_cast<unsigned char*>(&sImage[0] + sizeof(PWImage8::PW_IMAGE_HEADER)),pHeader->m_uWidth,pHeader->m_uHeight,false);
    else
        ui->imageViewer->UpdateImage(reinterpret_cast<unsigned char*>(&sImage[0] + sizeof(PWImage8::PW_IMAGE_HEADER)),pHeader->m_uWidth,pHeader->m_uHeight,true);

    ui->imageViewer->fitInView(0,0,pHeader->m_uWidth,pHeader->m_uHeight,Qt::KeepAspectRatio);

    QString str;
    str.append(QString("R: %1").arg(pHeader->m_uBlueMean));
    str.append(QString(" G: %1").arg(pHeader->m_uGreenMean));
    str.append(QString(" B: %1").arg(pHeader->m_uRedMean)); // Tonin mukaan nain pitas olla..
    ui->Rgblabel->setText(str);



 }


void CameraThreadViewerGUI::FPSUpdate()
{
    ui->FPSlabel->setText(QString::number(ui->imageViewer->GetFPS()) + " fps");

    if (ui->imageViewer->GetFPS() > 0 && !m_bCalibratingLights)
    {
        ui->pbFocus->setEnabled(true);
        ui->btnFocusTest->setEnabled(true);
    }
    else
    {
        ui->pbFocus->setEnabled(false);
        ui->btnFocusTest->setEnabled(false);
    }

    if (ui->imageViewer->GetFPS() > 0 && !m_bFocusing)
        ui->pbLights->setEnabled(true);
    else
        ui->pbLights->setEnabled(false);

}

void CameraThreadViewerGUI::selectSaveFolder()
{

    QString qsDir = QFileDialog::getExistingDirectory(this, tr("Select save folder"),
                                                 m_qsSaveFolder,
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if (qsDir.length() > 5)
    {
        m_qsSaveFolder = qsDir;
        ui->lblSaveFolder->setText(m_qsSaveFolder);
        m_CameraBuffer->setSaveFolder(m_qsSaveFolder);

        IniFile ini("CameraThreadViewer.ini");
        ini.SetValue("Saving", "Folder", m_qsSaveFolder.toStdString());
        ini.Save();
    }
}

void CameraThreadViewerGUI::startSave()
{
    m_uSaveCounter = 0;
    ui->btnStartSave->setDisabled(true);
    ui->btnEndSave->setDisabled(false);
    m_CameraBuffer->setSaving(true);

}

void CameraThreadViewerGUI::endSave()
{
    ui->btnStartSave->setDisabled(false);
    ui->btnEndSave->setDisabled(true);
    m_CameraBuffer->setSaving(false);
}

void CameraThreadViewerGUI::blinkSaving()
{
    if (m_CameraBuffer->isSaving() && false == ui->startButton->isEnabled())
    {
        if (ui->lblSaveFolder->text().isEmpty())
            ui->lblSaveFolder->setText(m_qsSaveFolder);
        else
            ui->lblSaveFolder->setText("");
    }
    else
        ui->lblSaveFolder->setText(m_qsSaveFolder);
}

void CameraThreadViewerGUI::saveCounter()
{
    m_uSaveCounter++;
    ui->lblCounter->setText(QString::number(m_uSaveCounter));
}

void CameraThreadViewerGUI::snapMsg(QString qs)
{
    ui->lblCounter->setText(qs);
}

void CameraThreadViewerGUI::snapPressed()
{
    m_CameraBuffer->saveOne();

}

void CameraThreadViewerGUI::saveFolderDoubleClick()
{
 /*   ui->lblSaveFolder->setVisible(false);
    ui->saveFolderLineEdit->setVisible(true);
    ui->saveFolderLineEdit->setText(ui->lblSaveFolder->text());
    QTimer::singleShot(5000,this, SLOT(saveFolderTimeout()));
    ui->saveFolderLineEdit->setSelection(0,ui->saveFolderLineEdit->text().length());
    ui->saveFolderLineEdit->setFocus();
*/
}

void CameraThreadViewerGUI::saveFolderTimeout()
{

}

void CameraThreadViewerGUI::saveFolderLineEditChanged(QString qs)
{

    if (ui->saveFolderLineEdit->text().length() > 0)
    {

        QString saveFolder = ui->lblSaveFolder->text() + "\\" + ui->saveFolderLineEdit->text();

        m_CameraBuffer->setSaveFolder(saveFolder);

        IniFile ini("CameraThreadViewer.ini");
        ini.SetValue("Saving", "Folder", m_qsSaveFolder.toStdString());
        ini.Save();
    }
}

void CameraThreadViewerGUI::focusPressed()
{
    if (ui->pbFocus->text() == "Focus")
    {
        //m_Focuser->start();
        QFuture<void> gsCameraThread = QtConcurrent::run(m_Focuser,&Focuser::run);
                    //this, &CameraThreadViewerGUI::moveFirgelli, 15);//m_Focuser,&Focuser::run);
    }
    else
        m_Focuser->cancelFocusing();

}

void CameraThreadViewerGUI::focusTestPressed()
{
    if (ui->pbFocus->text() == "Focus")
    {
        //m_Focuser->start();
        QFuture<void> gsCameraThread = QtConcurrent::run(m_Focuser,&Focuser::doTargetedFocusTest);
                    //this, &CameraThreadViewerGUI::moveFirgelli, 15);//m_Focuser,&Focuser::run);
    }
    else
        m_Focuser->cancelFocusing();

}

void CameraThreadViewerGUI::moveFirgelli(uint value)
{

   // send message to castpro tool to move.
    QTime timer;
    std::cout<< "Sending message Z axis"<<std::endl;
    timer.start();
    Json::Value msg;
    //Json::Value array;
    msg["type"] = "JSON";
    msg["name"] = "ZAXIS";
    msg["pos"] = value;
    qDebug()<<"Writing message to FESTO";
    m_topic->writeMessage("ZAXIS_COMMANDS","",false,"JSON",msg.toStyledString().c_str(),msg.toStyledString().length(),100);
    int eTime = timer.elapsed();
    std::cout << "Message sent: " << eTime <<std::endl;





    if (!ui->sbFirgelliVal->isEnabled())
    {
        ui->sbFirgelliVal->blockSignals(true);
        ui->sbFirgelliVal->setValue(value);
        ui->sbFirgelliVal->blockSignals(false);
    }

}

void CameraThreadViewerGUI::focusingOn(bool b)
{
    m_bFocusing = b;
    if (b)
    {               
        ui->pbFocus->setText("Cancel");
        ui->btnFocusTest->setText("Cancel");
        ui->sbFirgelliVal->setEnabled(false);
    }
    else
    {
        ui->pbFocus->setText("Focus");
        ui->btnFocusTest->setText("Focus test");
        ui->sbFirgelliVal->setEnabled(true);
    }
}

void CameraThreadViewerGUI::firgelliValueChanged(int iNewValue)
{
    //if (m_bUsingBeckhoff9050)
//        m_TwincatManager->set9050Value("Firgelli",61488,2,iNewValue);
   // else
//        m_TwincatManager->setValue(m_qsFirgelliVariable,(qint16 )iNewValue);
}

void CameraThreadViewerGUI::bestFocus(uint value)
{    
    ui->sbFirgelliVal->setValue(value);


    std::cout<< "Sending message best Focus Z value"<<std::endl;

    Json::Value msg;
    //Json::Value array;
    msg["type"] = "JSON";
    msg["name"] = "ZAXIS_BEST";
    msg["pos"] = value;
    qDebug()<<"Writing best focus to FESTO";
    m_topic->writeMessage("ZAXIS_COMMANDS","",false,"JSON",msg.toStyledString().c_str(),msg.toStyledString().length(),100);

    std::cout << "Best focus sent: " <<std::endl;
}

void CameraThreadViewerGUI::imageSize(unsigned w, unsigned h, QString src)
{
    ui->lblImfSize->setText("Size " + QString::number(w) + "," + QString::number(h) + " (" + src + ")");
}

void CameraThreadViewerGUI::darkImg()
{
    m_uDarkImgCounter++;
    ui->lblDark->setText("Dark images: " + QString::number(m_uDarkImgCounter));
}

void CameraThreadViewerGUI::error(QString)
{
}

void CameraThreadViewerGUI::lightsCalibPressed()
{
    if (ui->pbLights->text() == "Calibrate")
    {
        IniFile ini("CameraThreadViewer.ini");
        unsigned ulightsTarget = ini.GetSetValue("LightsCalibration", "TargetValue", 500);
        unsigned ulightsStart = ini.GetSetValue("LightsCalibration", "StartValue", 999);
        if (ini.IsDirty())
            ini.Save();

        m_lights->calibrate(ulightsTarget,0,0, ulightsStart);
    }
    else
        m_lights->cancel();

}

void CameraThreadViewerGUI::lightsCalibOn(bool b)
{
    m_bCalibratingLights = b;
    if (b)
    {

        ui->pbLights->setText("Cancel");
    }
    else
    {
        ui->pbLights->setText("Calibrate");
    }
}

void CameraThreadViewerGUI::pbLightsTestPressed()
{
    if (ui->pbLights->text() == "Test")
    {

        m_lights->test();
    }
    else
        m_lights->cancel();

}

void CameraThreadViewerGUI::doTimedFocus()
{
    if (!m_bFocusing)
        ui->btnFocusTest->animateClick();
}

void CameraThreadViewerGUI::newMessage(QString type, QByteArray data)
{
    using namespace std;
    cout<<"got new message in CameraThreadViewer. Reading type:" << type.toStdString() <<endl;
    qDebug()<<"Got Message in CTV" ;

    if (type == "EXTERNAL_SHOOT_COMMANDS")
    {
        Json::Value msg;
        Json::Reader reader;
        if(reader.parse(data.data(), msg))
        {
            cout << data.data() << endl;
            if (msg["name"] == "SHOOT_MSG")
            {
                double x = msg["x"].asDouble();
                double y = msg["y"].asDouble();
                QString tag = QString::fromStdString(msg["tag"].asString());
                QString cLass = QString::fromStdString(msg["class"].asString());

                m_CameraBuffer->saveOne(x,y,tag, cLass);
            }

            if (msg["name"] == "CALC_MSG")
            {
                QString tag = QString::fromStdString(msg["tag"].asString());
                QString cLass = QString::fromStdString(msg["class"].asString());
                QFuture<void> future = QtConcurrent::run(m_CameraBuffer,&CameraBufferWrapper::calcFolderInThread,tag, cLass);
            }

        }
        else
        {
            QMessageBox msgBox;
                msgBox.setText("Could not parse message." );
                msgBox.exec();
        }
    }

//    // z AXIS CONFIRMS FROM CASTRO TOOL
//    if (type.toStdString() == "ZAXIS_CONFIRMS")
//    {
//        Json::Value msg;
//        Json::Reader reader;
//        cout<<"Reading Z Axis CONFIRMS" <<endl;
//        if(reader.parse(data.data(), msg))
//        {
//            cout << data.data() << endl;
//            if(msg["name"].asString() == "ZMOTION_COMPLETED"){
//                    cout<<"Z Focus Motion confirmed" << endl;
//                    qDebug()<< "Z Motion Confirmation RECIEVED";
//                    emit zConfirmed();
//                }
//        }
//        else
//        {
//            QMessageBox msgBox;
//                msgBox.setText("Could not parse message." );
//                msgBox.exec();
//        }

//    }


}

void CameraThreadViewerGUI::newTwincatValue(QString s, qint16 v)
{
    using namespace std;
}

void CameraThreadViewerGUI::gardaPageRequest()
{
    m_GardaPage->sendRequest("http://" + QString::fromStdString(m_sGardaIP));
}

void CameraThreadViewerGUI::gardaPageDownload()
{
    using namespace  std;
    // Temperature: 43C
    QString qPage = QString::fromLatin1(m_GardaPage->downloadedData());
    int j = 0;
    Logger tempLog("log_gardaTemperature");
    while ((j = qPage.indexOf("Temperature", j)) != -1) {
        tempLog.Log(StringBuilder() << qPage.mid(j+13,3).toStdString());
        bool bParseFailed = false;
        //double dTemp = qPage.mid(j+13,2).toDouble(&bParseFailed);
        if (!bParseFailed)
            cout << "Temperature parse failed: " << qPage.mid(j+13,2).toStdString() << endl;
        ++j;

    }
}




void CameraThreadViewerGUI::startButtonPressed()
{
    m_uDarkImgCounter = 0;
    using namespace std;    
    m_CameraBuffer->setPause(false);
    m_CameraBuffer->start();

    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    m_bInitializedOnce = true;
}

void CameraThreadViewerGUI::stopButtonPressed()
{    
    m_CameraBuffer->setPause(true);

    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
}

void CameraThreadViewerGUI::pauseButtonPressed()
{
    ui->pauseButton->setCheckable(true);
    //ui->pauseButton->setChecked(m_CameraBuffer->pause());
}


void  CameraThreadViewerGUI::rawRadioChecked(bool bToggle)
{
    if (bToggle)
        m_CameraBuffer->setIS(CameraBufferWrapper::RAW);

}

void CameraThreadViewerGUI::rGBRadioChecked(bool bToggle)
{
    if (bToggle){
        m_CameraBuffer->setIS(CameraBufferWrapper::RGB);
        qDebug() <<" Setting RGB" ;
    }

}

void CameraThreadViewerGUI::rRadioChecked(bool bToggle)
{
    if (bToggle)
        m_CameraBuffer->setIS(CameraBufferWrapper::R);

}

void CameraThreadViewerGUI::gRadioChecked(bool bToggle )
{
    if (bToggle)
        m_CameraBuffer->setIS(CameraBufferWrapper::G);

}
void CameraThreadViewerGUI::bRadioChecked(bool bToggle )
{
    if (bToggle)
        m_CameraBuffer->setIS(CameraBufferWrapper::B);

}



