#ifndef SERIALROBOT_H
#define SERIALROBOT_H

#include <QThread>
#include <QMutex>
#include <QFile>
#include <QVector>

#include <fstream>



#include "qasyncserial.h"

class SerialRobot : public QThread
{
    Q_OBJECT
protected:
    bool    m_bStopRequested;
    virtual void run();

    double currX;
    double currY;
    double currZ;



    typedef struct
    {
        double x;
        double y;
        unsigned z; // firgelli
    } RecipePoint;

    QVector<RecipePoint> mapRecipe;
    QMutex *mutex;

    QAsyncSerial* m_serial;
    bool bMoveSent;

public:
    explicit SerialRobot(QObject *parent = 0);
    ~SerialRobot();
    bool initialize(QString comPort);
    bool goTo(double x, double y, double z);
    bool setSpeed(double speed);

    double getX()
    {
        return currX;
    }
    double getY()
    {
        return currY;
    }
    double getZ()
    {
        return currZ;
    }

    bool runRecipe(QString recipe);

    bool hasMoveEnded()
    {
        return !bMoveSent;
    }

    void stop();

signals:
    void error(QString);
    void threadRunning(bool);
    void pos(double,double);
    void shoot(double,double);
    void msg(QString);
    void moving(bool);
    void recipeFinished();
private slots:
    void lineReceived(QString);
};

#endif // SERIALROBOT_H
