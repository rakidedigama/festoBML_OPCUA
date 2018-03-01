#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "festomotionlibrary.h"
#include "open62541.h"
#include "messagehandler.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ini("CastProTool.ini")
{

    cout<<"Starting CastProTool" <<endl;
    ui->setupUi(this);

    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    m_thread = new QThread(this);
    fRobot = new FestoMotionLibrary();

    fRobot->connectPLC(client);
    fRobot->initialize();

    fRobot->moveToThread(m_thread);

    msg_thread = new QThread(this);
    mHandler = new MessageHandler();
    mHandler->moveToThread(msg_thread);
    msg_thread->start();



//    QObject::connect(this,SIGNAL(moveCamToFocus(double)),fRobot,SLOT(moveZtoFocus(double)));
    QObject::connect(mHandler,SIGNAL(moveCamToFocus(double)),fRobot,SLOT(moveZtoFocus(double)));

    //QObject::connect(fRobot,SIGNAL(ZMotionComplete()),this,SLOT(ZMotionConfirmMsg()));

    //QObject::connect(this,SIGNAL(systemEnableBtnPressed()),fRobot,SLOT(triggerSystemEnable()));

    //QObject::connect(this,SIGNAL(systemEnableBtnPressed()),this,SLOT(ZMotionConfirmMsg()));
    // connections to run PTP points in Festo
    QObject::connect(ui->btnRecipe,SIGNAL(pressed()),this,SLOT(runRecipe()));
    QObject::connect(this,SIGNAL(runRecipeSignal(QString)),fRobot,SLOT(processRecipe(QString)));
    QObject::connect(this,SIGNAL(sampleReferenceTag(QString)),fRobot,SLOT(getSampleRefTag(QString)));

    QObject::connect(fRobot,SIGNAL(error(QString)),this,SLOT(error(QString)));
    QObject::connect(this,SIGNAL(destroyed(QObject*)),m_thread,SLOT(quit()));

    QObject::connect(fRobot,SIGNAL(pos(double,double)),this,SLOT(pos(double,double)));

    //QObject::connect(fRobot,SIGNAL(shoot(double,double)),this,SLOT(shootImage(double,double)));
    QObject::connect(fRobot,SIGNAL(msg(QString)),this,SLOT(msg(QString)));

    QObject::connect(fRobot,SIGNAL(moving(bool)),this,SLOT(moving(bool)));
    QObject::connect(fRobot,SIGNAL(recipeFinished()),this,SLOT(recipeDone()));


    // UI connections

    // Jogging triggers
    QObject::connect(ui->btnDown,SIGNAL(pressed()),this,SLOT(down()));
    QObject::connect(ui->btnDown,SIGNAL(released()),this,SLOT(releaseDown()));
    QObject::connect(ui->btnUp,SIGNAL(pressed()),this,SLOT(up()));
    QObject::connect(ui->btnUp,SIGNAL(released()),this,SLOT(releaseUp()));
    QObject::connect(ui->btnRight,SIGNAL(pressed()),this,SLOT(right()));
    QObject::connect(ui->btnRight,SIGNAL(released()),this,SLOT(releaseRight()));
    QObject::connect(ui->btnLeft,SIGNAL(pressed()),this,SLOT(left()));
    QObject::connect(ui->btnLeft,SIGNAL(released()),this,SLOT(releaseLeft()));
    QObject::connect(ui->radioButton,SIGNAL(pressed()),this,SLOT(joggingMode()));

    // Z axis
    QObject::connect(ui->zAxisUpBtn,SIGNAL(pressed()),this,SLOT(zUp()));
     QObject::connect(ui->zAxisUpBtn,SIGNAL(released()),this,SLOT(zUpRelease()));
     QObject::connect(ui->zAxisDownBtn,SIGNAL(pressed()),this,SLOT(zDown()));
      QObject::connect(ui->zAxisDownBtn,SIGNAL(released()),this,SLOT(zDownRelease()));

    QObject::connect(this,SIGNAL(jogSignal(int)),fRobot,SLOT(handleJog(int)));

    QObject::connect(ui->verticalSlider,SIGNAL(valueChanged(int)),this,SLOT(vertSlider(int)));
    QObject::connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(horSlider(int)));
    QObject::connect(ui->verticalSlider,SIGNAL(sliderMoved(int)),this,SLOT(vertSliderMoved(int)));
    QObject::connect(ui->horizontalSlider,SIGNAL(sliderMoved(int)),this,SLOT(horSliderMoved(int)));


