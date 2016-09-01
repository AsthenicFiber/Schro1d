#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include "string.h"
#include <armadillo/armadillo>
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

std::string print_mat(arma::mat A)
{
    std::string text;
    for (unsigned int i = 0; i < A.n_rows; i++)
    {
        for (unsigned int j = 0; j < A.n_cols; j++)
        {
            char buffer [50];
            //sprintf(buffer,"%g",A(i,j));
            sprintf_s(buffer,sizeof(buffer),"%g",A(i,j));
            text.append(buffer);
            //text.append(std::to_string(A(i,j)));
            if (j != A.n_cols)
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
    arma::mat A(2,2);
    A << 1 << 2.4 << arma::endr
      << 3 << 4 << arma::endr;
    outputText->append(QString::fromStdString(print_mat(A)));
}
