#ifndef FESTOMOTIONLIBRARY_H
#define FESTOMOTIONLIBRARY_H
#include "open62541.h"

#include <QObject>
#include <QThread>

class FestoMotionLibrary : public QThread
{
    Q_OBJECT

public:

    FestoMotionLibrary(QObject* parent = 0);


    //~FestoMotionLibrary();

    void connect(UA_Client *someClient);
    void setEnable( bool bEnableValue);
    bool isEnabled();
    void startMotion(bool bStart);
    bool readStartMotion();
    void abort(bool bAbort);
    void reset(bool bReset);
    bool isHomed();
    double getActualPosX();
    double getActualPosY();
    double getActualPosZ();
    void getActualCoordinates(double coordinates[]);
    void changeMOP(int mode);
    void setMOP(bool bSetMOP);
    void setPtpPosX(double x);
    void setPtpPosY(double y);
    void setPtpPosZ(double z);
    void setPtpCoordinates(double coordinates[],double bStart,double velo);
    bool isMotionComplete();
    bool isMoving();
    bool isError();

    void startPTPMotion();
    void stopPTPMotion();
    void setPTPVelo(double velo);

    bool isBusy();


    UA_Client *client;



    bool bIsEnabled;
    bool bIsMoving;
    bool bMotionComplete;
    bool bStart;

    double posX;
    double posY;
    double posZ;

    double ptpVelo;

signals:
    void sigUpdatePosition();
//    void currentPosition(double,double,double);
private slots:
  void updatePosition();
protected:
    virtual void run();

};

#endif // FESTOMOTIONLIBRARY_H