//    m_topic = new QMQTopicInterface("CastProTool", "PW","CastProTool.ini");
//    QObject::connect(m_topic,SIGNAL(msg(QString,QByteArray)),this,SLOT(newMessage(QString,QByteArray)));
//    m_topic->subscribe("ZAXIS_COMMANDS");
//    m_topic->start();


    currentRecipe = QString::fromStdString(ini.GetSetValue("Recipe", "File", "D:\\temp\\shit.json"));
    ui->lblRecipe->setText(currentRecipe);

    QObject::connect(ui->btnSelect,SIGNAL(pressed()),this,SLOT(recipeSelection()));
    QObject::connect(ui->btnStop,SIGNAL(pressed()),this,SLOT(stopRecipePressed()));


    ui->btnStop->setEnabled(false);
    //QObject::connect(ui->coordinates,SIGNAL(moveTo(QPointF)),this,SLOT(moveRobotTo(QPointF)));

    // PTP & Jog Velocities

    ui->ptpVelocity->setValue((double)ini.GetSetValue("UI","PTPvelo", 20));
    ui->dsbStepSize->setValue((double)ini.GetSetValue("UI", "StepSize", 20));
    QObject::connect(ui->dsbStepSize,SIGNAL(valueChanged(double)),this,SLOT(stepSizeChanged(double)));
    QObject::connect(ui->ptpVelocity,SIGNAL(valueChanged(int)),this,SLOT(ptpVelocityChanged(int)));
    //QObject::connect(fRobot,SIGNAL(ptpChanged(double)),ui->ptpVelocity->setVa;

    ui->verticalSlider->setVisible(false);
    ui->horizontalSlider->setVisible(false);

    std::string cLass = ini.GetSetValue("UI", "Class","PE");
    if (cLass == "Raw")
        ui->rbRaw->setChecked(true);
    if (cLass == "PE")
        ui->rbPe->setChecked(true);
    if (cLass == "Printed")
        ui->rbPrinted->setChecked(true);


    QObject::connect(ui->rbPe,SIGNAL(toggled(bool)),this,SLOT(rbChanged()));
    QObject::connect(ui->rbRaw,SIGNAL(toggled(bool)),this,SLOT(rbChanged()));
    QObject::connect(ui->rbPrinted,SIGNAL(toggled(bool)),this,SLOT(rbChanged()));
    ui->pbMoving->setStyleSheet("background-color:green;");
    if (ui->lblCoords->text().length() == 0)
          ui->pbMoving->setStyleSheet("background-color:red ;");


    ui->cbCalc->setChecked(ini.GetSetValue("UI", "Class",true));
    QObject::connect(ui->cbCalc,SIGNAL(toggled(bool)),this,SLOT(calcToggled(bool)));

    if (ini.IsDirty())
            ini.Save();

      m_thread->start();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::down()
{
  emit jogSignal(1);


}
void MainWindow::releaseDown()
{
  emit jogSignal(2);

}

void MainWindow::up()
{
   emit jogSignal(3);

}
void MainWindow::releaseUp()
{
    emit jogSignal(4);

}

void MainWindow::left()
{
    emit jogSignal(5);
}
void MainWindow::releaseLeft()
{
    emit jogSignal(6);
}

void MainWindow::right()
{    
    emit jogSignal(7);

}
void MainWindow::releaseRight(){
    emit jogSignal(8);
}

void MainWindow::zUp(){
    emit jogSignal(9);
}

void MainWindow::zUpRelease(){
    emit jogSignal(10);
}

void MainWindow::zDown(){
    emit jogSignal(11);
}

void MainWindow::zDownRelease(){
    emit jogSignal(12);
}


void MainWindow::error(QString s)
{
    ui->lblMsg->setText(QDateTime::currentDateTime().toString("hh:mm:ss") + " " + s);
}

