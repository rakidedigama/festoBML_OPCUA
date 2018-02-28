#ifndef IMAGEVIEWERWIDGET_H
#define IMAGEVIEWERWIDGET_H



#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QMouseEvent>



namespace Ui {
class ImageViewerWidget;
}

class ImageViewerWidget : public QGraphicsView
{
    Q_OBJECT
    
public:
    explicit ImageViewerWidget(QWidget *parent = 0);
    ~ImageViewerWidget();

    unsigned   m_iFPS;
    bool UpdateImage(unsigned char*, unsigned, unsigned, bool);
    bool UpdateImage(unsigned char*, unsigned, unsigned, bool, unsigned, unsigned, bool bHorizontalMirror, bool bVerticalMirror);
    bool UpdateImage(QImage &);
    //bool UpdateImage(unsigned short*, unsigned, unsigned);
    bool MoveImage(short, short);

    unsigned char* m_pImage;
    unsigned m_uWidth;
    unsigned m_uHeight;
    unsigned GetFPS();

    inline void SetZoomable(bool b ){m_bZoomable = b;};

    void Zoom(QPointF, double dScaling);

    virtual QImage getCurrentImage();
    QPixmap getCurrentPixmap();





protected:
    Ui::ImageViewerWidget *ui;


    QGraphicsPixmapItem*    m_pixmapItem;
    QGraphicsScene*         m_scene;

    QImage*                 m_Image;
    QTimer*                 m_fpsTimer;

    unsigned m_iframeCounter;

	QPoint m_qpLastPanPoint;
    QPointF m_CurrentCenterPoint;
    QPointF m_nominalZoom;

    bool    m_bZoomable;

protected slots:
    void updateFPS();

protected:
    //Set the current centerpoint in the
       void SetCenter(const QPointF& centerPoint);
       QPointF GetCenter() { return m_CurrentCenterPoint; }

       //Take over the interaction
       virtual void mousePressEvent(QMouseEvent* event);
       virtual void mouseReleaseEvent(QMouseEvent* event);
       virtual void mouseMoveEvent(QMouseEvent* event);
       virtual void mouseDoubleClickEvent ( QMouseEvent * event );       
       virtual void wheelEvent(QWheelEvent* event);
       virtual void resizeEvent(QResizeEvent* event);
       virtual void keyPressEvent(QKeyEvent *event);       



signals:
    void FPSupdated();
    void RightButtonPressed();

    void doubleClicked();
    void clicked();
    void keyPressed(QKeyEvent*);


};

#endif // IMAGEVIEWERWIDGET_H
