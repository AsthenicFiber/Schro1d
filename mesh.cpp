#include "mesh.h"

Mesh::Mesh()
{

}

QString Mesh::parse_input(QString in_text)
{
    QStringList in_text_list = in_text.split('\n',QString::SkipEmptyParts);
    QString out_text = "";
    layers.clear();
    matfile = "materials.csv";

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
                {
                    layer.d = val.toInt();
                }
                else if (param == "X")
                {
                    layer.X = val.toDouble();
                }
                else if (param == "dX")
                {
                    layer.dX = val.toDouble();
                }
                else if (param == "ddX")
                {
                    layer.ddX = val.toDouble();
                }
                else if (param == "ternX")
                {
                    layer.ternX = val;
                }
                else if (param == "Y")
                {
                    layer.Y = val.toDouble();
                }
                else if (param == "dY")
                {
                    layer.dY = val.toDouble();
                }
                else if (param == "ddY")
                {
                    layer.ddY = val.toDouble();
                }
                else if (param == "ternY")
                {
                    layer.ternY = val;
                }
                else if (param == "Efn")
                {
                    layer.Efn = val.toDouble();
                }
                else if (param == "dEfn")
                {
                    layer.dEfn = val.toDouble();
                }
                else if (param == "ddEfn")
                {
                    layer.ddEfn = val.toDouble();
                }
                else if (param == "Efp")
                {
                    layer.Efp = val.toDouble();
                }
                else if (param == "dEfp")
                {
                    layer.dEfp = val.toDouble();
                }
                else if (param == "ddEfp")
                {
                    layer.ddEfp = val.toDouble();
                }
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

    for (unsigned int i = 0; i < layers.size(); i++)
    {
        length = length + layers[i].d;
    }

    Efn = Matrix(length,1);
    Efp = Matrix(length,1);
    Eg = Matrix(length,1);
    Ec = Matrix(length,1);
    eps = Matrix(length,1);
    me = Matrix(length,1);
    mh = Matrix(length,1);
    pol = Matrix(length,1);
    Matrix pol_ = Matrix(length,1);

    psip = Matrix(length,1);
    psin = Matrix(length,1);
    Ec = Matrix(length,1);
    Q = Matrix(length,1);
    V = Matrix(length,1);
    Up = Matrix(length,1);
    Un = Matrix(length,1);

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
                double X = layers[i].X + layers[i].dX*double(x-x_min) + layers[i].ddX*double(x-x_min)*double(x-x_min)/2;
                double Y = layers[i].Y + layers[i].dY*double(x-x_min) + layers[i].ddY*double(x-x_min)*double(x-x_min)/2;
                matx = Material(matdata[layers[i].material],matdata[layers[i].ternX],X/(1-Y));
                matx = Material(matx,matdata[layers[i].ternY],Y);
            }
            else if (layers[i].ternX != "")
            {
                // find ternary
                double X = layers[i].X + layers[i].dX*double(x-x_min) + layers[i].ddX*double(x-x_min)*double(x-x_min)/2;
                matx = Material(matdata[layers[i].material],matdata[layers[i].ternX],X);
            }
            else
            {
                // return semiconductor
                matx = matdata[layers[i].material];
            }

            Efn[x][0] = layers[i].Efn;
            Efn[x][0] += layers[i].dEfn*double(x-x_min);
            Efn[x][0] += layers[i].ddEfn*double(x-x_min)*double(x-x_min)/2;
            Efp[x][0] = layers[i].Efp;
            Efp[x][0] += layers[i].dEfp*double(x-x_min);
            Efp[x][0] += layers[i].ddEfp*double(x-x_min)*double(x-x_min)/2;

            Efn *= -1;
            Efp *= -1;

            Eg[x][0] = matx.Eg;
            Ec[x][0] = -matx.chi;
            eps[x][0] = matx.eps;
            me[x][0] = matx.m_e;
            mh[x][0] = pow((pow(matx.m_lh,3/2) + pow(matx.m_hh,3/2)),2/3);
            if (matx.Psp != 0)
            {
                pol_[x][0] = matx.Psp + 2*(3.191 - matx.a)/matx.a*(matx.e31-matx.e33*matx.c13/matx.c33);
                if (x == 0)
                {
                    pol[x][0] = pol_[x][0];
                }
                else if (x == x_min)
                {
                    pol[x][0] = pol_[x][0]-pol_[x-1][0];
                }
                else if (x == length-1)
                {
                    pol[x][0] = -pol_[x][0];
                }
            }

            x++;
        }
    }
    return "Mesh Generated";
}

void Mesh::calc_potentials()
{
    double q = 1.602e-19;
    //double Evac = 0;
    Un = V*-q + (Ec*-1)*-1;
    Up = V*q + (Ec*-1) + Eg;
}

void Mesh::calc_charges()
{
    //double q = 1.602e-19;

    //Matrix rho = -1*sum_over_En((Efn + -1*Ec)*psin(En)*psin(En)) + sum_over_Ep((Ec + Eg + -1*Efp)*psin(En)*psin(En));
    Matrix rho = Matrix(length,1);
    Q = rho + pol;

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
    double kT = 0.26; //eV
    Matrix Nd_i = Matrix(length,1);
    for (int i = 0; i < length; i++)
    {
        Nd_i[i][0] = Nd[i][0]/(1 + 2*exp((Efn[i][0] - Ed[i][0])/kT));
    }
    return Nd;
}

Matrix Mesh::Na_ion(Matrix Ea, Matrix Na)
{
    double kT = 0.26; //eV
    Matrix Na_i = Matrix(length,1);
    for (int i = 0; i < length; i++)
    {
        Na_i[i][0] = Na[i][0]/(1 + 4*exp((Ea[i][0] - Efp[i][0])/kT));
    }
    return Na_i;
}

Matrix Mesh::n_boltz()
{
    double kT = 0.26; //eV
    double h = 4.13567e-15; //eV*s
    Matrix n = Matrix(length,1);
    for (int i = 0; i < length; i++)
    {
        double Nc = 2*pow(2*3.14159*me[i][0]*kT/(h*h),3/2);
        // non-degenerate
        n[i][0] = Nc*exp(-(Ec[i][0] - Efn[i][0])/kT);
    }
    return n;
}

Matrix Mesh::p_boltz()
{
    double kT = 0.26; //eV
    double h = 4.13567e-15; //eV*s
    Matrix p = Matrix(length,1);
    Matrix Ev = Ec + Eg*-1;
    for (int i = 0; i < length; i++)
    {
        double Nv = 2*pow(2*3.14159*mh[i][0]*kT/(h*h),3/2);
        // non-degenerate
        p[i][0] = Nv*exp(-(Efn[i][0] - Ev[i][0])/kT);
    }
    return p;
}