void MainWindow::runRecipe()
{
    cout<<"run recipe button pressed" <<endl;
    ui->btnStop->setEnabled(true);
    ui->coordinates->clearGrid();
    QString refTag = ui->leTag->text();
    emit sampleReferenceTag(refTag);
   // readJsonFile(currentRecipe);

    cout<<"Sending recipe to festo" <<endl;
    emit runRecipeSignal(currentRecipe);




    //fRobot->runPoints(&points);
    if (ini.IsDirty())
        ini.Save();
}

void MainWindow::stopRecipePressed(){
    fRobot->stopRecipe();
}

void MainWindow::readJsonFile(QString recipe){
    using namespace std;

    //m_bStopRequested = false;
    //if (mutex->tryLock(10))
    {
        Json::Value root;
        Json::Reader reader;
        std::ifstream test(recipe.toStdString());
        bool parsingSuccessful = reader.parse( test, root );
        if ( !parsingSuccessful )
        {
            // report to the user the failure and their locations in the document.
            std::cout  << "Failed to parse configuration: "<< reader.getFormattedErrorMessages();
            emit error("Failed to parse " + recipe);
            //mutex->unlock();
           //return false;
        }
        else{
         points = root["points"];
         //ptpPointsFromFileReady = true;
          cout<<"Got PTP points from File" <<endl;
        }
    }
}

void MainWindow::disableAll(bool b)
{
    ui->btnDown->setEnabled(!b);
    ui->btnUp->setEnabled(!b);
    ui->btnLeft->setEnabled(!b);
    ui->btnRight->setEnabled(!b);
    ui->btnRecipe->setEnabled(!b);
    ui->leTag->setEnabled(!b);
    ui->btnStop->setEnabled(b);
    ui->groupBox->setEnabled(!b);

}

void MainWindow::pos(double x, double y)
{
    cout<<"slot pos" << x << " " << y << endl;
    ui->lblCoords->setText("Actual position: " + QString::number(x) + " , " + QString::number(y));

        ui->coordinates->setCurrentPoint(QPointF(x,y));
}

void MainWindow::vertSlider(int y)
{
//    robot->goTo(robot->getX(),y/10,robot->getZ());
}

void MainWindow::horSlider(int x)
{
//    robot->goTo((double)x/10,robot->getY(),robot->getZ());
}

void MainWindow::vertSliderMoved(int y)
{
    ui->lblSliderY->setText(QString::number((double)y/10));
}

void MainWindow::horSliderMoved(int x)
{
    ui->lblSliderX->setText(QString::number((double)x/10));
}

// Has been moved to festo
void MainWindow::shootImage(double x, double y)
{
    Json::Value msg;
    //Json::Value array;
    msg["type"] = "JSON";
    msg["name"] = "SHOOT_MSG";
    msg["tag"] = ui->leTag->text().toStdString();
    if (ui->rbRaw->isChecked())
        msg["class"] = "Raw";
    if (ui->rbPe->isChecked())
        msg["class"] = "PE";
    if (ui->rbPrinted->isChecked())
        msg["class"] = "Printed";

    msg["x"] = x;
    msg["y"] = y;
    //m_topic->writeMessage("EXTERNAL_SHOOT_COMMANDS","",false,"JSON",msg.toStyledString().c_str(),msg.toStyledString().length(),100);
    std::cout << "Shoot msg" << std::endl;
    //ui->coordinates->addPoint(QPointF(x,y));
}

void MainWindow::msg(QString msg)
{
    ui->lblMsg->setText(msg);
}

void MainWindow::recipeSelection()
{
    QString recipe = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    currentRecipe,
                                                    tr("Recipes (*.json)"));

    if (recipe.length() > 5)
    {
        currentRecipe = recipe;
        ini.SetValue("Recipe", "File", recipe.toStdString());
                ini.Save();
        ui->lblRecipe->setText(currentRecipe);
    }

}

//void MainWindow::stopPressed()
//{

//    //m_thread->quit();
//    //fRobot->abort(true);

//}

void MainWindow::moveRobotTo(QPointF p)
{
    cout<< "Moving in Grid" <<endl;
     double velo = fRobot->getPTPVelo();
     double z = fRobot->getActualPosZ();
     double coordinates[] = {double(p.x()),double(p.y()),z};
    fRobot->setPtpCoordinates(coordinates, true,velo);
    //this->robot->goTo(p.x(),p.y(),20);
}

