#include "imageviewerwidget.h"
#include "ui_imageviewerwidget.h"

ImageViewerWidget::ImageViewerWidget(QWidget *parent) :
    QGraphicsView(parent),
    ui(new Ui::ImageViewerWidget)
{
    ui->setupUi(this);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    m_scene = new QGraphicsScene(this);
    m_pixmapItem = new QGraphicsPixmapItem();
    m_scene->addItem(m_pixmapItem);

    setScene(m_scene);

    m_fpsTimer = new QTimer(this);

    m_iframeCounter = 0;
    m_iFPS = 0;
    QObject::connect(m_fpsTimer,SIGNAL(timeout()),this,SLOT(updateFPS()));
    m_fpsTimer->start(1000);

    QRectF visibleArea = mapToScene(rect()).boundingRect();
     QRectF sceneBounds = sceneRect();

    SetCenter(QPointF(this->width()/2, this->height()/2)); //A modified version of centerOn(), handles special cases
    setCursor(Qt::OpenHandCursor);

    m_bZoomable = true;

}

ImageViewerWidget::~ImageViewerWidget()
{
    delete m_pixmapItem;
    delete m_scene;

    delete m_fpsTimer;
    delete ui;
}


#include <iostream>

using namespace std;
bool ImageViewerWidget::UpdateImage(unsigned char* pszImage, unsigned uWidth, unsigned uHeight, bool bRGB)
{

   m_pImage = pszImage;

   if (false == bRGB)
        m_Image = new QImage(m_pImage, uWidth, uHeight,uWidth,QImage::Format_Indexed8 );
   else
   {       
       m_Image = new QImage(m_pImage, uWidth, uHeight,uWidth*3,QImage::Format_RGB888);
   }

    m_pixmapItem->setPixmap( QPixmap::fromImage(*m_Image));//.scaled(QSize(width()-20, height()-20)));
    m_iframeCounter++;
    delete m_Image;

    return true;
}


bool ImageViewerWidget::UpdateImage(unsigned char* pszImage, unsigned uWidth, unsigned uHeight, bool bRGB, unsigned uSubImageWidth, unsigned uSubImageHeight, bool bHorizontalMirror, bool bVerticalMirror)
{

   m_pImage = pszImage;

   if (false == bRGB)
   {
        m_Image = new QImage(m_pImage, uWidth, uHeight,uWidth,QImage::Format_Indexed8 );
   }
   else
   {

       m_Image = new QImage(m_pImage, uWidth, uHeight,uWidth*3,QImage::Format_RGB888);

//       for ( int i=0; i<255; i++ ) // build color table
//           m_Image->setColor( i, qRgb(i,i,i) );
   }


   m_pixmapItem->setPixmap( QPixmap::fromImage(m_Image->copy(
                                                   (uWidth-uSubImageWidth)/2,
                                                   (uHeight-uSubImageHeight)/2,
                                                   uSubImageWidth,
                                                   uSubImageHeight).mirrored(bHorizontalMirror,bVerticalMirror)));


    m_iframeCounter++;
    delete m_Image;


    return true;
}

bool ImageViewerWidget::UpdateImage(QImage &Image)
{
    m_pixmapItem->setPixmap( QPixmap::fromImage(Image));//.scaled(QSize(width()-20, height()-20)));
    m_iframeCounter++;
    return true;
}


unsigned ImageViewerWidget::GetFPS()
{
    return m_iFPS;
}



bool ImageViewerWidget::MoveImage(short dx, short dy)
{
    // naihin pitaisi zoomauskerroin upottaa mukaan jotenkin
    short deltax = 10*dx;
    short deltay = 10*dy;
    QPointF delta = QPointF(deltax,deltay);
    SetCenter(GetCenter()+delta);
    return true;
}


void ImageViewerWidget::updateFPS()
{	
        m_iFPS = m_iframeCounter;
        m_iframeCounter = 0;
        emit FPSupdated();
}


void ImageViewerWidget::keyPressEvent(QKeyEvent *event)
{
    emit keyPressed(event);
}

/**
  * Handles when the mouse button is pressed
  */

void ImageViewerWidget::mousePressEvent(QMouseEvent* event) {
    //For panning the view
    if(event->button() == Qt::LeftButton && m_bZoomable)
    {
        m_qpLastPanPoint = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }

    if(event->button() == Qt::RightButton)
    {
        emit RightButtonPressed();
    }

    emit clicked();
}


void ImageViewerWidget::mouseDoubleClickEvent ( QMouseEvent * event )
{
    if (m_bZoomable)
    {
        SetCenter(QPointF(this->width()/2, this->height()/2));
        resetMatrix();
    }
    emit doubleClicked();

}



/**
  * Handles when the mouse button is released
  */
