
#include <QCoreApplication>
#include <stdio.h>
#include "open62541.h"
#include "iostream"
#include <iomanip>
#include <cstdint>
#include "festomotionlibrary.h"
#include "fstream"

#include "QTime"

// LIST OF SYMBOLS

// g_stIn




using namespace std;

void delay()
{
    QTime dieTime= QTime::currentTime().addSecs(2);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


int main(int argc,char *argv[]) {
    QCoreApplication a(argc,argv);

    UA_Client *client = UA_Client_new(UA_ClientConfig_standard);
    FestoMotionLibrary festoMotion;

    festoMotion.connect(client);
    festoMotion.start();
    //festoMotion.setEnable(true);



    ofstream logFile("log.txt");
    streambuf *coutbuf = cin.rdbuf();
    cout.rdbuf(logFile.rdbuf());

    cout<<"Start App" <<endl;

     double actCoordinates[] = {10, 10, 10};
     double setCoordinates1[] = {100,100,0};
     double setCoordinates2[] = {200,200,0};
     double setCoordinates3[] = {280,280,0};
     double setCoordinates4[] = {280,50,0};
     double setCoordinates5[] = {50,280,0};

     double defaultVelo = 100;

      while(!festoMotion.isEnabled()){
          cout<<"NOT ENABLED YET" <<endl;
          festoMotion.setEnable(true);
      }

      if(festoMotion.isEnabled()){
          cout<<"SYSTEM ENABLED..." <<endl;
          festoMotion.setMOP(false);

          if(festoMotion.isHomed()){
              cout<<"Start ptp" <<endl;
              festoMotion.changeMOP(50);  // PTP mode
              festoMotion.setMOP(true);

          }

          festoMotion.getActualCoordinates(actCoordinates);
          cout<<actCoordinates[0]<<" " <<actCoordinates[1]<<" " << actCoordinates[2]<<endl;

          festoMotion.setPtpCoordinates(setCoordinates2,true,defaultVelo);

          festoMotion.setPtpCoordinates(setCoordinates3,true,defaultVelo);
//          delay();
        festoMotion.setPtpCoordinates(setCoordinates4,true,defaultVelo);
//            delay();
          festoMotion.setPtpCoordinates(setCoordinates5,true,defaultVelo);
          festoMotion.setPtpCoordinates(setCoordinates1,true,defaultVelo);
//            delay();

      }





    return a.exec();


}
















/*
 *
 *
 *
char  eMOPin[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.eMOP";
char  xSetOpMode[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stCW.xSetOpMode";
char  start[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stCW.xStart";
char abort[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stCW.xAbort";
char symReset[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stCW.xReset";
char  enable[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stCW.xEnable";
char  xMoveNegative[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stCW.xMoveNegative";
char  xMovePositive[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stCW.xMovePositive";

char ptpX[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stPosPtp.lrX";
char ptpY[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stPosPtp.lrY";
char ptpZ[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stIn.stPosPtp.lrZ";

// g_stOut
char symIsHomed[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stOut.stSW.xHomed";
char symIsMotionComplete[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stOut.stSW.xMotionComplete";
char symIsMoving[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stOut.stSW.xIsMoving";
char enabled[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stOut.stSW.xEnabled";
char busy[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stOut.stSW.xBusy";
char symIsError[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stOut.xError";

char symActualposX[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stOut.stActualPositionMCS.lrX";
char acpY[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stOut.stActualPositionMCS.lrY";
char acpZ[] = "|var|CECC-X-M1.Application.GVL_Motion_INOUT.g_stOut.stActualPositionMCS.lrY";

void printHexStatusCode(int statusCode){
    std::cout << std::setfill('0') << std::setw(8) << std::hex << statusCode << '\n';
}

void readValue(UA_Client *client, char *symbol, int dataType){
    UA_Variant value;
    UA_Variant_init(&value);
    const UA_DataType *type = &UA_TYPES[dataType];
    const UA_NodeId nodeID = UA_NODEID_STRING(2,symbol);
    UA_StatusCode retvalReadEnabled = UA_Client_readValueAttribute(client, nodeID, &value);
    if(retvalReadEnabled == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value,type)) {
       cout<<"reading symbol;" <<symbol << retvalReadEnabled<<endl;

        cout<<  value.data<<endl;
    }
    else
        cout<<"Cannot access Enabled variable" <<endl;
    UA_Variant_deleteMembers(&value);

}

*/

/* Read the value attribute of the node. UA_Client_readValueAttribute is a
 * wrapper for the raw read service available as UA_Client_Service_read. */
/* Variants can hold scalar values and arrays of any type */

/*
UA_Variant value;
UA_Variant_init(&value);


const UA_NodeId nodeId =
    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME);

retval = UA_Client_readValueAttribute(client, nodeId, &value);
if(retval == UA_STATUSCODE_GOOD &&
   UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_DATETIME])) {
    UA_DateTime raw_date = *(UA_DateTime*)value.data;
    UA_String string_date = UA_DateTime_toString(raw_date);
    printf("string date is: %.*s\n", (int)string_date.length, string_date.data);
    UA_String_deleteMembers(&string_date);
}



UA_Variant_deleteMembers(&value);
UA_Client_delete(client); /* Disconnects the client internally */



  /*


    //////////////////////////////////////////

    // Reading enabled Festo variable

    UA_Variant value2;
    UA_Variant_init(&value2);

    const UA_NodeId nodeID2 = UA_NODEID_STRING(2,enabled);
    UA_StatusCode retvalReadEnabled = UA_Client_readValueAttribute(client, nodeID2, &value2);
    if(retvalReadEnabled == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value2,&UA_TYPES[UA_TYPES_BOOLEAN])) {
        cout<<"Reading Enabled state" <<"retval Read enable" <<retvalReadEnabled<<endl;
        cout<<*(bool*)value2.data<<endl;
    }
    else
        cout<<"Cannot access Enabled variable" <<endl;


    /////////////////////////////////////

    // Reading enabled Festo variable using method
   // const UA_DataType *Datatype = &UA_TYPES[UA_TYPES_BOOLEAN];

    readValue(client, enabled,0);




    cout<<"Setting enabled to true" <<endl;
    // Setting enabled Festo variable

    const UA_NodeId nodeID2New = UA_NODEID_STRING(2,enable);

    UA_Boolean newState = true;
    UA_Variant_setScalar(&value2, &newState, &UA_TYPES[UA_TYPES_BOOLEAN]);
    UA_StatusCode retvalSetEnable = UA_Client_writeValueAttribute(client,nodeID2New,&value2);
     //cout<<"Retval set" <<printHexStatusCode(retvalSetEnable);


    /////////////////////////////////////////

    UA_StatusCode retvalReadEnabledNew = UA_Client_readValueAttribute(client, nodeID2, &value2);
    if(retvalReadEnabledNew == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value2,&UA_TYPES[UA_TYPES_BOOLEAN])) {
        cout<<"Reading Enabled state" <<endl;
        cout<<*(bool*)value2.data<<endl;
    }
    else
        cout<<"Cannot access Enabled variable" <<endl;

    ///////////////////////////////////

     // Reading Festo Motion start variable
    UA_Variant value3;
    UA_Variant_init(&value3);

    const UA_NodeId nodeID3 = UA_NODEID_STRING(2,start);
    UA_StatusCode retvalReadStart = UA_Client_readValueAttribute(client, nodeID3, &value3);
    if(retvalReadStart == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value3,&UA_TYPES[UA_TYPES_BOOLEAN])) {
        cout<<"Reading start state" <<*(UA_Boolean*)value3.data<<endl;
    }
    else
        cout<<"Cannot access start variable" <<endl;

    // Reading Festo Motion posX variable
   UA_Variant value4;
   UA_Variant_init(&value4);

   const UA_NodeId nodeID4 = UA_NODEID_STRING(2,acpX);
   UA_StatusCode retvalReadPosX = UA_Client_readValueAttribute(client, nodeID4, &value4);
   if(retvalReadPosX  == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value4,&UA_TYPES[UA_TYPES_DOUBLE])) {
       cout<<"Reading posX state" <<endl;
       cout<<*(UA_Double*)value4.data<<endl;
   }
   else
       cout<<"Cannot access posX variable" <<endl;

       */


