#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <string>
#include <fstream>

//if visual studio c++
//#include <complex>
//#define lapack_complex_float std::complex<float>
//#define lapack_complex_double std::complex<double>

#include "lapacke.h"
#include "matrix.h"
#include "schro_pois.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    build_form();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString print_mat(Matrix A)
{
    QString text;
    for (int i = 0; i < A.rows(); i++)
    {
        for (int j = 0; j < A.cols(); j++)
        {
            text.append(QString("%1").arg(A[i][j]));
            if (j != A.cols()-1)
            {
                text.append("\t");
            }
        }
        text.append("\n");
    }
    return text;
}

QString print_mat(double *A, int rows, int cols)
{
    QString text;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            text.append(QString("%1").arg(A[i*cols+j]));
            if (j != cols-1)
            {
                text.append("\t");
            }
        }
        text.append("\n");
    }
    return text;
}

QString print_mat(int *A, int rows, int cols)
{
    QString text;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            text.append(QString("%1").arg(A[i*cols+j]));
            if (j != cols-1)
            {
                text.append("\t");
            }
        }
        text.append("\n");
    }
    return text;
}

void MainWindow::on_runButton_clicked()
{
    // Clear mesh and disconnect
    mesh = Mesh();
    disconnect(chargescene, SIGNAL(rangeSelected(int)), this, SLOT(on_rangeSelected()));

    // Parse input text and generate mesh
    QString in_text = inputText->toPlainText();
    outputText->append(mesh.parse_input(in_text));
    outputText->append(mesh.read_matfile());
    outputText->append(mesh.generate());

    // Run calculation
    mesh.solve_SP();

    outputText->append(QString("x\tEc\tEfn\tEv\t\tn\tp\tpol\tQ\tV"));
    for (int i = 0; i < mesh.V.rows(); i++)
    {
        outputText->append(QString("%1\t%2\t%3\t%4\t\t%5\t%6\t%7\t%8\t%9").arg(i).arg(mesh.Un[i][0]).arg(mesh.Efn[i][0]).arg(-mesh.Up[i][0]).arg(mesh.Qn[i][0]).arg(mesh.Qp[i][0]).arg(mesh.pol[i][0]).arg(mesh.Q[i][0]).arg(mesh.V[i][0]));
    }

    plot_band(mesh.V,mesh.Un,mesh.Up*-1,mesh.Efn,mesh.Efp);
    plot_charge(mesh.Qn,mesh.Qp,mesh.Q);
    connect(chargescene, SIGNAL(rangeSelected(int)), this, SLOT(on_rangeSelected()));
    nsLabel->setText("Select range to calculate ns.");
}

void MainWindow::testrun()
{
    //outputText->append("Hello World!");
    std::string in_text = inputText->toPlainText().toStdString();
    outputText->append(QString::fromStdString(in_text));

    Matrix A(2,2);
    A[0][0] = 1;
    A[0][1] = 2;
    A[1][0] = 2;
    A[1][1] = 3;
    Matrix Avec = A;

    Matrix B(2,1);
    B[0][0] = 1;
    B[1][0] = 1;
    Matrix Bi = B;

    int ipiv[2] = {0,0};
    outputText->append(print_mat(ipiv,2,1));

    int n = A.rows();
    int nrhs = B.cols();
    int lda = A.cols();
    int ldb = B.cols();

    LAPACKE_dgesv(LAPACK_ROW_MAJOR,n,nrhs,A.mat(),lda,ipiv,B.mat(),ldb);
    LAPACKE_dgeev(LAPACK_ROW_MAJOR,'N','V',n,A.mat(),lda,B.mat(),Bi.mat(),Avec.mat(),Avec.cols(),Avec.mat(),Avec.cols());
    LAPACKE_dsysv(LAPACK_ROW_MAJOR,'U',n,nrhs,A.mat(),lda,ipiv,B.mat(),ldb);
    LAPACKE_dsyev(LAPACK_ROW_MAJOR,'V','U',n,A.mat(),lda,B.mat());

    //outputText->append(print_mat(ipiv,2,1));
    //outputText->append(print_mat(Avec));
    outputText->append(print_mat(B));
    outputText->append(print_mat(A));

    Matrix D(5,5);
    D = diff2_n(5);

    Matrix M(5,1);
    for (int i = 0; i < M.rows(); i++)
    {
        M[i][0] = i+1;
    }

    M = vec2diag(M);
    A = (D*M+M*D)*.5;
    B = Matrix(5,1);
    outputText->append(print_mat(A));
    LAPACKE_dsyev(LAPACK_ROW_MAJOR,'V','U',A.rows(),A.mat(),A.cols(),B.mat());
    outputText->append(print_mat(B));
    outputText->append(print_mat(A));
}

void MainWindow::on_saveButton_clicked()
{
    std::string filenm = filenameText->text().toStdString();
    //filenm.append(".txt");
    if (filenm == "" || filenm == ".txt")
    {
        outputText->append("Enter Filename");
        return;
    }

    std::ofstream  outfile;
    outfile.open(filenm, std::ifstream::out | std::ofstream::trunc);
    if (outfile.is_open())
    {
        outputText->append("Saving File");
        //char buffer[1000];
        //buffer = inputText->toPlainText().toLatin1().data();
        int len = inputText->toPlainText().size();
        outfile.write(inputText->toPlainText().toLatin1().data(),len);
    }
    else
    {
        outputText->append("Error Saving File");
    }
    outfile.close();
}

