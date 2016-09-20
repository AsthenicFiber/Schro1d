#include "materialsdata.h"

#include <fstream>
#include <QStringList>

MaterialsData::MaterialsData()
{

}

bool MaterialsData::load()
{
    std::fstream matfile;
    matfile.open("materials.csv",std::ifstream::in);
    if (!matfile.is_open())
    {
        return false;
    }
    QString mattext;
    while (!matfile.eof())
    {
        mattext.append(matfile.get());
    }
    mattext.chop(1);

    materialdata.clear();

    QStringList mattext_list = mattext.split('\n',QString::KeepEmptyParts);
    for (int i = 1; i < mattext_list.length(); i++)
    {
        QStringList mat_vals = mattext_list[i].split(',',QString::KeepEmptyParts);
        if (mat_vals.length() >= 23)
        {
            materialdata[mat_vals[0]].name = mat_vals[0];
            materialdata[mat_vals[0]].Eg = mat_vals[1].toDouble();
            materialdata[mat_vals[0]].chi = mat_vals[2].toDouble();
            materialdata[mat_vals[0]].eps = mat_vals[3].toDouble();
            materialdata[mat_vals[0]].m_e = mat_vals[4].toDouble();
            materialdata[mat_vals[0]].m_hh = mat_vals[5].toDouble();
            materialdata[mat_vals[0]].m_lh = mat_vals[6].toDouble();
            materialdata[mat_vals[0]].a = mat_vals[7].toDouble();
            materialdata[mat_vals[0]].c = mat_vals[8].toDouble();
            materialdata[mat_vals[0]].Psp = mat_vals[9].toDouble();
            materialdata[mat_vals[0]].c11 = mat_vals[10].toDouble();
            materialdata[mat_vals[0]].c12 = mat_vals[11].toDouble();
            materialdata[mat_vals[0]].c13 = mat_vals[12].toDouble();
            materialdata[mat_vals[0]].c33 = mat_vals[13].toDouble();
            materialdata[mat_vals[0]].c44 = mat_vals[14].toDouble();
            materialdata[mat_vals[0]].b = mat_vals[15].toDouble();
            materialdata[mat_vals[0]].e31 = mat_vals[16].toDouble();
            materialdata[mat_vals[0]].e33 = mat_vals[17].toDouble();
            materialdata[mat_vals[0]].alpha = mat_vals[18].toDouble();
            materialdata[mat_vals[0]].mu_n = mat_vals[19].toDouble();
            materialdata[mat_vals[0]].mu_p = mat_vals[20].toDouble();
            materialdata[mat_vals[0]].tau_n = mat_vals[21].toDouble();
            materialdata[mat_vals[0]].tau_p = mat_vals[22].toDouble();

            int j = 23;
            while (j < mat_vals.length()-1)
            {
                materialdata[mat_vals[0]].bowing[mat_vals[j]] = mat_vals[j+1].toDouble();
                j += 2;
            }
        }
    }

    return true;
}

Material::Material(Material A, Material B, double X)
{
    double bow = A.bowing[B.name];
    Eg = A.Eg + (B.Eg - A.Eg - bow)*X + bow*X*X;
    chi = A.chi*(1-X) + B.chi*X;
    eps = A.eps*(1-X) + B.eps*X;
    m_e = A.m_e*(1-X) + B.m_e*X;
    m_hh = A.m_hh*(1-X) + B.m_hh*X;
    m_lh = A.m_lh*(1-X) + B.m_lh*X;
    a = A.a*(1-X) + B.a*X;
    c = A.c*(1-X) + B.c*X;
    Psp = A.Psp*(1-X) + B.Psp*X;
    c11 = A.c11*(1-X) + B.c11*X;
    c12 = A.c12*(1-X) + B.c12*X;
    c13 = A.c13*(1-X) + B.c13*X;
    c33 = A.c33*(1-X) + B.c33*X;
    c44 = A.c44*(1-X) + B.c44*X;
    b = A.b*(1-X) + B.b*X;
    e31 = A.e31*(1-X) + B.e31*X;
    e33 = A.e33*(1-X) + B.e33*X;
    alpha = A.alpha*(1-X) + B.alpha*X;
    mu_n = A.mu_n*(1-X) + B.mu_n*X;
    mu_p = A.mu_p*(1-X) + B.mu_p*X;
    tau_n = A.tau_n*(1-X) + B.tau_n*X;
    tau_p = A.tau_p*(1-X) + B.tau_p*X;
    for (std::map<QString,double>::iterator it = A.bowing.begin(); it != A.bowing.end(); ++it)
    {
        bowing[it->first] = it->second;
    }
    for (std::map<QString,double>::iterator it = B.bowing.begin(); it != B.bowing.end(); ++it)
    {
        if (bowing.count(it->first))
        {
            bowing[it->first] = it->second*X + bowing[it->first]*(1-X);
        }
        else
        {
            bowing[it->first] = it->second;
        }
    }
}

Material MaterialsData::operator [] (QString name)
{
    return materialdata[name];
    //return &p[row*n];
}
