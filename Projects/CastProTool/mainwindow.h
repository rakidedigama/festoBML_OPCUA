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
#include "messagehandler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    //SerialRobot *robot;
    FestoMotionLibrary *fRobot;
    MessageHandler *mHandler;
    IniFile ini;
//    QMQTopicInterface* m_topic;
    QString currentRecipe;

    QThread* m_thread;
    QThread* msg_thread;
    Json::Value points;
    int nnode;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void runRecipeSignal(QString recipe);
    void checkSignal();
    void jogSignal(int jogVal);
    void systemEnableBtnPressed();
    void moveCamToFocus(double zPos);
    void sampleReferenceTag(QString refTag);



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
    void zUp();
    void zUpRelease();
    void zDown();
    void zDownRelease();

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

    void newMessage(QString, QByteArray); // from msg queue


    void on_pushButton_clicked();
public slots:

    void ZMotionConfirmMsg();

};

#endif // MAINWINDOW_H
