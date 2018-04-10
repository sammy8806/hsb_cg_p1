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
    Vertex( float p1[3]);
    Vertex(float x,float y, float z);
    void Print(); // print coordinates on screen
    void SetValenceCount(int valence);
};


Vertex operator+(Vertex a, Vertex b);
Vertex operator-(Vertex a, Vertex b);
Vertex operator*(float a, Vertex b);
Vertex operator/(Vertex a, float b);
Vertex operator%(Vertex a, Vertex b);//cross product

#endif // VERTEX_H
