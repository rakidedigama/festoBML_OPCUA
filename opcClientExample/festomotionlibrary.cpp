#include "festomotionlibrary.h"
#include <stdio.h>
#include "iostream"
#include <iomanip>
#include <cstdint>

using namespace std;




FestoMotionLibrary::FestoMotionLibrary( QObject* parent)
    : QThread(parent)
{

   bIsMoving = false;
   bIsEnabled = false;

  QObject::connect(this,SIGNAL(sigUpdatePosition()),this,SLOT(updatePosition()));


}



void FestoMotionLibrary::run()
{
    while (true)    {

        // updatePosition();
       //emit sigUpdatePosition();

    }
}


void FestoMotionLibrary::connect(UA_Client *someClient){
    client = someClient;
    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://192.168.1.21:4840");
    if(retval != UA_STATUSCODE_GOOD) {
        cout<<"Cannot connect" <<endl;
        UA_Client_delete(client);
        return;// (int)retval;
    }
    else{
        cout<<"Connected to PLC"<<endl;
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
        cout<<"Reading Enabled state" << outValue <<"STATUS:  " <<retvalReadEnabled<<endl;
        bIsEnabled = outValue;
     }
    else{
            cout<<"Cannot access Enabled variable" <<endl;
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
    cout<<"Setting Start to " << bStartValue << " STATUS   : " << retvalSetEnable<<endl;

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
        cout<<"Reading StartMotion state" << outValue <<"STATUS:  " <<retvalReadEnabled<<endl;
          bStart = outValue;
     }
    else{
            cout<<"Cannot access Enabled variable" <<endl;
    }

    return outValue;
    UA_Variant_deleteMembers(&variant);

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
    cout<<"Setting Abort to " << bReset << " STATUS   : " << retvalSetEnable<<endl;
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
        cout<<"Reading IsHomed state: " << outValue <<"STATUS  " <<retvalReadEnabled<<endl;

    }
    else{
            cout<<"Cannot access IsHomed variable" <<endl;
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
            cout<<"Reading IsMotionComplete state: " << outValue <<"STATUS  " <<retvalReadEnabled<<endl;
        }
        bMotionComplete = outValue;
    }
    else{
            cout<<"Cannot access IsMotionComplete variable" <<endl;
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
        outValue = *(bool*)variant.data;
        cout<<"Reading IsMoving state: " << outValue <<"STATUS  " <<retvalReadEnabled<<endl;
         bIsMoving = outValue;
    }
    else{
            cout<<"Cannot access IsMoving variable" <<endl;
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
        cout<<"Reading IsBusy state: " << outValue <<"STATUS  " <<retvalReadEnabled<<endl;
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
        cout<<"Reading IsError state: " << outValue <<"STATUS  " <<retvalReadEnabled<<endl;
    }
    else{
            cout<<"Cannot access IsError variable" <<endl;
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

    UA_StatusCode retvalSetEnable = UA_Client_writeValueAttribute(client,nodeID,&variant);
    cout<<"Change MOP to " << modeNum << " STATUS   : " << retvalSetEnable<<endl;
}

void FestoMotionLibrary::setMOP(bool bSetMOP){


    UA_Variant variant;
    UA_Variant_init(&variant);
    char  symSetMOP[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stCW.xSetOpMode";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symSetMOP);

    UA_Variant_setScalar(&variant, &bSetMOP, &UA_TYPES[UA_TYPES_BOOLEAN]);

    UA_StatusCode retvalSetEnable = UA_Client_writeValueAttribute(client,nodeID,&variant);
    cout<<"Setting MOP to " << bSetMOP <<"STATUS  : " << retvalSetEnable<<endl;
    //UA_Variant_deleteMembers(&variant);

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
        cout<<"Reading actualPositionX: " << outValue <<"STATUS  " <<retvalReadEnabled<<endl;

    }
    else{
            cout<<"Cannot access actualPositionX variable" <<endl;
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
        cout<<"Reading actualPositionY: " << outValue <<"STATUS  " <<retvalReadEnabled<<endl;

    }
    else{
            cout<<"Cannot access actualPositionX variable" <<endl;
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
        //cout<<"Reading actualPositionZ: " << outValue <<"STATUS  " <<retvalReadEnabled<<endl;

    }
    else{
            cout<<"Cannot access actualPositionX variable" <<endl;
    }
    return outValue;
    UA_Variant_deleteMembers(&variant);
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
    cout<<"Setting ptpX to " << ptpX <<" STATUS  : " << retvalSetEnable<<endl;
    //UA_Variant_deleteMembers(&variant);
}


void FestoMotionLibrary::setPtpPosY(double ptpY){

    UA_Variant variant;
    UA_Variant_init(&variant);
   char symSetPtpY[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stPosPtp.lrY";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symSetPtpY);

    UA_Variant_setScalar(&variant, &ptpY, &UA_TYPES[UA_TYPES_DOUBLE]);

    UA_StatusCode retvalSetEnable = UA_Client_writeValueAttribute(client,nodeID,&variant);
    cout<<"Setting ptpY to " << ptpY <<" STATUS  : " << retvalSetEnable<<endl;
    //UA_Variant_deleteMembers(&variant);
}

void FestoMotionLibrary::setPtpPosZ(double ptpZ){


    UA_Variant variant;
    UA_Variant_init(&variant);
   char symSetPtpZ[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stPosPtp.lrZ";
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symSetPtpZ);

    UA_Variant_setScalar(&variant, &ptpZ, &UA_TYPES[UA_TYPES_DOUBLE]);

    UA_StatusCode retvalSetEnable = UA_Client_writeValueAttribute(client,nodeID,&variant);
    cout<<"Setting ptpZ to " << ptpZ <<" STATUS  : " << retvalSetEnable<<endl;
    //UA_Variant_deleteMembers(&variant);
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
}

void FestoMotionLibrary::setPtpCoordinates(double coordinates[], double bStartValue, double ptpVeloIn){
    if(!readStartMotion()){
        if(ptpVeloIn!=ptpVelo){
            ptpVelo = ptpVeloIn;
            setPTPVelo(ptpVelo);
        }
        startMotion(!bStartValue);
        setPtpPosX(coordinates[0]);
        setPtpPosY(coordinates[1]);
        setPtpPosZ(coordinates[2]);

        // set velocity


        if(isMoving()){
            cout<<" Already moving : Try later " <<endl;
        }
        if(bStartValue){// if true

            cout<<"Start Motion" <<endl;
            startPTPMotion();  // set xStart to true until ismoving is true

            while(isMoving() && isBusy()){  // wait until isBusy return false.
                // print axis values
                getActualPosX();
                getActualPosY();
            }

        cout<<"Motion Complete :  " <<endl;
        stopPTPMotion(); // set xStart bit back to false

        }

    }

    else{
        cout<<"Start not set to false : Toggle start bit" <<endl;
        startMotion(false);
    }


}
