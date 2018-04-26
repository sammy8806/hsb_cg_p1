#ifndef VERTEX_H
#define VERTEX_H

#include <QSet>

class Vertex
{
public:
    int valence;
    float vertexCoord[3];

    QSet<int> edges;

    Vertex();
    Vertex(const Vertex &source);
    Vertex(float p1[3]);
    Vertex(float x,float y, float z);

    void SetValenceCount(int valence);

    Vertex operator+(const Vertex &b);
    Vertex operator*(const float a);
    Vertex operator/(const float b);
};

Vertex operator+=(Vertex a, Vertex b);
Vertex operator/=(Vertex a, float b);
float *operator-(Vertex a, Vertex b);
//Vertex operator%(Vertex a, Vertex b);//cross product

#endif // VERTEX_H
