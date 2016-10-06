#include "mesh.h"
#include <cmath>

Mesh::Mesh()
{

}

QString Mesh::parse_input(QString in_text)
{
    QStringList in_text_list = in_text.split('\n',QString::SkipEmptyParts);
    QString out_text = "";
    layers.clear();
    matfile = "materials.csv";
    T = 300;
    schrodinger = true;

    for (int i = 0; i < in_text_list.length(); i++)
    {
        // Parse each line
        QString command = in_text_list[i].section(' ',0,0);
        if (QString("layer").startsWith(command.toLower()))
        {
            // search for parameters
            Layer layer;
            layer.material = in_text_list[i].section(QRegExp("[\\t ]"),1,1);

            QStringList params = in_text_list[i].split(QRegExp("[\\t ]"),QString::SkipEmptyParts);
            for (int j = 2; j < params.length(); j++)
            {
                //check if contains '='
                if (params[j].count('=') != 1)
                {
                    return parse_error(i+1,j-2);
                }
                QString param = params[j].section('=',0,0);
                QString val = params[j].section('=',1,1);
                // check values are not empty
                if (param == "" || val == "")
                {
                    return parse_error(i+1,j-2);
                }
                // Store data
                if (param == "d")
                    layer.d = val.toInt();
                else if (param == "X")
                    layer.X = val.toDouble();
                else if (param == "dX")
                    layer.dX = val.toDouble();
                else if (param == "ddX")
                    layer.ddX = val.toDouble();
                else if (param == "ternX")
                    layer.ternX = val;
                else if (param == "Y")
                    layer.Y = val.toDouble();
                else if (param == "dY")
                    layer.dY = val.toDouble();
                else if (param == "ddY")
                    layer.ddY = val.toDouble();
                else if (param == "ternY")
                    layer.ternY = val;
                else if (param == "Efn")
                    layer.Efn = val.toDouble();
                else if (param == "dEfn")
                    layer.dEfn = val.toDouble();
                else if (param == "ddEfn")
                    layer.ddEfn = val.toDouble();
                else if (param == "Efp")
                    layer.Efp = val.toDouble();
                else if (param == "dEfp")
                    layer.dEfp = val.toDouble();
                else if (param == "ddEfp")
                    layer.ddEfp = val.toDouble();
                else if (param.startsWith("Ed"))
                {
                    QString dope_index = param.remove(0,2);
                    layer.layerdoping[dope_index].E = val.toDouble();
                    layer.layerdoping[dope_index].type = 'n';
                }
                else if (param.startsWith("dEd"))
                {
                    QString dope_index = param.remove(0,3);
                    layer.layerdoping[dope_index].dE = val.toDouble();
                    layer.layerdoping[dope_index].type = 'n';
                }
                else if (param.startsWith("ddEd"))
                {
                    QString dope_index = param.remove(0,4);
                    layer.layerdoping[dope_index].ddE = val.toDouble();
                    layer.layerdoping[dope_index].type = 'n';
                }
                else if (param.startsWith("Nd"))
                {
                    QString dope_index = param.remove(0,2);
                    layer.layerdoping[dope_index].N = val.toDouble();
                    layer.layerdoping[dope_index].type = 'n';
                }
                else if (param.startsWith("dNd"))
                {
                    QString dope_index = param.remove(0,3);
                    layer.layerdoping[dope_index].dN = val.toDouble();
                    layer.layerdoping[dope_index].type = 'n';
                }
                else if (param.startsWith("ddNd"))
                {
                    QString dope_index = param.remove(0,4);
                    layer.layerdoping[dope_index].ddN = val.toDouble();
                    layer.layerdoping[dope_index].type = 'n';
                }
                else if (param.startsWith("Ea"))
                {
                    QString dope_index = param.remove(0,2);
                    layer.layerdoping[dope_index].E = val.toDouble();
                    layer.layerdoping[dope_index].type = 'p';
                }
                else if (param.startsWith("dEa"))
                {
                    QString dope_index = param.remove(0,3);
                    layer.layerdoping[dope_index].dE = val.toDouble();
                    layer.layerdoping[dope_index].type = 'p';
                }
                else if (param.startsWith("ddEa"))
                {
                    QString dope_index = param.remove(0,4);
                    layer.layerdoping[dope_index].ddE = val.toDouble();
                    layer.layerdoping[dope_index].type = 'p';
                }
                else if (param.startsWith("Na"))
                {
                    QString dope_index = param.remove(0,2);
                    layer.layerdoping[dope_index].N = val.toDouble();
                    layer.layerdoping[dope_index].type = 'p';
                }
                else if (param.startsWith("dNa"))
                {
                    QString dope_index = param.remove(0,3);
                    layer.layerdoping[dope_index].dN = val.toDouble();
                    layer.layerdoping[dope_index].type = 'p';
                }
                else if (param.startsWith("ddNa"))
                {
                    QString dope_index = param.remove(0,4);
                    layer.layerdoping[dope_index].ddN = val.toDouble();
                    layer.layerdoping[dope_index].type = 'p';
                }
                else
                {
                    return parse_error(i+1,j-2);
                }
            }

            layers.push_back(layer);

            out_text.append(in_text_list[i].section(QRegExp("[\\t ]"),1,-1));
        }
        else if (QString("contact").startsWith(command.toLower()))
        {
            out_text.append(in_text_list[i].section(QRegExp("[\\t ]"),1,-1));
        }
        else if (QString("matfile").startsWith(command.toLower()))
        {
            out_text.append(in_text_list[i].section(QRegExp("[\\t ]"),1,-1));
            matfile = in_text_list[i].section(QRegExp("[\\t ]"),1,1);
        }
        else if (QString("temp").startsWith(command.toLower()))
        {
            out_text.append(in_text_list[i].section(QRegExp("[\\t ]"),1,-1));
            T = in_text_list[i].section(QRegExp("[\\t ]"),1,1).toDouble();
        }
        else if (QString("schro").startsWith(command.toLower()))
        {
            out_text.append(in_text_list[i].section(QRegExp("[\\t ]"),1,-1));
            QString b_val = in_text_list[i].section(QRegExp("[\\t ]"),1,1).toLower();
            if (QString("false").startsWith(b_val))
            {
                schrodinger = false;
            }
            else if (QString("true").startsWith(b_val))
            {
                schrodinger = true;
            }
            else
            {
                return parse_error(i+1,1);
            }
        }
        else
        {
            out_text.append(in_text_list[i]);
        }
        out_text.append("\n");
    }
    return out_text;
}

