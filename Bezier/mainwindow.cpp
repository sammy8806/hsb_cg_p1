#include <assert.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

    QTextStream in(this->objFile);
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(QRegExp("\\s+"));

        if(parts.length() == 0)
            continue;

        if(parts.at(0) == "v") {
                // ui->glwidget->addVertex(parts.at(1).toFloat(), parts.at(2).toFloat(), parts.at(3).toFloat());
        } else if(parts.at(0) == "f" && parts.length() == 4) {
            // Add trieforc^W triface
                // ui->glwidget->addTriFace(parts.at(1).toFloat(), parts.at(2).toFloat(), parts.at(3).toFloat());
        } else if(parts.at(0) == "f" && parts.length() == 5) {
                // ui->glwidget->addQuadFace(parts.at(1).toFloat(), parts.at(2).toFloat(), parts.at(3).toFloat(), parts.at(4).toFloat());
        } else {
            qDebug() << "Unexpected entry type " + parts.at(0);
            assert("Unexpected entry type");
        }

    }

    objFile->close();

    ui->glwidget->updateFinished();
}

void MainWindow::on_actionLoad_obj_File_triggered()
{

}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                tr("Open OBJ File"), "/Users/steven/Nextcloud/hs-bremen/ismi/cg", tr("OBJ Files (*.obj)")
    );
    qDebug() << "selecting " + fileName;

    this->readData(fileName);
}
