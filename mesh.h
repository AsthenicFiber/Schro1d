#ifndef MESH_H
#define MESH_H

#include <QString>
#include <QStringList>
#include <vector>
#include "matrix.h"


struct Dopant
{
    Matrix N;
    Matrix E;
    int type;
};

struct LayerDopant
{
    double N,dN,ddN;
    double E,dE,ddE;
    int type;
};

struct Layer
{
    QString material;
    double d;
    double X,dX,ddX;
    double Efn,dEfn,ddEfn;
    double Efp,dEfp,ddEfp;
    std::vector <LayerDopant> layerdoping;
};

QString parse_error(int line, int parameter);

class Mesh
{
public:
    Mesh();
    QString parse_input(QString in_text);

private:
    std::vector <Layer> layers;
    Matrix psi;
    Matrix Efn;
    Matrix Efp;
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
};

#endif // MESH_H
