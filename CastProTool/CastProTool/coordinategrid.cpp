#include "coordinategrid.h"

#include <iostream>

CoordinateGrid::CoordinateGrid(QWidget* parent)
    :QCustomPlot(parent)
{
    this->addGraph();
    this->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    this->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue


    graph(0)->setLineStyle(QCPGraph::lsNone);
    graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));

    this->xAxis->setRange(QCPRange(0,360));
    this->yAxis->setRange(QCPRange(0,360));
   // axisRect()->setBackground(QPixmap("D:\\GITROOT\\SE\\Tools\\CastProTool\\laban.jpg"));
    // make left and bottom axes always transfer their ranges to right and top axes:
    //connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    //connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
    // pass data points to graphs:

    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    this->graph(0)->rescaleAxes();
    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):

    // Note: we could have also just called customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:

    this->addGraph();
    this->graph(1)->setPen(QPen(Qt::red)); // line color blue for first graph
    this->graph(1)->setBrush(QBrush(QColor(255, 0, 00, 40))); // first graph will be filled with translucent blue
    graph(1)->setLineStyle(QCPGraph::lsNone);
    graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond, 4));


    QObject::connect(this,SIGNAL(mousePress(QMouseEvent*)),this,SLOT(mousePressed(QMouseEvent*)));
    setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
}

void CoordinateGrid::mouseMoveEvent(QMouseEvent *event)
{

}

void CoordinateGrid::clearGrid()
{
    m_x.clear();
    m_y.clear();
    replot();
}

void CoordinateGrid::addPoint(QPointF q)
{

    m_x.push_back(q.x());
    m_y.push_back(q.y());
    this->graph(0)->setData(m_x,m_y);
    this->replot();
}

void CoordinateGrid::setCurrentPoint(QPointF p)
{
    m_currX.clear();
    m_currY.clear();
    m_currX.push_back(p.x());
    m_currY.push_back(p.y());
    this->graph(1)->setData(m_currX,m_currY);
    this->replot();
}

void CoordinateGrid::mousePressed(QMouseEvent *e)
{


}

void CoordinateGrid::contextMenuRequest(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);


    m_mousePos.setX(xAxis->pixelToCoord(pos.x()));
    m_mousePos.setY(yAxis->pixelToCoord(pos.y()));
    QString txt = "Move here " + QString::number(m_mousePos.x()) + ", " + QString::number(m_mousePos.y());
    menu->addAction(txt, this, SLOT(moveCmd()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->popup(this->mapToGlobal(pos));

}

void CoordinateGrid::moveCmd()
{
    emit moveTo(m_mousePos);

}

