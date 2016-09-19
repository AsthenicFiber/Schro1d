#include "mesh.h"

Mesh::Mesh()
{

}

QString Mesh::parse_input(QString in_text)
{
    QStringList in_text_list = in_text.split('\n',QString::SkipEmptyParts);
    QString out_text = "";
    layers.clear();

    for (int i = 0; i < in_text_list.length(); i++)
    {
        // Parse each line
        QString command = in_text_list[i].section(' ',0,0);
        if (QString("layer").startsWith(command.toLower()))
        {
            // search for parameters
            Layer layer;
            layer.material = in_text_list[i].section(' ',1,1);

            QStringList params = in_text_list[i].split(' ',QString::SkipEmptyParts);
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

            out_text.append(in_text_list[i].section(' ',1,-1));
        }
        else if (QString("contact").startsWith(command.toLower()))
        {
            out_text.append(in_text_list[i].section(' ',1,-1));
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

bool Mesh::read_matfile()
{
    return matdata.load();
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

    int x = 0;
    for (unsigned int i = 0; i < layers.size(); i++)
    {
        int x_min = x;
        int x_max = x + layers[i].d;
        while (x < x_max)
        {
            Efn[x][0] = layers[i].Efn;
            Efn[x][0] += layers[i].dEfn*double(x-x_min);
            Efn[x][0] += layers[i].ddEfn*double(x-x_min)*double(x-x_min)/2;
            Efp[x][0] = layers[i].Efp;
            Efp[x][0] += layers[i].dEfp*double(x-x_min);
            Efp[x][0] += layers[i].ddEfp*double(x-x_min)*double(x-x_min)/2;
            x++;
        }
    }
    return "Mesh Generated";
}
