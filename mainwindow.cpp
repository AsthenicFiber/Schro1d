#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include "string.h"

//if visual c++
#include <complex>
#define lapack_complex_float std::complex<float>
#define lapack_complex_double std::complex<double>

//#define ADD_
//#define HAVE_LAPACK_CONFIG_H
//#define LAPACK_COMPLEX_STRUCTURE

#include "lapacke.h"
#include "matrix.h"
//#include <armadillo/armadillo>
//using namespace arma;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    runButton = new QPushButton("Run",this);
    //runButton->setText("Run");
    runButton->setGeometry(QRect(QPoint(450,320),QSize(75,23)));
    connect(runButton, SIGNAL(clicked()), this, SLOT(on_runButton_clicked()));

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

}

MainWindow::~MainWindow()
{
    delete ui;
}

std::string print_mat(Matrix A)
{
    std::string text;
    for (int i = 0; i < A.rows(); i++)
    {
        for (int j = 0; j < A.cols(); j++)
        {
            char buffer [50];
#if defined(_MSC_VER) && _MSC_VER >= 1400
            sprintf_s(buffer,sizeof(buffer),"%g",A(i,j));
#else
            sprintf(buffer,"%g",A(i,j));
#endif
            text.append(buffer);
            //text.append(std::to_string(A(i,j)));
            if (j != A.cols())
            {
                text.append(" ");
            }
        }
        text.append("\n");
    }
    return text;
}

std::string print_mat(double *A, int rows, int cols)
{
    std::string text;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            char buffer [50];
#if defined(_MSC_VER) && _MSC_VER >= 1400
            sprintf_s(buffer,sizeof(buffer),"%g",A[i*cols+j]);
#else
            sprintf(buffer,"%g",A[i*cols+j]);
#endif
            text.append(buffer);
            //text.append(std::to_string(A(i,j)));
            if (j != cols)
            {
                text.append(" ");
            }
        }
        text.append("\n");
    }
    return text;
}

std::string print_mat(int *A, int rows, int cols)
{
    std::string text;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            char buffer [50];
            //sprintf(buffer,"%g",A[i*cols+j]);
#if defined(_MSC_VER) && _MSC_VER >= 1400
            sprintf_s(buffer,sizeof(buffer),"%g",A[i*cols+j]);
#else
            sprintf(buffer,"%d",A[i*cols+j]);
#endif
            text.append(buffer);
            //text.append(std::to_string(A(i,j)));
            if (j != cols)
            {
                text.append(" ");
            }
        }
        text.append("\n");
    }
    return text;
}

void MainWindow::on_runButton_clicked()
{
    //outputText->append("Hello World!");
    std::string in_text = inputText->toPlainText().toStdString();
    outputText->append(QString::fromStdString(in_text));

    Matrix A(2,2);
    A[0][0] = 1;
    A[0][1] = 2;
    A[1][0] = 2;
    A[1][1] = 4;
    //Matrix Avec = A;

    Matrix B(2,1);
    B[0][0] = 1;
    B[1][0] = 1;
    //Matrix Bi = B;

    //double a[2*2] = {1,2,3,4};
    //double b[2*1] = {1,1};
    //int ipiv[2] = {0,0};

    //int n = 2;
    //int nrhs = 1;
    //int lda = 2;
    //int ldb = 1;

    outputText->append(QString::fromStdString(print_mat(A.mat(),2,2)));
    outputText->append(QString::fromStdString(print_mat(B.mat(),2,1)));

    //int info = 0;

    //LAPACKE_dgesv(LAPACK_ROW_MAJOR,A.rows(),B.cols(),A.mat(),A.cols(),ipiv,B.mat(),B.cols());
    //LAPACKE_dgeev(LAPACK_ROW_MAJOR,'N','V',A.rows(),A.mat(),A.cols(),B.mat(),Bi.mat(),Avec.mat(),Avec.cols(),Avec.mat(),Avec.cols());
    //LAPACKE_dsysv(LAPACK_ROW_MAJOR,'U',A.rows(),B.cols(),A.mat(),A.cols(),ipiv,B.mat(),B.cols());
    LAPACKE_dsyev(LAPACK_ROW_MAJOR,'V','U',A.rows(),A.mat(),A.cols(),B.mat());

    //dgesv_(&n,&nrhs,a,&lda,ipiv,b,&ldb,&info);

    //outputText->append(QString::fromStdString(print_mat(ipiv,2,1)));
    //outputText->append(QString::fromStdString(print_mat(A.mat(),2,2)));
    //outputText->append(QString::fromStdString(print_mat(B.mat(),2,1)));
    //outputText->append(QString::fromStdString(print_mat(Avec)));
    outputText->append(QString::fromStdString(print_mat(B)));
    outputText->append(QString::fromStdString(print_mat(A)));
    //outputText->append(QString("%1\n").arg(B.cols()));


    //double A[2][2] = {1,2,3,4};
    //int ld = 2;

    //arma::mat A(2,2);
    //A << 1 << 2.4 << arma::endr << 3 << 4 << arma::endr;
}
