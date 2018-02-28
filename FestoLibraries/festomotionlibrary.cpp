#include "festomotionlibrary.h"
#include <stdio.h>
#include "iostream"
#include "fstream"
#include <iomanip>
#include <cstdint>
#include "json.h"
#include "qmqtopicinterface.h"

using namespace std;


FestoMotionLibrary::FestoMotionLibrary( QObject* parent)

{

   bIsMoving = false;
   bIsEnabled = false;
   bStopRequested = false;
   //runPointsReady = false;

   ptpPointsFromFileReady  = false;
   recipeVelo = 20;
   ptpVelo = 20; // default velo
   jogVelo = 20;
   focusVelo = 2;
   mode = 50; // default ptp
  QObject::connect(this,SIGNAL(sigUpdatePosition()),this,SLOT(updatePosition()));
  m_topic = new QMQTopicInterface("Festo", "PW","CastProTool.ini");


}

void FestoMotionLibrary::connectPLC(UA_Client *someClient){
    client = someClient;
    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://192.168.1.21:4840");
    if(retval != UA_STATUSCODE_GOOD) {
        cout<<"Cannot connect" <<endl;
        UA_Client_delete(client);
        return;// (int)retval;
    }
    else{
        cout<<"Connected to PLC"<<endl;
//        initialize();

    }
}

void FestoMotionLibrary::initialize()
{
    cout<<"Initial Mode:" << viewMOP()<<endl;
    if(viewMOP()==0){
         cout<<"Start Configuration" <<endl;
         changeMOP(10);
         if(viewMOP()==10)
                cout<<"Configuration successful" <<endl;
    }


    //emit threadRunning(true);
    //System Initialization
    if(!(bIsEnabled&&bHomed)){
         cout<<"SYSTEM INITIALIZATION STARTING" <<endl;
         while(!isEnabled()){
            cout<<"..." <<endl;
            setEnable(true);
          }
          cout<<"SYSTEM ENABLED" <<endl;
          while(!isHomed()){
            homeSystem();
          }
          cout<<"SYSTEM HOMED" <<endl;
    }
    setPTPVelo(ptpVelo);
    setJogVelo(jogVelo);
    cout<<"System MOP" << viewMOP()<<endl;

}

void FestoMotionLibrary::triggerSystemEnable()
{
    if(isEnabled())
        setEnable(false);
    else
        setEnable(true);
}


bool FestoMotionLibrary::readJsonFile(QString recipe)
{
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
            return false;
        }
         points = root["points"];
         ptpPointsFromFileReady = true;
          cout<<"Got PTP points from File" <<endl;
    }

}