void ImageViewerWidget::mouseReleaseEvent(QMouseEvent* event) {
    setCursor(Qt::OpenHandCursor);
    m_qpLastPanPoint = QPoint();
}

/**
*Handles the mouse move event
*/
void ImageViewerWidget::mouseMoveEvent(QMouseEvent* event) {
    if(!m_qpLastPanPoint.isNull() ) {
        //Get how much we panned
        QPointF delta = mapToScene(m_qpLastPanPoint) - mapToScene(event->pos());
        m_qpLastPanPoint = event->pos();

        //Update the center ie. do the pan
        SetCenter(GetCenter() + delta);
    }
}

/**
  * Zoom the view in and out.
  */
#include <iostream>
void ImageViewerWidget::wheelEvent(QWheelEvent* event) {

    if (m_bZoomable)
    {
        //Get the position of the mouse before scaling, in scene coords
        QPointF pointBeforeScale(mapToScene(event->pos()));

        //Get the original screen centerpoint
        QPointF screenCenter = GetCenter(); //CurrentCenterPoint; //(visRect.center());

        //Scale the view ie. do the zoom
        double scaleFactor = 1.15; //How fast we zoom
        if(event->delta() > 0) {
            //Zoom in
            scale(scaleFactor, scaleFactor);
        } else {
            //Zooming out
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }

        //Get the position after scaling, in scene coords
        QPointF pointAfterScale(mapToScene(event->pos()));

        //Get the offset of how the screen moved
        QPointF offset = pointBeforeScale - pointAfterScale;

        //Adjust to the new center for correct zooming
        QPointF newCenter = screenCenter + offset;
        SetCenter(newCenter);
        //std::cout << "zoom " << newCenter.x() << "," << newCenter.y() << " - " << scaleFactor << std::endl;
    }
}

void ImageViewerWidget::Zoom(QPointF qCenterPoint, double dScaling)
{
    if (qCenterPoint.x() <= m_pixmapItem->pixmap().width() && qCenterPoint.y() <= m_pixmapItem->pixmap().height())
    {
        scale(dScaling,dScaling);
        SetCenter(qCenterPoint);
    }
}

QImage ImageViewerWidget::getCurrentImage()
{
   return m_pixmapItem->pixmap().toImage();
}

QPixmap ImageViewerWidget::getCurrentPixmap()
{
    return m_pixmapItem->pixmap();
}





/**
  * Need to update the center so there is no jolt in the
  * interaction after resizing the widget.
  */
void ImageViewerWidget::resizeEvent(QResizeEvent* event) {
    //Get the rectangle of the visible area in scene coords
    QRectF visibleArea = mapToScene(rect()).boundingRect();
    SetCenter(visibleArea.center());

    if (scene()->sceneRect().width() < visibleArea.width() || scene()->sceneRect().height() < visibleArea.height())
        fitInView(this->scene()->sceneRect(), Qt::KeepAspectRatio);

    //Call the subclass resize so the scrollbars are updated correctly
    QGraphicsView::resizeEvent(event);
}

void ImageViewerWidget::SetCenter(const QPointF& centerPoint) {
    //Get the rectangle of the visible area in scene coords
    QRectF visibleArea = mapToScene(rect()).boundingRect();

    //Get the scene area
    QRectF sceneBounds = sceneRect();

    double boundX = visibleArea.width() / 2.0;
    double boundY = visibleArea.height() / 2.0;
    double boundWidth = sceneBounds.width() - 2.0 * boundX;
    double boundHeight = sceneBounds.height() - 2.0 * boundY;

    //The max boundary that the centerPoint can be to
    QRectF bounds(boundX, boundY, boundWidth, boundHeight);

    if(bounds.contains(centerPoint)) {
        //We are within the bounds
        m_CurrentCenterPoint = centerPoint;
    } else {
        //We need to clamp or use the center of the screen
        if(visibleArea.contains(sceneBounds)) {
            //Use the center of scene ie. we can see the whole scene
            m_CurrentCenterPoint = sceneBounds.center();
        } else {

            m_CurrentCenterPoint = centerPoint;

            //We need to clamp the center. The centerPoint is too large
            if(centerPoint.x() > bounds.x() + bounds.width()) {
                m_CurrentCenterPoint.setX(bounds.x() + bounds.width());
            } else if(centerPoint.x() < bounds.x()) {
                m_CurrentCenterPoint.setX(bounds.x());
            }

            if(centerPoint.y() > bounds.y() + bounds.height()) {
                m_CurrentCenterPoint.setY(bounds.y() + bounds.height());
            } else if(centerPoint.y() < bounds.y()) {
                m_CurrentCenterPoint.setY(bounds.y());
            }

        }
    }

    //Update the scrollbars
    centerOn(m_CurrentCenterPoint);
}


