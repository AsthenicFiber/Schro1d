#ifndef GRAPHICSPLOT_H
#define GRAPHICSPLOT_H

#include <QGraphicsScene>
//#include <QObject>
#include "matrix.h"

class GraphicsPlot : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicsPlot();
    GraphicsPlot(int X, int Y, int width, int height, QObject * parent = nullptr);
    void set_view(double x_min, double y_min, double x_max, double y_max);
    void plot_matrix(Matrix A, QPen pen = QPen(Qt::black));

public:
    int x1();
    int x2();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);

signals:
    void rangeSelected(int X);

private:
    double x_pix;
    double y_pix;
    double top;
    double right;
    double bottom;
    double left;
    double X1;
    double X2;
};

#endif // GRAPHICSPLOT_H