void MainWindow::on_loadButton_clicked()
{
    std::string filenm = filenameText->text().toStdString();
    //filenm.append(".txt");

    std::ifstream  infile;
    infile.open(filenm, std::ifstream::in);
    if (infile.is_open())
    {
        outputText->append("File Opened");
        inputText->clear();
        QString text;
        while (!infile.eof())
        {
            text.append(infile.get());
        }
        text.chop(1);
        inputText->appendPlainText(text);
    }
    else
    {
        outputText->append("Error Openning File");
    }
    infile.close();
}

void MainWindow::on_filenameText_editingFinished()
{
    QString filenm = filenameText->text();
    QString filext = filenm.section('.',-1,-1);
    if (filext != "txt")
    {
        filenm.append(".txt");
    }
    filenameText->setText(filenm);
}

void MainWindow::build_form()
{
    runButton = new QPushButton("Run",this);
    runButton->setGeometry(QRect(QPoint(450,320),QSize(75,23)));
    connect(runButton, SIGNAL(clicked()), this, SLOT(on_runButton_clicked()));

    saveButton = new QPushButton("Save",this);
    saveButton->setGeometry(QRect(QPoint(450,240),QSize(75,23)));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(on_saveButton_clicked()));

    loadButton = new QPushButton("Load",this);
    loadButton->setGeometry(QRect(QPoint(450,280),QSize(75,23)));
    connect(loadButton, SIGNAL(clicked()), this, SLOT(on_loadButton_clicked()));

    inputText = new QPlainTextEdit(this);
    inputText->setGeometry(10,50,411,261);
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
    inputText->setFont(font);
    highlighter = new Highlighter(inputText->document());

    inputLabel = new QLabel(this);
    inputLabel->setText("Input Text");
    inputLabel->setGeometry(10,30,411,13);

    outputText = new QTextBrowser(this);
    outputText->setGeometry(10,350,411,261);

    outputLabel = new QLabel(this);
    outputLabel->setText("Output");
    outputLabel->setGeometry(10,330,411,13);

    filenameText = new QLineEdit(this);
    filenameText->setGeometry(450,200,120,20);
    filenameText->setPlaceholderText(QString("Filename"));
    connect(filenameText, SIGNAL(editingFinished()), this, SLOT(on_filenameText_editingFinished()));

    bandscene = new GraphicsPlot(0,0,400,260,this);
    bandview = new QGraphicsView(bandscene,this);
    bandview->setScene(bandscene);
    bandview->setGeometry(599,49,402,262);
    bandview->show();

    bandLabel = new QLabel(this);
    bandLabel->setText("Band Diagram");
    bandLabel->setGeometry(600,30,400,13);

    chargescene = new GraphicsPlot(0,0,400,260,this);
    chargeview = new QGraphicsView(chargescene,this);
    chargeview->setScene(chargescene);
    chargeview->setGeometry(599,349,402,262);
    chargeview->show();

    chargeLabel = new QLabel(this);
    chargeLabel->setText("Charge");
    chargeLabel->setGeometry(600,330,400,13);

    nsLabel = new QLabel(this);
    nsLabel->setText("");
    nsLabel->setGeometry(600,620,400,13);
}

void MainWindow::plot_band(Matrix V, Matrix Ec, Matrix Ev, Matrix Efn, Matrix Efp)
{
    bandscene->clear();
    bandscene->set_view(0,min(Ev),V.rows()-1,-min(V));
    bandscene->plot_matrix(V*-1,QPen(Qt::green));
    bandscene->plot_matrix(Ec,QPen(Qt::black));
    bandscene->plot_matrix(Ev,QPen(Qt::black));
    bandscene->plot_matrix(Efn,QPen(Qt::blue));
    bandscene->plot_matrix(Efp,QPen(Qt::red));
}

void MainWindow::plot_charge(Matrix n, Matrix p, Matrix Q)
{
    chargescene->clear();
    double top;
    double bot;
    bot = min(Q,1,Q.rows()-1); // remove this line when Q input is removed
    if (max(n) > max(p))
        top = max(n);
    else
        top = max(p);
    if (min(n) < min(p))
        bot = min(n);
    else
        bot = min(p);

    chargescene->set_view(0,bot,n.rows()-1,top);
    //chargescene->plot_matrix(Q,QPen(Qt::black));
    chargescene->plot_matrix(n,QPen(Qt::blue));
    chargescene->plot_matrix(p,QPen(Qt::red));
}

void MainWindow::on_rangeSelected()
{
    int x1 = chargescene->x1();
    int x2 = chargescene->x2();
    double ns, ps;
    if (x1 < x2)
    {
        ns = sum(mesh.Qn,x1,x2)*1e-8;
        ps = sum(mesh.Qp,x1,x2)*1e-8;
    }
    else
    {
        ns = sum(mesh.Qn,x2,x1)*1e-8;
        ps = sum(mesh.Qp,x2,x1)*1e-8;
    }
    nsLabel->setText(QString("x1 = %1A, x2 = %2A, ns = %3 cm^-2, ps = %4 cm^-2").arg(x1).arg(x2).arg(ns).arg(ps));
}
