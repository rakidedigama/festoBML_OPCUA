#ifndef FESTOMOTIONLIBRARY_H
#define FESTOMOTIONLIBRARY_H
#include "open62541.h"

#include <QObject>
#include <QThread>
#include "json.h"
#include "qmqtopicinterface.h"
class FestoMotionLibrary : public QObject
{
    Q_OBJECT

protected:
    virtual void run();

public:

    FestoMotionLibrary(QObject* parent = 0);
    QMQTopicInterface* m_topic;


    //~FestoMotionLibrary();

    // Basic Functions
    void connectPLC(UA_Client *someClient);
    void setEnable( bool bEnableValue);    
    void startMotion(bool bStart);
    bool readStartMotion();
    void abort(bool bAbort);
    void reset(bool bReset);
    double getActualPosX();
    double getActualPosY();
    double getActualPosZ();
    bool isEnabled();
    bool isHomed();
    bool isMotionComplete();
    bool isMoving();
    bool isError();
    bool isBusy();
    void setMOP(bool bSetMOP);
    void changeMOP(int mode);
    int viewMOP();
    void changeAxis(int axis);
    void movePositive(bool val);
    void moveNegative(bool val);
    void setPtpPosX(double x);
    void setPtpPosY(double y);
    void setPtpPosZ(double z);

     // Derived functions
    void initialize();
    void getActualCoordinates(double coordinates[]);
    void homeSystem();
    void checkSlot();

    //void changeModeToJogging();

    void jog(int axis,bool direction, bool toggle);

    void setPtpCoordinates(double coordinates[],double bStart,double velo);


    void startPTPMotion();
    void stopPTPMotion();
    void setPTPVelo(double velo);
    void setJogVelo(double velo);
    double getPTPVelo();

    void runPoints();

    // Other functions

    bool readJsonFile(QString recipe);
    void parseJsonPointsFromFile(Json::Value *pPoints);



    UA_Client *client;

    // Initializations
    bool bStopRequested;

    bool bIsEnabled;
    bool bIsMoving;
    bool bMotionComplete;
    bool bStart;
    bool bHomed;
    bool ptpPointsFromFileReady ;

    int mode;

    double posX;
    double posY;
    double posZ;

    double ptpVelo;
    double jogVelo;
    double focusVelo;
    double recipeVelo;
    QString recipeRefTag;
    Json::Value points;


signals:
     void finished();
     void error(QString);
     void threadRunning(bool);
     void pos(double,double);
     void shoot(double,double);
     void msg(QString);
     void moving(bool);
     void recipeFinished();
    void sigUpdatePosition();
      void ptpChanged(double);
      void ZMotionComplete();

private slots:
  void updatePosition();
  void sendZConfirmMsg();  // AMQP Message
  void sendShootMsg();

public slots:
  // called from outside, mostly from cast pro Tool
    void processRecipe(QString recipe);
    //void getJogHandle(int jogVal);
    void handleJog(int jogV);
    void triggerSystemEnable();
    void moveZtoFocus(double zp);
    void getSampleRefTag(QString refTag);

};

#endif // FESTOMOTIONLIBRARY_H
