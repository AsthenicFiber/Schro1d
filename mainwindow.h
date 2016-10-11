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
#include "graphicsplot.h"

#include "mesh.h"
#include "highlighter.h"

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
    void on_rangeSelected();

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
    QLabel * nsLabel;
    QPlainTextEdit * inputText;
    QLineEdit * filenameText;
    GraphicsPlot * bandscene;
    QGraphicsView * bandview;
    GraphicsPlot * chargescene;
    QGraphicsView * chargeview;

    Highlighter *highlighter;

    Mesh mesh;

    void build_form();
    void plot_band(Matrix V, Matrix Ec, Matrix Ev, Matrix Efn, Matrix Efp);
    void plot_charge(Matrix n, Matrix p, Matrix Q);

};

#endif // MAINWINDOW_H
