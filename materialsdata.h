#ifndef MATERIALSDATA_H
#define MATERIALSDATA_H

#include <map>
#include <QString>

struct Material
{
    QString name = "";
    double Eg = 0;
    double chi = 0;
    double eps = 1;
    double m_e = 1;
    double m_hh = 1;
    double m_lh = 1;
    double a = 1;
    double c = 1;
    double Psp = 0;
    double c11 = 1;
    double c12 = 1;
    double c13 = 1;
    double c33 = 1;
    double c44 = 1;
    double b = 0;
    double e31 = 0;
    double e33 = 0;
    double alpha = 1;
    double mu_n = 1;
    double mu_p = 1;
    double tau_n = 1;
    double tau_p = 1;
    std::map <QString,double> bowing;
    Material() {}
    Material(Material A, Material B, double X);
};

class MaterialsData
{
public:
    MaterialsData();
    QString load(QString matfilenm);
    QString load();
    Material operator [](QString name);

private:
    std::map <QString,Material> materialdata;
};

#endif // MATERIALSDATA_H
