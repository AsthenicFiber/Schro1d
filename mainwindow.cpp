#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <string>
#include <fstream>

//if visual c++
#include <complex>
#define lapack_complex_float std::complex<float>
#define lapack_complex_double std::complex<double>

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
    // Parse input text
    QString in_text = inputText->toPlainText();
    outputText->append(mesh.parse_input(in_text));

    // Run calculation
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

    inputLabel = new QLabel(this);
    inputLabel->setText("Input Text");
    inputLabel->setGeometry(10,30,411,13);

    outputText = new QTextBrowser(this);
    outputText->setGeometry(10,350,411,241);

    outputLabel = new QLabel(this);
    outputLabel->setText("Output");
    outputLabel->setGeometry(10,330,411,13);

    filenameText = new QLineEdit(this);
    filenameText->setGeometry(450,200,120,20);
    filenameText->setPlaceholderText(QString("Filename"));
    connect(filenameText, SIGNAL(editingFinished()), this, SLOT(on_filenameText_editingFinished()));
}
