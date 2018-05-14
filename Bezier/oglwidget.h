#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include "quad.h"
#include "vertex.h"
#include "triangle.h"
#include "neighbor.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QDebug>
#include <QVector>
#include <math.h>

#include "cgpoint.h"

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
    void cleanObjects();
    void updateFinished();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

protected:
    QTimer* animtimer; // Timer needed to step animation every x msec
    int animstep;      // Current animation step (used to rotate triangle

private:
    struct CgPoint;
};


#endif // OGLWIDGET_H
