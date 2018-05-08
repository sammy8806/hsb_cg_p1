#include "vertex.h"

Vertex::Vertex()
{
    valence = 0;
    for(int i=0; i<3; i++) {
        this->vertexCoord[i] = 0;
    }
}

Vertex::Vertex(const Vertex &source)
{
    this->edges = source.edges;
    this->valence = source.valence;

    for(int i=0; i<3; i++) {
        this->vertexCoord[i] = source.vertexCoord[i];
    }
}

Vertex::Vertex(float x, float y, float z){
    vertexCoord[0]=x;
    vertexCoord[1]=y;
    vertexCoord[2]=z;
}

void Vertex::SetValenceCount(int valence){
    this->valence = valence;
}

Vertex Vertex::operator+(Vertex &b)
{
    Vertex result = Vertex();
    for(int i=0; i < 3; i++) {
        result.vertexCoord[i] = this->vertexCoord[i] + b.vertexCoord[i];
    }
    return result;
}

Vertex Vertex::operator/(float b)
{
    Vertex result = Vertex();
    for(int i=0; i < 3; i++) {
        result.vertexCoord[i] = this->vertexCoord[i] / b;
    }
    return result;
}

Vertex Vertex::operator*(float a)
{
    Vertex result = Vertex();
    for(int i=0; i < 3; i++) {
        result.vertexCoord[i] = a * this->vertexCoord[i];
    }
    return result;
}

Vertex operator+=(Vertex a, Vertex b)
{
    return a + b;
}

Vertex operator/=(Vertex a, float b)
{
    return a / b;
}
