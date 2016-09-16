#ifndef MESH_H
#define MESH_H

#include <QString>
#include <QStringList>
#include <vector>
#include <map>
#include "matrix.h"

struct Dopant
{
    Matrix N;
    Matrix E;
    int type;
    Dopant():type('i'){}
};

struct LayerDopant
{
    double N,dN,ddN;
    double E,dE,ddE;
    char type;
    LayerDopant():N(0),dN(0),ddN(0),E(0),dE(0),ddE(0),type('i'){}
};

struct Layer
{
    QString material;
    int d;
    double X,dX,ddX;
    double Efn,dEfn,ddEfn;
    double Efp,dEfp,ddEfp;
    std::map <QString,LayerDopant> layerdoping;
    Layer():d(0),X(0),dX(0),ddX(0),Efn(0),dEfn(0),ddEfn(0),Efp(0),dEfp(0),ddEfp(0){}
};

QString parse_error(int line, int parameter);

class Mesh
{
public:
    Mesh();
    QString parse_input(QString in_text);
    QString generate();

public:
    Matrix Efn;
    Matrix Efp;

private:
    std::vector <Layer> layers;
    Matrix psip;
    Matrix psin;
    Matrix Eg;
    Matrix Ec;
    Matrix Q;
    std::vector <Dopant> doping;
    Matrix eps;
    Matrix V;
    Matrix Un;
    Matrix Up;
    Matrix me;
    Matrix mh;
    Matrix Ep;
    Matrix En;
    int length;
};

#endif // MESH_H