QString parse_error(int line, int parameter)
{
    return QString("Error Parsing Line %1, Parameter %2").arg(line).arg(parameter);
}

QString Mesh::read_matfile()
{
    return matdata.load(matfile);
}

QString Mesh::generate()
{
    length = 0;
    unsigned int max_dopants = 0;
    for (unsigned int i = 0; i < layers.size(); i++)
    {
        length = length + layers[i].d;
        if (layers[i].layerdoping.size() > max_dopants)
            max_dopants = layers[i].layerdoping.size();
    }

    Efn = Matrix(length,1);
    Efp = Matrix(length,1);
    Eg = Matrix(length,1);
    Ec = Matrix(length,1);
    eps = Matrix(length,1);
    me = Matrix(length,1);
    mlh = Matrix(length,1);
    mhh = Matrix(length,1);
    mh = Matrix(length,1);
    pol = Matrix(length,1);
    Matrix pol_ = Matrix(length,1);

    Dopant new_dopant;
    new_dopant.E = Matrix(length,1);
    new_dopant.N = Matrix(length,1);
    doping.resize(max_dopants*2, new_dopant);

    psip = Matrix(length,length);
    psiph = Matrix(length,length);
    psin = Matrix(length,length);
    Ec = Matrix(length,1);
    Q = Matrix(length,1);
    V = Matrix(length,1);
    Up = Matrix(length,1);
    Un = Matrix(length,1);
    Ep = Matrix(length,1);
    Eph = Matrix(length,1);
    En = Matrix(length,1);
    Qp = Matrix(length,1);
    Qn = Matrix(length,1);

    int x = 0;
    for (unsigned int i = 0; i < layers.size(); i++)
    {
        int x_min = x;
        int x_max = x + layers[i].d;
        while (x < x_max)
        {
            Material matx;

            if (layers[i].ternY != "" && layers[i].ternX != "")
            {
                // find quaternary
                double X = d2val(layers[i].X, layers[i].dX, layers[i].ddX, x-x_min);
                double Y = d2val(layers[i].Y, layers[i].dY, layers[i].ddY, x-x_min);

                matx = Material(matdata[layers[i].material],matdata[layers[i].ternX],X/(1-Y));
                matx = Material(matx,matdata[layers[i].ternY],Y);
            }
            else if (layers[i].ternX != "")
            {
                // find ternary
                double X = d2val(layers[i].X, layers[i].dX, layers[i].ddX, x-x_min);
                matx = Material(matdata[layers[i].material],matdata[layers[i].ternX],X);
            }
            else
            {
                // return semiconductor
                matx = matdata[layers[i].material];
            }

            Eg[x][0] = matx.Eg;
            Ec[x][0] = -matx.chi;

            Efn[x][0] = -d2val(layers[i].Efn, layers[i].dEfn, layers[i].ddEfn,x-x_min);
            Efp[x][0] = -d2val(layers[i].Efp, layers[i].dEfp, layers[i].ddEfp,x-x_min);

            eps[x][0] = matx.eps*EPSo; // e/c*s*V
            me[x][0] = matx.m_e*Me;
            mlh[x][0] = matx.m_lh*Me;
            mhh[x][0] = matx.m_hh*Me;
            mh[x][0] = pow(double(pow(double(matx.m_lh),double(1.5)) + pow(double(matx.m_hh),double(1.5))),double(.6667))*Me;

            if (matx.Psp != 0)
            {
                // change to have a different sub-layer than GaN
                pol_[x][0] = 2*(matdata["GaN"].a-matx.a)/matx.a*(matx.e31-matx.e33*matx.c13/matx.c33);
                pol_[x][0] += matx.Psp;
                pol_[x][0] *= 1e4/Qe; // convert to e/cm^3 (e/cm^2 from C/m^2 then times 1/dx)

                if (x == 0)
                {
                    pol[x][0] = pol_[x][0];
                }
                else if (x == x_min)
                {
                    pol[x][0] = pol_[x][0] - pol_[x-1][0];
                }
                else if (x == length-1)
                {
                    pol[x][0] = -pol_[x][0];
                }
                else
                {
                    pol[x][0] = pol_[x][0] - pol_[x-1][0];
                }
            }
            std::map<QString,LayerDopant>::iterator it = layers[i].layerdoping.begin();
            int N_dop = 0;
            double dop_conc = 0;
            while (it != layers[i].layerdoping.end())
            {
                double N_ = it->second.N;
                double dN_ = it->second.dN;
                double ddN_ = it->second.ddN;
                double E_ = it->second.E;
                double dE_ = it->second.dE;
                double ddE_ = it->second.ddE;

                if (it->second.type == 'n')
                {
                    doping[N_dop].N[x][0] = d2val(N_,dN_,ddN_,x-x_min);
                    doping[N_dop].E[x][0] = d2val(E_,dE_,ddE_,x-x_min);
                    doping[N_dop].type = 'n';
                    dop_conc += doping[N_dop].N[x][0];
                }
                else if (it->second.type == 'p')
                {
                    doping[N_dop + max_dopants].N[x][0] = d2val(N_,dN_,ddN_,x-x_min);
                    doping[N_dop + max_dopants].E[x][0] = d2val(E_,dE_,ddE_,x-x_min);
                    doping[N_dop + max_dopants].type = 'p';
                    dop_conc -= doping[N_dop].N[x][0];
                }
                N_dop++;
                ++it;
            }
            if (x == 0)
            {
                double kT = kB*T; // eV
                double h = hP; //eV*s
                double Nc = 2*pow(double(2*PI*me[i][0]*kT/(h*h)),double(1.5));
                Nc *= DX*DX*DX*1e24; // convert to 1/cm^3
                double Nv = 2*pow(double(2*PI*mh[i][0]*kT/(h*h)),double(1.5));
                Nv *= DX*DX*DX*1e24; // convert to 1/cm^3
                double ni = sqrt(Nc*Nv*exp(-Eg[x][0]/kT));
                Efn[x][0] += Ec[x][0] - Eg[x][0]/2;
                Efp[x][0] += Ec[x][0] - Eg[x][0]/2;
                if (dop_conc > 0)
                {
                    Efn[x][0] += kT*log(dop_conc/ni);
                    Efp[x][0] += kT*log(dop_conc/ni);
                }
                else if (dop_conc < 0)
                {
                    Efn[x][0] -= kT*log(-dop_conc/ni);
                    Efp[x][0] -= kT*log(-dop_conc/ni);
                }
                else
                {
                    Efn[x][0] += 3*kT/4*log(mh[x][0]/me[x][0]);
                    Efp[x][0] += 3*kT/4*log(mh[x][0]/me[x][0]);
                }
            }
            else
            {
                Efn[x][0] += Efn[0][0] + layers[0].Efn;
                Efp[x][0] += Efp[0][0] + layers[0].Efp;
            }
            x++;
        }
    }
    for (unsigned int i = 0; i < doping.size(); i++)
    {
        if (doping[i].type == 'n')
        {
            doping[i].E = doping[i].E*-1 + Ec; // negative value relative to Evac
        }
        else if (doping[i].type == 'p')
        {
            doping[i].E = doping[i].E + Ec + Eg*-1; // negative value relative to Evac
        }
    }
    return "Mesh Generated";
}

