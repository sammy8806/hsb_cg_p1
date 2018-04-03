#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include "quad.h"
#include "vertex.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QDebug>
#include <QVector>

class OGLWidget : public QOpenGLWidget,
                  protected QOpenGLFunctions
{
    Q_OBJECT

public:
    OGLWidget(QWidget *parent = 0);
    ~OGLWidget();

public slots:
    void stepAnimation();
    void addVertex(float x, float y, float z);
    void addTriFace(float x, float y, float z);
    void addQuadFace(float x, float y, float z, float a);
//    void lineRead(QString key, float x, float y, float z);


protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void drawObject(QVector<Vertex> vertices, QVector<Quad> shape);

protected:
    QTimer* animtimer; // Timer needed to step animation every x msec
    int animstep;      // Current animation step (used to rotate triangle

private:
    QVector<Quad> quads;
    QVector<Vertex> vertices;

    void cross(float c[3], float const a[3], float const b[3]);
};


#endif // OGLWIDGET_H
