#ifndef QUAD_H
#define QUAD_H

#include <QSet>
#include <QVector>

#include "neighbor.h"
#include "vertex.h"

class Quad
{
public:
    QVector<int> vertexIndex;
    QSet<Neighbor*> neighbors;

    Vertex *faceVertex;

    Quad();
    Quad( float[4]);
    Quad(int i, int j, int k, int l);//int - vertex index
    void Print();
};

#endif // TRIANGLE_H