void MainWindow::stepSizeChanged(double d)
{
//    ini.SetValue("UI", "StepSize",d);
//    ini.Save();
    fRobot->setJogVelo(d);
}

void MainWindow::ptpVelocityChanged(int v){
//    ini.SetValue("UI", "PTPvelo",v);
//    ini.Save();
    double velo = double(v);
    cout<<"Changing ptp velo" <<endl;
    fRobot->setPTPVelo(velo);
}

void MainWindow::rbChanged()
{
    std::string cLass = "";
    if (ui->rbRaw->isChecked())
        cLass = "RAW";
    if (ui->rbPe->isChecked())
        cLass = "PE";
    if (ui->rbPrinted->isChecked())
        cLass = "Printed";
    ini.SetValue("UI", "Class", cLass);
    ini.Save();

}

void MainWindow::moving(bool b)
{
    if (b)
        ui->pbMoving->setStyleSheet("background-color:red;");
    else
        ui->pbMoving->setStyleSheet("background-color:green;");
}



void MainWindow::recipeDone()
{
    ui->lblMsg->setText("Recipe Finished");

    if (ui->cbCalc->isChecked())
    {
        Json::Value msg;
        //Json::Value array;
        msg["type"] = "JSON";
        msg["name"] = "CALC_MSG";
        msg["tag"] = ui->leTag->text().toStdString();
        if (ui->rbRaw->isChecked())
            msg["class"] = "RAW";
        if (ui->rbPe->isChecked())
            msg["class"] = "PE";
        if (ui->rbPrinted->isChecked())
            msg["class"] = "Printed";

        cout<<"Sending message to Queue" <<endl;
        //m_topic->writeMessage("EXTERNAL_SHOOT_COMMANDS","",false,"JSON",msg.toStyledString().c_str(),msg.toStyledString().length(),100);
    }

}
//void MainWindow::zAxisMove(){
//    cout<<"Sending message Z axis" <<endl;
//    Json::Value msg;
//    //Json::Value array;
//    msg["type"] = "JSON";
//    msg["name"] = "CALC_MSG";
//    msg["tag"] = "Z axis";

//    m_topic->writ

//}

void MainWindow::calcToggled(bool b)
{
    ini.GetSetValue("UI", "Class",b);
    ini.Save();
}



void MainWindow::joggingMode()
{
    fRobot->changeMOP(30);
}


void MainWindow::on_pushButton_clicked()
{
    emit systemEnableBtnPressed();
}


void MainWindow::newMessage(QString type, QByteArray data)
{

    using namespace std;
    cout<<"got new message. Reading" <<endl;

    cout << "Got external msg TYPE:" << type.toStdString() << endl;

    if (type.toStdString() == "ZAXIS_COMMANDS")
    {
        Json::Value msg;
        Json::Reader reader;
        cout<<"Reading Z Axis commands" <<endl;
        if(reader.parse(data.data(), msg))
        {
            //cout << data.data() << endl;
//            cout << msg["type"] << endl;
//            cout << msg["name"].asString() << ":" << "\"ZAXIS\"" << endl;
//             cout << msg["pos"] << endl;


            if (msg["name"].asString() == "ZAXIS")
            {
                 std::string zposStr = msg["pos"].asString();
                double zPos = double( std::stoi (zposStr,nullptr,10));
                //double zpos = msg["pos"].asDouble();
                cout<<" Gotttttt message to move Z axis" << zPos << endl;
                //emit moveCamToFocus(zPos);

                //cout<<"Move to Zpos " << zpos <<endl;
            }
            else
                cout<<"CAnnot read" <<endl;

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


// Confirmation msg to Image Thread Viewer/Focusser that Z has moved to desired location
void MainWindow::ZMotionConfirmMsg(){
//    std::cout<<"Z motion completed. Sending confirmation message" <<endl;
//    Json::Value msg;
//    //Json::Value array;
//    msg["type"] = "JSON";
//    msg["name"] = "ZMOTION_COMPLETED";
//    //msg["tag"] = "MOTION_COMPLETED";
//    qDebug()<<"Z motion completed. Sending confirmation message";
//   m_topic->writeMessage("ZAXIS_CONFIRMS","",false,"JSON",msg.toStyledString().c_str(),msg.toStyledString().length(),100);
//    std::cout << "Message sent " << std::endl;

}
