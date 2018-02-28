#ifndef COORDINATEGRID_H
#define COORDINATEGRID_H

#include "qcustomplot.h"

#include <QMenu>

class CoordinateGrid : public QCustomPlot
{
    Q_OBJECT
public:
    explicit CoordinateGrid(QWidget* parent = NULL);
protected:
    QVector<double> m_x;
    QVector<double> m_y;

    QVector<double> m_currX;
    QVector<double> m_currY;

    QPointF m_mousePos;
    virtual void mouseMoveEvent(QMouseEvent *event);

public slots:
    void clearGrid();
    void addPoint(QPointF);
    void setCurrentPoint(QPointF);

private slots:
    void mousePressed(QMouseEvent*);
    void contextMenuRequest(QPoint);
    void moveCmd();
signals:
    void moveTo(QPointF);

};

#endif // COORDINATEGRID_H
