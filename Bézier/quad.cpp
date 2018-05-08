#include "quad.h"

Quad::Quad()
{

}

Quad::Quad(int i, int j, int k, int l){
    vertexIndex.append(i);
    vertexIndex.append(j);
    vertexIndex.append(k);
    vertexIndex.append(l);
}

Quad::operator std::string() const
{
    return QString(QString("{v1=") + QString(this->vertexIndex[0]) +
            QString(", v2=") + QString(this->vertexIndex[1]) +
            QString(", v3=") + QString(this->vertexIndex[2]) +
            QString(", v4=") + QString(this->vertexIndex[3]) +
            QString(", val=") + QString(this->neighbors.size())).toStdString();
}
