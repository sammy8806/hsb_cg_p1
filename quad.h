#ifndef QUAD_H
#define QUAD_H
#include <QSet>
#include <QVector>

class Quad
{
public:
    QVector<int> vertexIndex;
    QSet<int> neighbors;

    Quad();
    Quad( float[4]);
    Quad(int i, int j, int k, int l);//int - vertex index
    void Print();
};

#endif // TRIANGLE_H
