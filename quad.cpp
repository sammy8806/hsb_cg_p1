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
