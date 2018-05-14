#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "oglwidget.h"

using namespace std;

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

 void MainWindow::writeData(){

    QFile file;
    file.setFileName("example.json");
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream out(&file);

        out<<"eidechsen schlafen auf dem ruecken"<<endl;
        out<<"Leguane auch"<< endl;

     file.flush();
     file.close();

}

void MainWindow::readData(QString filename)
{

    QString fullFilename = filename;
    QFile file;
    file.setFileName("example.json");

    qDebug() << "Opening: " + fullFilename;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << "error opening file" << std::endl;
        return;
    }

    QTextStream in(&file);
    QString firstChar = in.read(1);
    qDebug() << "Found first char:" << firstChar;

    if(firstChar == "{") {
        qDebug() << "Processing as Json";
        in.seek(0);
        QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
        QJsonObject curvesObject = jsonDoc.object();
        QJsonArray slice = curvesObject.value(QString("curves")).toArray();
        qDebug() << "Found" << slice.size() << "slices";

        for(QJsonValue sliceVal : slice) {
            QJsonArray sliceArr = sliceVal.toArray();
            qDebug() << "Data:" << sliceArr;

            for(QJsonValue pointVal : sliceArr) {
                QJsonObject pointObj = pointVal.toObject();
                qDebug() << "Data:" << pointObj;

                struct CgPoint point = {
                    static_cast<float>(pointObj["x"].toDouble()),
                    static_cast<float>(pointObj["y"].toDouble()),
                    static_cast<float>(pointObj["z"].toDouble())
                };

                qDebug() << "Point: x" << point.x << "y:" << point.y << "z:" << point.z;
                // ...
            }

        }

    } else {
        qDebug() << "Processing as Obj";
        in.seek(0);

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
    }

    file.close();

    ui->glwidget->updateFinished();
}

void MainWindow::on_actionLoad_obj_File_triggered()
{

}

void MainWindow::on_pushButton_clicked()
{
    //writeData();

    QString fileName = QFileDialog::getOpenFileName(this,
                tr("Open OBJ File"), "/Users/steven/Nextcloud/hs-bremen/ismi/cg", tr("OBJ Files (example.json)")
    );
    qDebug() << "selecting " + fileName;

    this->readData(fileName);

}
