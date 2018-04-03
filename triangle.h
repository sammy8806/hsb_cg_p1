#ifndef TRIANGLE_H
#define TRIANGLE_H


class Triangle
{
public:
    float vertexIndex[3];//vertex indices
    int neighbours[3];//adjacent triangles
    int edgeVertices [3];//edge vertex ind

    Triangle();
    Triangle( float[3]);
    Triangle( float i, float j, float k);//int - vertex index
    void Print();
};

#endif // TRIANGLE_H