void Mesh::calc_potentials()
{
    double q = 1; // electrons
    //double Evac = 0;
    Un = V*-q + Ec;
    Up = V*q + (Ec*-1) + Eg;
}

void Mesh::calc_charges()
{
    if (schrodinger)
    {
        Qn = q_psi(Un,Efn,me,En,psin,T);
        Qp = q_psi(Up,Efp*-1,mlh,Ep,psip,T) + q_psi(Up,Efp*-1,mhh,Eph,psiph,T);
    }
    else
    {
        Qn = q_boltz(Un,Efn,me,T);
        Qp = q_boltz(Up,Efp*-1,mh,T);
    }
    Matrix rho = Qn*-1 + Qp; // e/cm^3
    Q = pol; // e/cm^3
    Q += rho;

    // Add ionized dopants to charge profile
    for (unsigned int i = 0; i < doping.size(); i++)
    {
        if (doping[i].type == 'n')
        {
            Q += Nd_ion(doping[i].E,doping[i].N);
        }
        else if (doping[i].type == 'p')
        {
            Q += Na_ion(doping[i].E,doping[i].N)*-1;
        }
    }
}

Matrix Mesh::Nd_ion(Matrix Ed, Matrix Nd)
{
    double kT = kB*T; //eV
    Matrix Nd_i = Matrix(length,1);
    for (int i = 0; i < length; i++)
    {
        Nd_i[i][0] = Nd[i][0]/(1 + 2*exp((Efn[i][0] - Ed[i][0])/kT)); // e/(cm^3)
    }
    return Nd_i;
}

Matrix Mesh::Na_ion(Matrix Ea, Matrix Na)
{
    double kT = kB*T; //eV
    Matrix Na_i = Matrix(length,1);
    for (int i = 0; i < length; i++)
    {
        Na_i[i][0] = Na[i][0]/(1 + 4*exp((Ea[i][0] - Efp[i][0])/kT)); // e/(cm^3)
    }
    return Na_i;
}

double d2val(double a, double da, double dda, double x)
{
    return a + da*x + dda*x*x/2;
}

void Mesh::poiss()
{
    poiss_solve(Q, eps, &V);
}

void Mesh::schro()
{
    schro_solve(Un, me, &psin, &En);
    schro_solve(Up, mlh, &psip, &Ep);
    schro_solve(Up, mhh, &psiph, &Eph);
}

void Mesh::solve_SP()
{
    calc_potentials();
    for (int i = 0; i < 20; i++)
    {
        if (schrodinger)
            schro();
        calc_charges();
        poiss();
        calc_potentials();
    }
}
