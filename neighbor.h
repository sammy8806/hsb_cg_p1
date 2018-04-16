#ifndef NEIGHBOR_H
#define NEIGHBOR_H

#include <QSet>

class Neighbor
{
public:
    Neighbor();
    Neighbor(int, int &);

    int neighborIndex;
    int edgesVertices[2];

};

#endif // NEIGHBOR_H
