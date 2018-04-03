#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, &MainWindow::lineRead, ui->glwidget, &OGLWidget::lineRead);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readData(QString filename)
{
    QString fullFilename = filename;
    qDebug() << "Opening: " + fullFilename;
    this->objFile = new QFile(fullFilename);

    if (!this->objFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << "error opening file" << std::endl;
        return;
    }

    QString key;
    float x, y, z;

    QTextStream in(this->objFile);
    while(!in.atEnd()) {
        in >> key >> x >> y >> z;
        emit lineRead(key, x, y, z);
    }

    objFile->close();
}

void MainWindow::on_actionLoad_obj_File_triggered()
{

}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                tr("Open OBJ File"), "/Users/steven/work", tr("OBJ Files (*.obj)")
    );
    qDebug() << "selecting " + fileName;

    this->readData(fileName);
}
