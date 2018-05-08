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

    void Print(); // print coordinates on screen
    void SetValenceCount(int valence);

    Vertex operator+(Vertex &b);
    Vertex operator*(float a);
    Vertex operator/(float b);
};

Vertex operator+=(Vertex a, Vertex b);
Vertex operator/=(Vertex a, float b);

// Vertex operator+(Vertex a, Vertex b);
//Vertex operator-(Vertex a, Vertex b);
//Vertex operator*(float a, Vertex b);
//Vertex operator/(Vertex a, float b);
//Vertex operator%(Vertex a, Vertex b);//cross product

#endif // VERTEX_H
