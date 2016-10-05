#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>
#include <QTextBrowser>
#include <QLineEdit>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "mesh.h"
#include "highlighter.h"
//#include <map>

void plot_matrix(Matrix A, double x_p, double y_p, double top, QPen pen, QGraphicsScene *scene);

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_runButton_clicked();
    void on_saveButton_clicked();
    void on_loadButton_clicked();
    void on_filenameText_editingFinished();
    void testrun();

private:
    Ui::MainWindow *ui;
    QPushButton * runButton;
    QPushButton * saveButton;
    QPushButton * loadButton;
    QTextBrowser * outputText;
    QLabel * outputLabel;
    QLabel * inputLabel;
    QLabel * bandLabel;
    QLabel * chargeLabel;
    QPlainTextEdit * inputText;
    QLineEdit * filenameText;
    QGraphicsScene * bandscene;
    QGraphicsView * bandview;
    QGraphicsScene * chargescene;
    QGraphicsView * chargeview;

    Highlighter *highlighter;

    Mesh mesh;

    void build_form();
    void plot_band(Matrix V, Matrix Ec, Matrix Ev, Matrix Efn, Matrix Efp);
    void plot_charge(Matrix n, Matrix p, Matrix Q);

};

#endif // MAINWINDOW_H