void FestoMotionLibrary::setEnable(bool bEnableValue){

    UA_Variant variant;
    UA_Variant_init(&variant);
    char  symEnable[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stCW.xEnable";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symEnable);

    UA_Variant_setScalar(&variant, &bEnableValue, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_StatusCode retvalSetEnable;

    // Wait until isEnabled value returns desired setValue
    while(bEnableValue!=isEnabled()){
         retvalSetEnable = UA_Client_writeValueAttribute(client,nodeID,&variant);
    }
     cout<<"Setting Enable to " << bEnableValue <<"STATUS  : " << retvalSetEnable<<endl;
    //UA_Variant_deleteMembers(&variant);

}

bool FestoMotionLibrary::isEnabled()
{
    UA_Variant variant;
    UA_Variant_init(&variant);
    char symIsEnabled[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stOut.stSW.xEnabled";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symIsEnabled);
    bool outValue;
    UA_StatusCode retvalReadEnabled = UA_Client_readValueAttribute(client, nodeID, &variant);
    if(retvalReadEnabled == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&variant,&UA_TYPES[UA_TYPES_BOOLEAN])) {
        outValue = *(bool*)variant.data;
        cout<<"Reading Enabled state" << outValue << endl ;;
        bIsEnabled = outValue;
     }
    else{
            cout<<"Cannot access Enabled variable"<<"STATUS CODE  " <<retvalReadEnabled<<endl;;
    }

    return outValue;
    UA_Variant_deleteMembers(&variant);

}

void FestoMotionLibrary::startMotion(bool bStartValue)
{
    UA_Variant variant;
    UA_Variant_init(&variant);
    char  symStart[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stCW.xStart";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symStart);

    UA_Variant_setScalar(&variant, &bStartValue, &UA_TYPES[UA_TYPES_BOOLEAN]);
     UA_StatusCode retvalSetEnable;
     // Wait until startMotion value returns desired start value
     while(bStartValue!=readStartMotion()){
        retvalSetEnable = UA_Client_writeValueAttribute(client,nodeID,&variant);
     }
    cout<<"Setting Start to " << bStartValue <<endl;

    //UA_Variant_deleteMembers(&variant);
}

bool FestoMotionLibrary::readStartMotion(){

    UA_Variant variant;
    UA_Variant_init(&variant);
    char  symStart[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stCW.xStart";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symStart);
    bool outValue;
    UA_StatusCode retvalReadEnabled = UA_Client_readValueAttribute(client, nodeID, &variant);
    if(retvalReadEnabled == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&variant,&UA_TYPES[UA_TYPES_BOOLEAN])) {
        outValue = *(bool*)variant.data;
        //cout<<"Reading StartMotion state" << outValue<< endl ;;
          bStart = outValue;
     }
    else{
            cout<<"Cannot access Enabled variable" <<"STATUS CODE  " <<retvalReadEnabled<<endl;;
    }

    return outValue;
    UA_Variant_deleteMembers(&variant);

}

void FestoMotionLibrary::movePositive(bool bMovePositive)
{
    UA_Variant variant;
    UA_Variant_init(&variant);
    char  symMovePos[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stCW.xMovePositive";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symMovePos);

    UA_Variant_setScalar(&variant, &bMovePositive, &UA_TYPES[UA_TYPES_BOOLEAN]);
     UA_StatusCode retvalSetEnable;

     retvalSetEnable = UA_Client_writeValueAttribute(client,nodeID,&variant);

    //cout<<"Setting Start to " << bStartValue <<endl;
}

void FestoMotionLibrary::moveNegative(bool bMoveNegative)
{
    UA_Variant variant;
    UA_Variant_init(&variant);
    char  symMoveNeg[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stCW.xMoveNegative";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symMoveNeg);

    UA_Variant_setScalar(&variant, &bMoveNegative, &UA_TYPES[UA_TYPES_BOOLEAN]);
     UA_StatusCode retvalSetEnable;

     retvalSetEnable = UA_Client_writeValueAttribute(client,nodeID,&variant);

}

void FestoMotionLibrary::changeAxis(int axis){
    UA_Variant variant;
    UA_Variant_init(&variant);
    char  symAxis[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.eSelectAxis";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symAxis);

    UA_Variant_setScalar(&variant, &axis, &UA_TYPES[UA_TYPES_SBYTE]);
     UA_StatusCode retvalSetEnable;

     retvalSetEnable = UA_Client_writeValueAttribute(client,nodeID,&variant);
}


void FestoMotionLibrary::abort(bool bAbort)
{
    UA_Variant variant;
    UA_Variant_init(&variant);
    char  symAbort[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stCW.xAbort";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symAbort);

    UA_Variant_setScalar(&variant, &bAbort, &UA_TYPES[UA_TYPES_BOOLEAN]);

    UA_StatusCode retvalSetEnable = UA_Client_writeValueAttribute(client,nodeID,&variant);
    cout<<"Setting Abort to " << bAbort << " STATUS   : " << retvalSetEnable<<endl;
    //UA_Variant_deleteMembers(&variant);
}

void FestoMotionLibrary::reset(bool bReset)
{
    UA_Variant variant;
    UA_Variant_init(&variant);
    char symReset[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stCW.xReset";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symReset);

    UA_Variant_setScalar(&variant, &bReset, &UA_TYPES[UA_TYPES_BOOLEAN]);

    UA_StatusCode retvalSetEnable = UA_Client_writeValueAttribute(client,nodeID,&variant);
    cout<<"Setting Reset to " << bReset << " STATUS   : " << retvalSetEnable<<endl;
    //UA_Variant_deleteMembers(&variant);
}

bool FestoMotionLibrary::isHomed()
{
    UA_Variant variant;
    UA_Variant_init(&variant);
    char symIsHomed[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stOut.stSW.xHomed";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symIsHomed);
    bool outValue;
    UA_StatusCode retvalReadEnabled = UA_Client_readValueAttribute(client, nodeID, &variant);
    if(retvalReadEnabled == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&variant,&UA_TYPES[UA_TYPES_BOOLEAN])) {
        outValue = *(bool*)variant.data;
        cout<<"Reading IsHomed state: " << outValue<< endl ; ;
        bHomed = outValue;
        if(bHomed){
            cout<<"SYSTEM ALREADY HOMED... " <<endl;
        }
        else
            cout<<"SYSTEM NOT HOMED YET" <<endl;

    }
    else{
            cout<<"Cannot access IsHomed variable" <<"STATUS CODE  " <<retvalReadEnabled<<endl;;
    }
    return outValue;
    UA_Variant_deleteMembers(&variant);
}

bool  FestoMotionLibrary::isMotionComplete()
{
    UA_Variant variant;
    UA_Variant_init(&variant);
    char symIsMotionComplete[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stOut.stSW.xMotionComplete";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symIsMotionComplete);
    bool outValue;
    UA_StatusCode retvalReadEnabled = UA_Client_readValueAttribute(client, nodeID, &variant);
    if(retvalReadEnabled == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&variant,&UA_TYPES[UA_TYPES_BOOLEAN])) {
        outValue = *(bool*)variant.data;
        if(outValue!=bMotionComplete){ // out only when changed
            cout<<"Reading IsMotionComplete state: " << outValue<< endl ; ;
        }
        bMotionComplete = outValue;
    }
    else{
            cout<<"Cannot access IsMotionComplete variable" <<"STATUS CODE  " <<retvalReadEnabled<<endl;;
    }
    return outValue;
    UA_Variant_deleteMembers(&variant);
}

bool FestoMotionLibrary::isMoving(){
    UA_Variant variant;
    UA_Variant_init(&variant);
    char symIsMoving[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stOut.stSW.xIsMoving";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symIsMoving);
    bool outValue;
    UA_StatusCode retvalReadEnabled = UA_Client_readValueAttribute(client, nodeID, &variant);
    if(retvalReadEnabled == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&variant,&UA_TYPES[UA_TYPES_BOOLEAN])) {
        if(bIsMoving!=outValue){ // Print output only when status changed
            //cout<<"Reading IsMoving state: " << outValue<< endl ; ;
        }
        outValue = *(bool*)variant.data;
        bIsMoving = outValue;
        if(bIsMoving){
            emit moving(true);
        }
        else{
            emit moving(false);
        }
    }
    else{
            cout<<"Cannot access IsMoving variable" <<"STATUS CODE  " <<retvalReadEnabled<<endl;
    }

    return outValue;
    UA_Variant_deleteMembers(&variant);
}

bool FestoMotionLibrary::isBusy(){
    UA_Variant variant;
    UA_Variant_init(&variant);
    char symIsBusy[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stOut.stSW.xBusy";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symIsBusy);
    bool outValue;
    UA_StatusCode retvalReadEnabled = UA_Client_readValueAttribute(client, nodeID, &variant);
    if(retvalReadEnabled == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&variant,&UA_TYPES[UA_TYPES_BOOLEAN])) {
        outValue = *(bool*)variant.data;
        //cout<<"Reading IsBusy state: " << outValue <<"STATUS  " <<retvalReadEnabled<<endl;
    }
    else{
            cout<<"Cannot access IsBusy variable" <<endl;
    }
    return outValue;
    UA_Variant_deleteMembers(&variant);

}

bool FestoMotionLibrary::isError(){
    UA_Variant variant;
    UA_Variant_init(&variant);
    char symIsError[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stOut.xError";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symIsError);
    bool outValue;
    UA_StatusCode retvalReadEnabled = UA_Client_readValueAttribute(client, nodeID, &variant);
    if(retvalReadEnabled == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&variant,&UA_TYPES[UA_TYPES_BOOLEAN])) {
        outValue = *(bool*)variant.data;
        cout<<"Reading IsError state: " << outValue << endl ;;
    }
    else{
            cout<<"Cannot access IsError variable" <<"STATUS CODE  " <<retvalReadEnabled<<endl;
    }
    return outValue;
    UA_Variant_deleteMembers(&variant);

}

void FestoMotionLibrary::changeMOP(int modeNum){

    UA_Variant variant;
    UA_Variant_init(&variant);
    char  symSetMOP[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.eMOP";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symSetMOP);

    UA_Variant_setScalar(&variant, &modeNum, &UA_TYPES[UA_TYPES_SBYTE]);
   UA_StatusCode retvalSetEnable;
    int cMode = 0;
    while( cMode!=modeNum){
        setMOP(false);
        retvalSetEnable = UA_Client_writeValueAttribute(client,nodeID,&variant); // write
        setMOP(true);
        cMode = viewMOP(); // read & set 'mode'


    }



    cout<<"Change MOP to " << modeNum << " STATUS   : " << retvalSetEnable<<endl;

}

void FestoMotionLibrary::setMOP(bool bSetMOP){


    UA_Variant variant;
    UA_Variant_init(&variant);
    char  symSetMOP[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stCW.xSetOpMode";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symSetMOP);

    UA_Variant_setScalar(&variant, &bSetMOP, &UA_TYPES[UA_TYPES_BOOLEAN]);

    UA_StatusCode retvalSetEnable = UA_Client_writeValueAttribute(client,nodeID,&variant);
    cout<<"Setting MOP bit to " << bSetMOP <<"STATUS  : " << retvalSetEnable<<endl;


}

int FestoMotionLibrary::viewMOP(){
    UA_Variant variant;
    UA_Variant_init(&variant);
    char symMOP[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stOut.eMoPDisplay";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symMOP);
    int outValue;
    UA_StatusCode retvalReadEnabled = UA_Client_readValueAttribute(client, nodeID, &variant);
    if(retvalReadEnabled == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&variant,&UA_TYPES[UA_TYPES_SBYTE])) {
        outValue = *(UA_SByte*)variant.data;
        mode = outValue;
        cout<<"MOP: " << outValue <<endl ;

    }
    else{
            cout<<"Cannot access MOP variable" << "STATUS CODE  " <<retvalReadEnabled<<endl;
    }
    return outValue;
    UA_Variant_deleteMembers(&variant);
}


double FestoMotionLibrary::getActualPosX(){

    UA_Variant variant;
    UA_Variant_init(&variant);
    char symActualposX[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stOut.stActualPositionMCS.lrX";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symActualposX);
    double outValue;
    UA_StatusCode retvalReadEnabled = UA_Client_readValueAttribute(client, nodeID, &variant);
    if(retvalReadEnabled == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&variant,&UA_TYPES[UA_TYPES_DOUBLE])) {
        outValue = *(UA_Double*)variant.data;
        cout<<"cX: " << outValue ;
        posX = outValue;

    }
    else{
            cout<<"Cannot access actualPositionX variable" << "STATUS CODE  " <<retvalReadEnabled<<endl;
    }
    return outValue;
    UA_Variant_deleteMembers(&variant);
}


double FestoMotionLibrary::getActualPosY(){

    UA_Variant variant;
    UA_Variant_init(&variant);
    char symActualposY[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stOut.stActualPositionMCS.lrY";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symActualposY);
    double outValue;
    UA_StatusCode retvalReadEnabled = UA_Client_readValueAttribute(client, nodeID, &variant);
    if(retvalReadEnabled == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&variant,&UA_TYPES[UA_TYPES_DOUBLE])) {

        outValue = *(UA_Double*)variant.data;
        cout<<", cY: " << outValue ;
        posY = outValue;

    }
    else{
            cout<<"Cannot access actualPositionX variable" << "STATUS CODE  " <<retvalReadEnabled<<endl;
    }
    return outValue;
    UA_Variant_deleteMembers(&variant);
}


double FestoMotionLibrary::getActualPosZ(){

    UA_Variant variant;
    UA_Variant_init(&variant);
    char symActualposZ[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stOut.stActualPositionMCS.lrZ";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symActualposZ);
    double outValue;
    UA_StatusCode retvalReadEnabled = UA_Client_readValueAttribute(client, nodeID, &variant);
    if(retvalReadEnabled == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&variant,&UA_TYPES[UA_TYPES_DOUBLE])) {

        outValue = *(UA_Double*)variant.data;
        posZ = outValue;
       cout<<"cZ: " << outValue << endl ;;

    }
    else{
            cout<<"Cannot access actualPositionX variable" <<"STATUS CODE " <<retvalReadEnabled<<endl;
    }
    return outValue;
    UA_Variant_deleteMembers(&variant);
}


void FestoMotionLibrary::startPTPMotion(){
    while(!isMoving()){
        startMotion(false);
        startMotion(true);
    }
}

void FestoMotionLibrary::stopPTPMotion(){
        startMotion(false);

}

void FestoMotionLibrary::homeSystem(){

    isHomed(); // check and set homed status first

    if(!bHomed){
        if(bStart )
            startMotion(false);   // make sure start bit is false
    // setMOP to false
       // setMOP(false);
        changeMOP(20);// homing mode
        //setMOP(true);
        while(!isMoving()){
            startMotion(true);
        }
        while(isBusy()||isMoving()){
            cout<<"Homing in progress" <<endl;
        }
        cout<< "Homing Complete: " << isHomed() <<endl;
        startMotion(false); // reset start to false
    }

}


void FestoMotionLibrary::getActualCoordinates(double coordinates[]){
    updatePosition();
    coordinates[0] = posX;
    coordinates[1] = posY;
    coordinates[2] = posZ;
    //return coordinates;
}

void FestoMotionLibrary::updatePosition(){
//    cout<<"Got signal emit"<<endl;
   posX = getActualPosX();
   posY = getActualPosY();
   posZ = getActualPosZ();
   //cout<<posX<<posY<<endl;
}

void FestoMotionLibrary::setPtpPosX(double ptpX){


    UA_Variant variant;
    UA_Variant_init(&variant);
   char symSetPtpX[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stPosPtp.lrX";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symSetPtpX);

    UA_Variant_setScalar(&variant, &ptpX, &UA_TYPES[UA_TYPES_DOUBLE]);

    UA_StatusCode retvalSetEnable = UA_Client_writeValueAttribute(client,nodeID,&variant);
    cout<<"Setting ptpX: " << ptpX ;
    if(retvalSetEnable!=0)
        cout<<" STATUS CODE  : " << retvalSetEnable<<endl;
    //UA_Variant_deleteMembers(&variant);
}


void FestoMotionLibrary::setPtpPosY(double ptpY){

    UA_Variant variant;
    UA_Variant_init(&variant);
   char symSetPtpY[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stPosPtp.lrY";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symSetPtpY);

    UA_Variant_setScalar(&variant, &ptpY, &UA_TYPES[UA_TYPES_DOUBLE]);

    UA_StatusCode retvalSetEnable = UA_Client_writeValueAttribute(client,nodeID,&variant);
    cout<<"ptpY: " << ptpY ;
    if(retvalSetEnable!=0)
        cout<<" STATUS CODE  : " << retvalSetEnable<<endl;
}

void FestoMotionLibrary::setPtpPosZ(double ptpZ){


    UA_Variant variant;
    UA_Variant_init(&variant);
   char symSetPtpZ[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stPosPtp.lrZ";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symSetPtpZ);

    UA_Variant_setScalar(&variant, &ptpZ, &UA_TYPES[UA_TYPES_DOUBLE]);

    UA_StatusCode retvalSetEnable = UA_Client_writeValueAttribute(client,nodeID,&variant);
    cout<<"ptpZ: " << ptpZ <<endl;
    if(retvalSetEnable!=0)
        cout<<" STATUS CODE  : " << retvalSetEnable<<endl;
}

void FestoMotionLibrary::setPTPVelo(double velo){
    UA_Variant variant;
    UA_Variant_init(&variant);
   char symPTPVelo[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stDynPtp.lrVel";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symPTPVelo);

    UA_Variant_setScalar(&variant, &velo, &UA_TYPES[UA_TYPES_DOUBLE]);

    UA_StatusCode retvalSetEnable = UA_Client_writeValueAttribute(client,nodeID,&variant);
    cout<<"Setting PTP Velocity to " << velo <<" STATUS  : " << retvalSetEnable<<endl;
    //UA_Variant_deleteMembers(&variant);
    emit ptpChanged(velo);

}

double FestoMotionLibrary::getPTPVelo(){
    return ptpVelo;
}

void FestoMotionLibrary::setJogVelo(double velo){
    UA_Variant variant;
    UA_Variant_init(&variant);
    char symJogVelo[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stDynJogInch.lrVel";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symJogVelo);

    UA_Variant_setScalar(&variant, &velo, &UA_TYPES[UA_TYPES_DOUBLE]);

    UA_StatusCode retvalSetEnable = UA_Client_writeValueAttribute(client,nodeID,&variant);
    jogVelo = velo;
    cout<<"Setting Jog Velocity to " << velo <<" STATUS  : " << retvalSetEnable<<endl;
    //UA_Variant_deleteMembers(&variant);

}




void FestoMotionLibrary::setPtpCoordinates(double coordinates[], double bStartValue, double ptpVeloIn){
    if(!readStartMotion()){
        if(isMoving()){
            cout<<" Already moving : Try later " <<endl;
        }
        // Set PtP velocity
        else{

            if(ptpVeloIn!=ptpVelo){
               ptpVelo = ptpVeloIn;
               setPTPVelo(ptpVelo);
            }
            startMotion(!bStartValue);
            setPtpPosX(coordinates[0]);
            setPtpPosY(coordinates[1]);
            setPtpPosZ(coordinates[2]);


            if(bStartValue){// if true
                cout<<"Start Position" <<endl;
                getActualPosX();
                getActualPosY();
                cout<<" " <<endl;

                cout<<"Start Motion" <<endl;
                startPTPMotion();  // set xStart to true until ismoving is true
                //startMotion(true);
                while(isMoving() && isBusy()){  // wait until isBusy return false.
                      // print axis values
                      getActualPosX();
                      getActualPosY();
                      getActualPosZ();
                      cout<<"Emitting pos" <<endl;
                      emit pos(posX,posY);
                 }

                    cout<<"Motion Complete :  " <<endl;

                 /*emit shoot(posX,posY);  */// shoot Image
                 //startMotion(false);
                 stopPTPMotion(); // set xStart bit back to false
             }
        }
    }
    else{
        cout<<"Start not set to false : Toggle start bit" <<endl;
        startMotion(false);
    }

}


void FestoMotionLibrary::jog(int axis,bool direction,bool toggle){

    // Initialization of Jog Run - Check enable, homed, mode

    if(viewMOP()!=30){

             changeMOP(30);
    }
    changeAxis(axis);
    if(toggle){ // motion. set true
        cout<<"Start Jog" << endl;
        if(direction)
            movePositive(true);

         else
             moveNegative(true);
    }
    else{
        cout<<"Stop Motion" <<endl;
        if(direction)
            movePositive(false);
         else
             moveNegative(false);
    }
    cout<<"Jog finished" <<endl;

}
void FestoMotionLibrary::checkSlot(){
    cout<<"Got Recipe from Mainwindow" <<endl;
}

void FestoMotionLibrary::moveZtoFocus(double Zp){
    double x = getActualPosX();
    double y = getActualPosY();
    double ptpCoordinates[] =  {x,y,Zp};
    cout<< " Moving Camera to Focus : " << " pX: " << ptpCoordinates[0]<<", pY: " <<ptpCoordinates[1]<<", pZ:  " << ptpCoordinates[2]<<endl;
    changeMOP(50);
    setPtpCoordinates(ptpCoordinates,true,focusVelo);
    if(abs(Zp - getActualPosZ())< 0.1){ // close enough
        cout<<"Z Focus Motion Complete in FESTO: " << endl;
        //emit ZMotionComplete();
        sendZConfirmMsg();
    }

}

void FestoMotionLibrary::sendZConfirmMsg(){
        std::cout<<"Writing Z AXIS confirmation message in FESTO" <<endl;
        Json::Value msg;
        //Json::Value array;
        msg["type"] = "JSON";
        msg["name"] = "ZMOTION_COMPLETED";
        msg["tag"] = "MOTION_COMPLETED";
       // qDebug()<<"Sending confirmation message in FESTO";
       m_topic->writeMessage("ZAXIS_CONFIRMS","",false,"JSON",msg.toStyledString().c_str(),msg.toStyledString().length(),100);
        std::cout << "Message sent from Festo " << std::endl;

}
void FestoMotionLibrary::getSampleRefTag(QString refTag){
    recipeRefTag = refTag;
}

void FestoMotionLibrary::sendShootMsg(){
    std::cout<<"Writing SHOOT message in FESTO" <<endl;
    Json::Value msg;
    //Json::Value array;
    msg["type"] = "JSON";
    msg["name"] = "PTP_SHOOT";
    msg["tag"] = recipeRefTag.toStdString();
   /* if (ui->rbRaw->isChecked())
        msg["class"] = "Raw";
    if (ui->rbPe->isChecked())
        msg["class"] = "PE";
    if (ui->rbPrinted->isChecked())
        msg["class"] = "Printed"; */
    msg["class"] = "Printed";
    msg["x"] = getActualPosX();
    msg["y"] = getActualPosY();

   m_topic->writeMessage("ZAXIS_CONFIRMS","",false,"JSON",msg.toStyledString().c_str(),msg.toStyledString().length(),100);
    std::cout << "SHOOT Message sent from Festo " << std::endl;





}

void FestoMotionLibrary::processRecipe(QString recipe){

    cout<<"Got Recipe " << recipe.toStdString()<< " from Mainwindow" <<endl;
    cout<<"Current Mode: " << mode <<endl;
     changeMOP(50);
    if(readJsonFile(recipe)){
        runPoints();
    }
    //emit recipeFinished();

}


void FestoMotionLibrary::runPoints(){

        changeMOP(50);
    //cout<<"Got signal from mainwindow" <<endl;

    // Initialization of PTP Run - Check enable, homed, mode
    if(bIsEnabled && bHomed){
        if(points.empty()){
            cout<<"Json Points Empty" <<endl;
        }
        else{
            // change to PTP

            startMotion(false); // setting start to false

            int pointsSize = points.size();
            cout<<"Number of points " <<pointsSize << endl;

            for(int index=0;index<pointsSize;index++){

                double x = points[index]["x"].asDouble();
                double y = points[index]["y"].asDouble();
                double z = points[index]["z"].asDouble();
                double ptpCoordinates[] =  {x,y,z};
                cout<<"pX: " << ptpCoordinates[0]<<", pY: " <<ptpCoordinates[1]<<", pZ:  " << ptpCoordinates[2]<<endl;
                setPtpCoordinates(ptpCoordinates,true,recipeVelo);
                //emit shoot(posX,posY);
                sendShootMsg();
            }
            points.clear();
        }

    }

    else
        cout<<"System not enabled, or not homed" <<endl;
    //emit finished(); // quit thread


}

void FestoMotionLibrary::run(){

    cout<<"Festo Run" <<endl;
}


void FestoMotionLibrary::handleJog(int jogVal){

    cout<<"Handling Jog"<< jogVal<<endl;


    switch(jogVal){
    case 1:  jog(1,true,true);  // start
                 break;
    case 2:  movePositive(false);  // stop
                   break;
    case 3:  jog(1,false,true);
        break;
    case 4: moveNegative(false);
        break;
    case 5: jog(2,true,true);
            break;
    case 6:   movePositive(false);
            break;
    case 7: jog(2,false,true);
            break;
    case 8: moveNegative(false);
            break;
    case 9:  jog(3,true,true);  // start
                 break;
    case 10:  movePositive(false);  // stop
                   break;
    case 11:  jog(3,false,true);
        break;
    case 12: moveNegative(false);
        break;

    }





}

//void FestoMotionLibrary::getJogHandle(int jogVal){
//    cout<<"Got Jog Signal" <<endl;
//    handleJog(jogVal);



//}

