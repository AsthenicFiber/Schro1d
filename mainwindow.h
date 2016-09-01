#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>
#include <QTextBrowser>
//#include <map>

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

private:
    Ui::MainWindow *ui;
    QPushButton * runButton;
    QTextBrowser * outputText;
    QLabel * outputLabel;
    QLabel * inputLabel;
    QPlainTextEdit * inputText;

};

#endif // MAINWINDOW_H
