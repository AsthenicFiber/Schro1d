#include "graphicsplot.h"
#include <QGraphicsSceneMouseEvent>
#include <cmath>

GraphicsPlot::GraphicsPlot()
{

}

GraphicsPlot::GraphicsPlot(int X, int Y, int width, int height, QObject * parent)
{
    this->setSceneRect(X,Y,width,height);
    this->setParent(parent);
    x_pix = 1;
    y_pix = 1;
}

void GraphicsPlot::set_view(double x_min, double y_min, double x_max, double y_max)
{
    bottom = y_min;
    top = y_max;
    left = x_min;
    right = x_max;

    x_pix = this->width()/(right - left);
    y_pix = this->height()/(top - bottom);

    // setup axis labels

}

void GraphicsPlot::plot_matrix(Matrix A, QPen pen)
{
    for (int i = 1; i < A.rows(); i++)
    {
        double x0 = x_pix*double(i-1);
        double x1 = x_pix*double(i);
        double y0 = y_pix*(-A[i-1][0] + top);
        double y1 = y_pix*(-A[i][0] + top);
        this->addLine(x0,y0,x1,y1,pen);
    }
}

void GraphicsPlot::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    //QPointF pnt = mouseEvent->buttonDownScenePos(Qt::LeftButton);
    QPointF pnt = mouseEvent->scenePos();
    X1 = pnt.x()/x_pix;
    if (X1 < left)
        X1 = left;
    else if (X1 > right)
        X1 = right;
}

void GraphicsPlot::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    QPointF pnt = mouseEvent->scenePos();
    X2 = pnt.x()/x_pix;
    if (X2 < left)
        X2 = left;
    else if (X2 > right)
        X2 = right;
    emit rangeSelected(int(round(X2)));
}

int GraphicsPlot::x1()
{
    return int(round(X1));
}

int GraphicsPlot::x2()
{
    return int(round(X2));
}
