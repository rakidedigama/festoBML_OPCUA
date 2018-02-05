#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QFileDialog>
#include <QProcess>
#include <QThread>


#include "serialrobot.h"
#include "IniFile.h"
#include "json.h"
#include "qmqtopicinterface.h"
#include "festomotionlibrary.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    //SerialRobot *robot;
    FestoMotionLibrary *fRobot;
    IniFile ini;
    QMQTopicInterface* m_topic;
    QString currentRecipe;

    QThread* m_thread;
    Json::Value points;
    int nnode;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void runRecipeSignal(QString recipe);
    void checkSignal();
    void jogSignal(int jogVal);



private:
    Ui::MainWindow *ui;
private slots:
    void readJsonFile(QString recipe);
    void down();
    void releaseDown();
    void up();
    void releaseUp();
    void left();
    void releaseLeft();
    void right();
    void releaseRight();

    void error(QString);

    void runRecipe();
    void disableAll(bool);

    void pos(double,double);
    void vertSlider(int);
    void horSlider(int);
    void vertSliderMoved(int);
    void horSliderMoved(int);
    void shootImage(double x, double y);
    void msg(QString);

    void recipeSelection();
    void stopPressed();

    void moveRobotTo(QPointF);

    void stepSizeChanged(double);

    void ptpVelocityChanged(int);

    void rbChanged();

    void moving(bool);

    void recipeDone();

    void calcToggled(bool);

    void joggingMode();





};

#endif // MAINWINDOW_H
