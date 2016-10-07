#ifndef MESH_H
#define MESH_H

#include <QString>
#include <QStringList>
#include <vector>
#include <map>
#include "matrix.h"
#include "materialsdata.h"
#include "schro_pois.h"

struct Dopant
{
    Matrix N;
    Matrix E;
    char type;
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
    double Y,dY,ddY;
    double Efn,dEfn,ddEfn;
    double Efp,dEfp,ddEfp;
    QString ternX,ternY = "";
    std::map <QString,LayerDopant> layerdoping;
    Layer():d(0),X(0),dX(0),ddX(0),Y(0),dY(0),ddY(0),Efn(0),dEfn(0),ddEfn(0),Efp(0),dEfp(0),ddEfp(0){}
};

QString parse_error(int line, int parameter);

double d2val(double a, double da, double dda, double x);

class Mesh
{
public:
    Mesh();
    QString parse_input(QString in_text);
    QString read_matfile();
    QString generate();
    void solve_SP();

public:
    Matrix Efn;
    Matrix Efp;
    Matrix V;
    Matrix Un;
    Matrix Up;
    Matrix Qp;
    Matrix Qn;
    Matrix Q;
    Matrix pol;

private:
    std::vector <Layer> layers;
    Matrix psip;
    Matrix psiph;
    Matrix psin;
    Matrix Ec;
    Matrix Eg;
    std::vector <Dopant> doping;
    Matrix eps;
    Matrix me;
    Matrix mlh;
    Matrix mhh;
    Matrix mh;
    Matrix Ep;
    Matrix Eph;
    Matrix En;
    int length;
    MaterialsData matdata;
    QString matfile;
    double T;
    double pol_sc;
    QString pol_sub;
    bool schrodinger;

    Matrix Nd_ion(Matrix Ed, Matrix Nd);
    Matrix Na_ion(Matrix Ea, Matrix Na);

    //void norm_psi();
    void poiss();
    void schro();
    void calc_potentials();
    void calc_charges();
};

#endif // MESH_H
