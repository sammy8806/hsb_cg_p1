#ifndef QUAD_H
#define QUAD_H
#include <qvector.h>

class Quad
{
public:
    int vertexIndex[4];
    QVector<int> neighbors;

    Quad();
    Quad( float[4]);
    Quad( float i, float j, float k, float l);//int - vertex index
    void Print();
};

#endif // TRIANGLE_H
