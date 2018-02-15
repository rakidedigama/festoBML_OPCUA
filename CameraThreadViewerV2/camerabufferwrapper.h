#ifndef CAMERABUFFERWRAPPER_H
#define CAMERABUFFERWRAPPER_H

#include <QThread>
#include <QDir>

#include <QProcess>



#include <math.h>
#include <numeric>

#include "PWImage.H"
#include "BayerFilter.H"
#include "IniFile.h"







class CameraBufferWrapper : public QThread
{
Q_OBJECT
private:


public:
    CameraBufferWrapper();
    ~CameraBufferWrapper();

    typedef enum IMAGE_SELECTION
    {
        RAW = 0,
        RGB = 1,
        R = 2,
        G = 3,
        B = 4
    };

   bool getImage(unsigned char*, unsigned,IMAGE_SELECTION);
   BayerFilter::IMAGE_PROPERTIES getImageProperties();
   unsigned getImageSize();
   bool initializeCamera();

   bool stopThread();
   void setIS(IMAGE_SELECTION is);
   IMAGE_SELECTION getIS();
    bool    m_bStopRequested;

    void setPause(bool);
    void setSaving(bool);
    bool isSaving();
    void setSaveFolder(QString);
    void saveOne();
    void saveOne(double x, double y, QString savetag="", QString saveClass ="");

    void calcFolderInThread(QString savetag, QString saveClass);
        void testFunc();

protected:
    virtual void run();



    //LVTImageUS *m_lvtImageRGB;
    //LVTImageUS *m_lvtImage;
    //LVTImageUC *m_lvtImageuc;
    IMAGE_SELECTION m_is;
    unsigned m_uImageId;
    BayerFilter::IMAGE_PROPERTIES m_Props;
    bool m_bCameraInited;

    IniFile*    m_Ini;

    bool m_bPaused;
    bool m_bSaving;

    bool m_bSaveOne;
    double m_dSaveX;
    double m_dSaveY;
    QString m_qsSaveTag;
    QString m_qsSaveType;

    QString qSaveFolder;
	
    int m_iFirgelliVal;

    bool    bTrigger;
    bool    bTriggerLast;


public slots:
    void firgelli(int);
    void trigger(bool);

signals:
    void newImageAvailable(std::string);
    void savedImg();
    void saveSnap(QString);
    void newRawImg(std::string);
    void imageSize(unsigned, unsigned,QString src);
    void darkImg();
    void error(QString);

};
#endif // CAMERABUFFERWRAPPER_H
