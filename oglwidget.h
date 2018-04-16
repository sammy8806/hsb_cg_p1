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
#include <math.h>

class OGLWidget : public QOpenGLWidget,
                  protected QOpenGLFunctions
{
    Q_OBJECT

public:
    OGLWidget(QWidget *parent = 0);
    ~OGLWidget();

public slots:
    void stepAnimation();

public:
    void addTriFace(int a, int b, int c);
    void addQuadFace(int a, int b, int c, int d);
    void addVertex(float x, float y, float z);
    void cleanObjects();
    void updateFinished();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void normalize(float v[3]) {
        float d = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
        v[0] /= d;
        v[1] /= d;
        v[2] /= d;
    }

    template<typename T>
    void drawObject(QVector<Vertex*> vertices, QVector<T*> shape, int elements)
    {
        if(elements == 3)
            glBegin(GL_TRIANGLES);

        if(elements == 4)
            glBegin(GL_QUADS);

        float normal[3];

        for(int i=0; i<shape.length(); i++) {
            if(elements == 3) {
                this->cross(normal,
                            vertices.at(shape[i]->vertexIndex[0])->vertexCoord,
                            vertices.at(shape[i]->vertexIndex[1])->vertexCoord
                );
            }

            if(elements == 4) {
                float d1[3], d2[3];
                for (int k = 0; k < 3; k++) {
                    // p1 - p0
                    d1[k] = vertices.at(shape[i]->vertexIndex[1])->vertexCoord [k] - vertices.at(shape[i]->vertexIndex[0])->vertexCoord [k] ;
                    // p2 - p0
                    d2[k] = vertices.at(shape[i]->vertexIndex[2])->vertexCoord [k] - vertices.at(shape[i]->vertexIndex[0])->vertexCoord [k] ;
                }

                this->cross(normal, d1, d2);
                this->normalize(normal);
            }

            glNormal3fv(normal);

            for(int j=0; j<elements; j++) {
                glVertex3fv(vertices.at(shape[i]->vertexIndex[j])->vertexCoord);
            }
        }

        glEnd();
    }


protected:
    QTimer* animtimer; // Timer needed to step animation every x msec
    int animstep;      // Current animation step (used to rotate triangle

private:
    QVector<Quad*> quads;
    QVector<Triangle*> tries;
    QVector<Vertex*> vertices;
    QVector<int> edges;

    void cross(float c[3], float const a[3], float const b[3]);
    void CalculateValences();
    void CalculateAllNeighbors();
    void CalculateAllFacesMidpoints();
    void CalculateAllEdgesMidpoints();
};


#endif // OGLWIDGET_H
