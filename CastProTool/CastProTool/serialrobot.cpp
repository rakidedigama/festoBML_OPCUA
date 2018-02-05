#include "serialrobot.h"

#include <iostream>

#include "StringBuilder.h"
#include "json.h"



SerialRobot::SerialRobot(QObject *parent) : QThread(parent)
{

    mutex = new QMutex();
    m_bStopRequested = false;

    bMoveSent = false;

    m_serial = new QAsyncSerial();
    QObject::connect(m_serial,SIGNAL(lineReceived(QString)),this,SLOT(lineReceived(QString)));


}

SerialRobot::~SerialRobot()
{
    m_bStopRequested = true;
    wait();

}

bool SerialRobot::initialize(QString comPort)
{
    m_serial->open(comPort,9600);
    if (m_serial->isOpen())
    {
        QString data = "Move\r\n20\r\n20\r\n20\r\n";
        m_serial->write(data);


        return true;
    }
    return false;
}


bool SerialRobot::goTo(double x, double y, double z)
{
    if (x > 350 || y > 350 || z > 350)
    {
        emit msg("ERROR given coordinates beyond limits (>350)");
        return false;

    }
    using namespace std;
    if (bMoveSent)
        return false;
    if (mutex->tryLock(10) )
    {
        if (x < 0)
            x == 0;
        if (y < 0)
            y == 0;
        cout << "Try to move to " << x << " , " << y << endl;
        if (m_serial->isOpen() && x <= 350 && y <= 350 && x >= 0 && y >= 0)
        {
            cout << "Move to " << x <<", " << y << ", " << z << endl;
            std::string data = StringBuilder() << "Move\r\n"
                                               << x <<"\r\n"
                                                << y <<"\r\n"
                                                << z <<"\r\n";
            //std::string data = "Move\r\n30.5\r\n30.5\r\n150\r\n";
            emit moving(true);
            m_serial->write(QString::fromStdString(data));
            bMoveSent = true;


            mutex->unlock();
            return true;
        }
    }
    return false;
}

bool SerialRobot::setSpeed(double speed)
{
    return false;
}

bool SerialRobot::runRecipe(QString recipe)
{
    using namespace std;

    m_bStopRequested = false;
    if (mutex->tryLock(10))
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
            mutex->unlock();
            return false;
        }

        const Json::Value array = root["points"];

        mapRecipe.clear();
        // Iterate over sequence elements and
        // print its values
        for(unsigned int index=0; index<array.size(); ++index)
        {

            RecipePoint p;
            p.x = (double)array[index]["x"].asDouble();
            p.y = (double)array[index]["y"].asDouble();
            p.z = (double)array[index]["z"].asInt();
            if (p.x < 0 || p.x > 350)
            {
                emit error("X coords in recipe " + recipe + " out of bounds");
                mutex->unlock();
                return false;
            }

            if (p.y < 0 || p.y > 350)
            {
                emit error("Y coords in recipe " + recipe + " out of bounds");
                mutex->unlock();
                return false;
            }

            if (p.z < 0 || p.z > 32767)
            {
                emit error("Z coords in recipe " + recipe + " out of bounds");
                mutex->unlock();
                return false;
            }

            mapRecipe.push_back(p);
            cout << "Read " << mapRecipe.size() << " points from recipe" << endl;
            start();
        }
        mutex->unlock();
    }
    return true;
}

void SerialRobot::stop()
{
    m_bStopRequested = true;
}

void SerialRobot::lineReceived(QString line)
{

    emit moving(false);
    bMoveSent = false;
    using namespace std;
    cout << "Receive: " << line.toStdString() << endl;
    // MOK_19.998_19.998
    QVector<unsigned> v;

    for (int i =0 ; i<  line.length(); i++)
    {
        if (line.at(i)== '_')
            v.push_back(i);
    }
    if (v.size() == 2)
    {
        double x = line.mid(4,v.at(1)-4).toDouble();
        double y = line.mid(v.at(1)+1,line.length()-v.at(1)-1).toDouble();
        emit pos(x,y);
        currX = x;
        currY = y;
        currZ = 100;

    }
    else
        emit error("Could not parse coords from " + line);
}


void SerialRobot::run()
{

    emit threadRunning(true);

    for (int i = 0; i < mapRecipe.size();i++)
    {
        RecipePoint p = mapRecipe.at(i);
        goTo(p.x,p.y,p.z);

        while (!hasMoveEnded() && !m_bStopRequested)
        {
            Sleep(100);
        }

        emit msg("Point " + QString::number(i+1) + "/" + QString::number(mapRecipe.size()));
        emit shoot(currX,currY);
        if (m_bStopRequested)
            break;
    }
    emit recipeFinished();
    emit threadRunning(false);


}


