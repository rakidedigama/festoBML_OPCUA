#ifndef CAMERATHREADVIEWERGUI_H
#define CAMERATHREADVIEWERGUI_H

#include <QMainWindow>
#include <QThread>
#include <QMetaType>
#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QLineEdit>
#include <QTimer>
#include <QFuture>
#include <QtCore>
#include <QFuture>
#include <QtConcurrentRun>
#include <iostream>

#include "PWImage.H"
#include "CameraBufferWrapper.h"
#include "Focuser.h"
#include "lightcalibrator.h"

#include "QMQTopicInterface.h"
#include "filedownloader.h"


class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    ClickableLabel(QWidget* parent = NULL)
        : QLabel(parent)
    {

    }
protected:

    void mousePressEvent ( QMouseEvent * event )
    {
        if (event->type() == QEvent::MouseButtonDblClick)
        {
            emit doubleClicked();
        }
    }


signals:
    void doubleClicked();
};


namespace Ui {
class CameraThreadViewerGUI;
}



class CameraThreadViewerGUI : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit CameraThreadViewerGUI(QWidget *parent = 0);
    ~CameraThreadViewerGUI();



private:
    Ui::CameraThreadViewerGUI *ui;

    CameraBufferWrapper *m_CameraBuffer;
    Focuser *m_Focuser;

    LightCalibrator* m_lights;    
    QThread* focusThread;
    //TwincatManager* m_TwincatManager;

    unsigned        m_uCounter;
    bool            m_bInitializedOnce;

    QString         m_qsSaveFolder;

    unsigned    m_uSaveCounter;

    bool        m_bUsingBeckhoff9050;

    unsigned    m_uFocusingStep;

    unsigned    m_uDarkImgCounter;

    bool    m_bCalibratingLights;
    bool    m_bFocusing;



    bool m_bMetricFirgelli;
    QString m_qsFirgelliVariable;

    QTimer* m_timerFocus;

    //QMQTopicInterface* m_topic;

    std::string m_sGardaIP;

    FileDownloader* m_GardaPage;
    QTimer* m_gardaTimer;

protected:
    virtual void resizeEvent(QResizeEvent *);
public slots:
    void updateImageViewer(std::string);
    void startButtonPressed();
    void stopButtonPressed();
    void pauseButtonPressed();
    void rawRadioChecked(bool);
    void rGBRadioChecked(bool);
    void rRadioChecked(bool);
    void gRadioChecked(bool);
    void bRadioChecked(bool);
    void FPSUpdate();
    void selectSaveFolder();
    void startSave();
    void endSave();
    void blinkSaving();

    void saveCounter();
    void snapMsg(QString);

    void snapPressed();

private slots:
    void saveFolderDoubleClick();
    void saveFolderTimeout();
    void saveFolderLineEditChanged(QString);

    void focusTestPressed();

//    void moveFirgelli(uint);
    void focusingOn(bool);

//    void firgelliValueChanged(int);

    void bestFocus(uint);

    void imageSize(unsigned, unsigned, QString);

    void darkImg();

    void error(QString);

    void lightsCalibPressed();
    void lightsCalibOn(bool);

    void pbLightsTestPressed();

    void doTimedFocus();

    void newMessage(QString, QByteArray);
    void newTwincatValue(QString,qint16);

    void gardaPageRequest();
    void gardaPageDownload();

    void zAxisMove();
    



signals:
    void doLightsCalib( unsigned);
    void cancelLightsCalib();
};

#endif // CAMERATHREADVIEWERGUI_H
