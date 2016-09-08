#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>
#include <QTextBrowser>
#include <QLineEdit>
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
    QPlainTextEdit * inputText;
    QLineEdit * filenameText;

};

#endif // MAINWINDOW_H
