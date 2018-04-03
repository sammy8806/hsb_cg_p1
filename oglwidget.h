#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include "quad.h"
#include "vertex.h"
#include "triangle.h"

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
    void addTriFace(int a, int b, int c);
    void addQuadFace(int a, int b, int c, int d);
    void addVertex(float x, float y, float z);
    void cleanObjects();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    template<typename T>
    void drawObject(QVector<Vertex> vertices, QVector<T> shape, int elements)
    {
        if(elements == 3)
            glBegin(GL_TRIANGLES);

        if(elements == 4)
            glBegin(GL_QUADS);

        float normal[3];

        for(int i=0; i<shape.length(); i++) {
            this->cross(normal,
                        vertices.at(shape[i].vertexIndex[0]).vertexCoord,
                        vertices.at(shape[i].vertexIndex[1]).vertexCoord
            );
            glNormal3fv(normal);

            for(int j=0; j<elements; j++) {
                glVertex3fv(vertices.at(shape[i].vertexIndex[j]).vertexCoord);
            }
        }

        glEnd();
    }


protected:
    QTimer* animtimer; // Timer needed to step animation every x msec
    int animstep;      // Current animation step (used to rotate triangle

private:
    QVector<Quad> quads;
    QVector<Triangle> tries;
    QVector<Vertex> vertices;

    void cross(float c[3], float const a[3], float const b[3]);
};


#endif // OGLWIDGET_H
