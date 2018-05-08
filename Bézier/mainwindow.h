#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <string>
#include <fstream>
#include <QFile>
#include <iostream>
#include <QTextStream>
#include <QDebug>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void readData(QString filename);

private slots:
    void on_actionLoad_obj_File_triggered();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    std::ifstream file;
    QFile *objFile;
};

#endif // MAINWINDOW_H
