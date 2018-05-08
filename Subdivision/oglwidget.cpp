#include "oglwidget.h"
#include <math.h>
#include <iostream>
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end

#define PI 3.14159265358979323846
using namespace std;

static double alpha = 35.0; // rotation angle
static double rotateY = 0;
static double rotateX = 0;
static double rotateZ = 0;

// initialize Open GL lighting and projection matrix
void InitLightingAndProjection() // to be executed once before drawing
{
    // light positions and colors
    GLfloat LightPosition1[4] = { 15, 10, 15,  0};
    GLfloat LightPosition2[4] = { -15, 10, -15,  0};
    GLfloat ColorRedish[4] = { 1.0,  .8,  .8,  1}; // white with a little bit of red
    GLfloat ColorBlueish[4] = { .8,  .8,  1.0,  1};// white with a little bit of blue

    glEnable( GL_DEPTH_TEST); // switch on z-buffer
    glDepthFunc( GL_LESS);

    glShadeModel( GL_SMOOTH); // Gouraud shading
    //glShadeModel( GL_FLAT);

    glEnable( GL_LIGHTING); // use lighting
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, 1); // draw both sides

    // define and switch on light 0
    glLightfv( GL_LIGHT0, GL_POSITION, LightPosition1);
    glLightfv( GL_LIGHT0, GL_DIFFUSE,  ColorRedish);
    glLightfv( GL_LIGHT0, GL_SPECULAR, ColorRedish);
    glEnable( GL_LIGHT0);

    // define and switch on light 1
    glLightfv( GL_LIGHT1, GL_POSITION, LightPosition2);
    glLightfv( GL_LIGHT1, GL_DIFFUSE,  ColorBlueish);
    glLightfv( GL_LIGHT1, GL_SPECULAR, ColorBlueish);
    glEnable( GL_LIGHT1);

    glMatrixMode( GL_PROJECTION); // define camera projection
    glLoadIdentity(); // reset matrix to identity (otherwise existing matrix will be multiplied with)
    glOrtho( -15, 15, -10, 10, -60, 60); // orthogonal projection (xmin xmax ymin ymax zmin zmax)
    //glFrustum( -10, 10, -8, 8, 2, 20); // perspective projektion
}

void DrawCylinder( int reso = 16){ // drawing a cylinder in OpenGL
    // alocate memory for x and y coordinates on a circle
    double *c = new double[ reso+1];
    double *s = new double[ reso+1];

    for( int i=0; i<=reso; i++){ // compute x and y coordinates of citcle
        c[i] = cos( 2.0 * PI * i / reso );
        s[i] = sin( 2.0 * PI * i / reso );
        //cout << i << " " << c[i] << endl;
    }

    glBegin( GL_QUADS); // each 4 points define a polygon
    for( int i=0; i<reso; i++){
        glNormal3f( c[i], s[i], 0.0); // normal vector used for all consecutive points
        glVertex3f( c[i], s[i], 3.0); // 2 points ...
        glVertex3f( c[i], s[i], 0.0);

        glNormal3f( c[i+1], s[i+1], 0.0); // another normal with two more points
        glVertex3f( c[i+1], s[i+1], 0.0);
        glVertex3f( c[i+1], s[i+1], 3.0);
    }
    glEnd(); // concludes GL_QUADS

    delete[] c; // de-allocate space
    delete[] s;
}

// define material color properties for front and back side
void SetMaterialColor( int side, float r, float g, float b){
    float	amb[4], dif[4], spe[4];
    int	i, mat;

    dif[0] = r; // diffuse color as defined by r,g, and b
    dif[1] = g;
    dif[2] = b;
    for( i=0; i<3; i++){
        amb[i] = .1 * dif[i]; // ambient color is 10 percent of diffuse
        spe[i] = .5; // specular color is just white / gray
    }
    amb[3] = dif[3] = spe[3] = 1.0; // alpha component is always 1
    switch( side){
        case 1:	mat = GL_FRONT; break;
        case 2:	mat = GL_BACK; break;
        default: mat = GL_FRONT_AND_BACK; break;
    }
    glMaterialfv( mat, GL_AMBIENT, amb); // define ambient, diffuse and specular components
    glMaterialfv( mat, GL_DIFFUSE, dif);
    glMaterialfv( mat, GL_SPECULAR, spe);
    glMaterialf( mat, GL_SHININESS, 50.0); // Phong constant for the size of highlights
}

OGLWidget::OGLWidget(QWidget *parent) // constructor
    : QOpenGLWidget(parent)
{
    // Setup the animation timer to fire every x msec
    animtimer = new QTimer(this);
    animtimer->start( 50 );

    // Everytime the timer fires, the animation is going one step forward
    connect(animtimer, SIGNAL(timeout()), this, SLOT(stepAnimation()));

    animstep = 0;
}

OGLWidget::~OGLWidget() // destructor
{
}

void OGLWidget::stepAnimation()
{
    animstep++;    // Increase animation steps
    update();      // Trigger redraw of scene with paintGL
}

void OGLWidget::cleanObjects() {
    this->vertices.clear();
    this->tries.clear();
    this->quads.clear();
}

void OGLWidget::updateFinished()
{
    qDebug() << "Update finished !";
    this->CalculateValences();
    this->CalculateAllNeighbors();
    this->CalculateAllFacesMidpoints();
    this->CalculateAllEdgesMidpoints();
    this->CalculateAllAlternativeVertices();
}

void OGLWidget::addTriFace(int a, int b, int c) {
        this->tries.push_back(new Triangle(--a, --b, --c));
}

void OGLWidget::addQuadFace(int a, int b, int c, int d) {
    this->quads.push_back(new Quad(--a, --b, --c, --d));
}

void OGLWidget::addVertex(float x, float y, float z) {
    this->vertices.push_back(new Vertex(x, y, z));
}

void OGLWidget::addVertex(Vertex *vertex) {
    this->vertices.push_back(vertex);
}

void OGLWidget::initializeGL() // initializations to be called once
{
    initializeOpenGLFunctions();
    InitLightingAndProjection(); // define light sources and projection
}

void OGLWidget::paintGL() // draw everything, to be called repeatedly
{
    glEnable(GL_NORMALIZE); // this is necessary when using glScale (keep normals to unit length)

    // set background color
    glClearColor(0.8, 0.8, 1.0, 1.0); // bright blue
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw the scene
    glMatrixMode( GL_MODELVIEW);
    glLoadIdentity();				// Reset The Current Modelview Matrix
    glTranslated( 0 ,0 ,-10.0);     // Move 10 units backwards in z, since camera is at origin
    glScaled( 5.0, 5.0, 5.0);       // scale objects
    glRotated( alpha, rotateX, rotateY, rotateZ);     // continuous rotation

    alpha += 5;
    rotateX = 9;
    rotateY = 2;
    rotateZ = 5;

    // define color: 1=front, 2=back, 3=both, followed by r, g, and b
    SetMaterialColor( 1, 1.0, .2, .2);  // front color is red
    SetMaterialColor( 2, 0.2, 0.2, 1.0); // back color is blue

    // draw a cylinder with default resolution
    // DrawCylinder();
    if(this->quads.length() > 0)
        drawObject(this->vertices, this->quads, 4);
    else if(this->tries.length() > 0)
        drawObject(this->vertices, this->tries, 3);

    // make it appear (before this, it's hidden in the rear buffer)
    glFlush();
}

void OGLWidget::cross(float c[3], const float a[], const float b[])
{
    c[0] = a[1]*b[2]-a[2]*b[1];
    c[1] = a[2]*b[0]-a[0]*b[2];
    c[2] = a[0]*b[1]-a[1]*b[0];
}

void OGLWidget::resizeGL(int w, int h) // called when window size is changed
{
    // adjust viewport transform
    glViewport(0,0,w,h);
}

void OGLWidget::CalculateValences(){
    for(int i = 0; i < this->quads.length(); i++){
        for(int j = 0; j < 4; j++) {

            QSet<int> vertexEdges;
            int vertex = this->quads[i]->vertexIndex[j];

            for(int faceId = 0; faceId < this->quads.length(); faceId++) {
                for(int vertexId = 0; vertexId < 4; vertexId++) {
                    if( this->quads.at(faceId)->vertexIndex[vertexId] == vertex ) {
                        int left, right;

                        if(vertexId - 1 < 0)
                            left = this->quads.at(faceId)->vertexIndex[3];
                        else
                            left = this->quads.at(faceId)->vertexIndex[vertexId - 1];

                        if(vertexId + 1 > 3)
                            right = this->quads.at(faceId)->vertexIndex[0];
                        else
                            right = this->quads.at(faceId)->vertexIndex[vertexId + 1];

                        vertexEdges.insert(left);
                        vertexEdges.insert(right);

                        this->vertices.at(vertex)->edges.insert(left);
                        this->vertices.at(vertex)->edges.insert(right);
                    }
                }
            }

            this->vertices.at(vertex)->SetValenceCount(vertexEdges.size());
            qDebug() << "Vertex" << j << "of face" << i
                     << "has" << vertexEdges.size() << "edges";
        }
    }
// qDebug() << "Overall" << this->edges.size() << "edges detected";
}

void OGLWidget::CalculateAllNeighbors(){

    for(int face = 0; face < this->quads.length(); face++) {

        int sameVertexCount = 0;
        QSet<int> foundFaces;
        for(int vertexId = 0; vertexId < 4; vertexId++){

            for(int comparedFace = 0; comparedFace < this->quads.length(); comparedFace++){
                if(face == comparedFace) {
                    continue;
                }

                for(int comparedVertex = 0; comparedVertex < 4; comparedVertex++){

                    if(this->quads.at(face)->vertexIndex[vertexId] == this->quads.at(comparedFace)->vertexIndex[comparedVertex]) {
                        sameVertexCount++;
                        foundFaces.insert(comparedFace);

                        qDebug() << "Found same Vertecies at " << face << ":" << vertexId
                                 << " and " << comparedFace << ":" << comparedVertex;
                    }
                }
                if(sameVertexCount == 2) {
                    sameVertexCount = 0;
                    qDebug() << "Found neighbor " << comparedFace << " of " << face;
                }
            }

            sameVertexCount = 0;
        }
        // this->quads.at(face)->neighbors = foundFaces;
        qDebug() << "Found xxx matching vertices";
    }

    for(int face = 0; face < this->quads.length(); face++) {
        for(int vertexId = 0; vertexId < 4; vertexId++) {
            int v1 = this->quads.at(face)->vertexIndex.at(vertexId);
            int v2 = this->quads.at(face)->vertexIndex.at( (vertexId + 1) >= 4 ? 0 : (vertexId + 1) );

            std::pair<int, int> edgeName;
            if(v1 < v2) {
                edgeName = std::make_pair(v1, v2);
            } else {
                edgeName = std::make_pair(v2, v1);
            }

            QVector<int> *oldEdge = this->edges[edgeName];
            if(oldEdge == nullptr) {
                this->edges.insert(edgeName, new QVector<int>{face});
            } else if(oldEdge->length() > 0) {
                oldEdge->append(face);
            }
        }
    }

    qDebug() << "Found" << this->edges.size() << "edges";

}

void OGLWidget::CalculateAllFacesMidpoints(){

    for(int face = 0; face < this->quads.length(); face++) {
        Vertex *vertex = new Vertex(0, 0, 0);

<<<<<<< HEAD:Subdivision/oglwidget.cpp
        float faceMidpoint[3] = {0,0,0};
=======
>>>>>>> 4d0ccddd9120297c93342cd8321bbec9c18395ae:oglwidget.cpp
        for(int vertexId = 0; vertexId < 4; vertexId++){
            *vertex = *vertex + *vertices[ this->quads.at(face)->vertexIndex[vertexId] ] / 4.0f;
        }

        this->quads.at(face)->faceVertex = vertex;
    }

    qDebug() << "Now" << vertices.size() << "vertices overall";
}


void OGLWidget::CalculateAllEdgesMidpoints(){

    for(auto edge : edges.toStdMap()) {
        int e1 = edge.first.first;
        int e2 = edge.first.second;

        int f1 = edge.second->at(0);
        int f2 = edge.second->at(1);

        qDebug() << "e1 e2 f1 f2" << e1 << e2 << f1 << f2;

        Vertex e_v1 = *this->vertices.at(e1);
        Vertex e_v2 = *this->vertices.at(e2);

        Vertex f_v1 = *this->quads.at(f1)->faceVertex;
        Vertex f_v2 = *this->quads.at(f2)->faceVertex;

        // e = ( (e_v1 + e_v2)/2 + (f_v1 + f_v2)/2 )/2;
        Vertex v = (e_v1 + e_v2) / 2;
        Vertex f = (f_v1 + f_v2) / 2;
        Vertex result = ( v + f )/2;

        this->vertices.append(new Vertex(result));
        qDebug() << "#midpoint done";
    }

    qDebug() << "#all midpoints done";

}

void OGLWidget::CalculateAllAlternativeVertices(){

    // for vertice (all edgepoints ; all facepoints ; old vertice ; valence(3) )
    QVector<Vertex*> realVertices;
    QVector<Vertex*> edgepoints;

    for(int vertex = 0; vertex < this->vertices.size(); vertex++) {
        if(this->vertices.at(vertex)->valence <= 0) {       // Edge and face midpoint vertices don't have a valence yet
            edgepoints.append(this->vertices.at(vertex));   // so we can differentiate between midpoint vertices
        } else {
            realVertices.append(this->vertices.at(vertex)); // and original vertices
        }
    }

    QVector<Vertex> alternativeVertices;
    // Loop for calculating the alternative vertices
    for(Vertex* vertex : realVertices) {
        QVector<Vertex*> facepoints;

        Vertex edge;
        Vertex face;

        QList<std::pair<int, int>> quadVertices;
        QSet<Quad*> quads;

        //qset<int>     // calculating the average of the edge midpoint vertices
        for(int vertex_id : vertex->edges) {
            edge = edge + *this->vertices.at(vertex_id);

            quadVertices.append(std::make_pair(this->vertices.indexOf(vertex), vertex_id));
        }
        edge = edge / vertex->edges.size();

        // Getting the right face midpoints for cc subdivision (?)
        for(int i = 0; i<quadVertices.size(); i++) {
            QVector<int>* quadList = this->edges.value(quadVertices.at(i));
            if(quadList == nullptr) {
                continue;
            }

            for(auto e : quadList->toStdVector() ) {
                quads.insert( this->quads.at(e) );
                facepoints.append(this->quads.at(e)->faceVertex);
            }
        }

        // calculating the average of the face midpoint vertices
        for(Vertex* facepoint : facepoints) {
            face = face + *facepoint;
        }
        face = face / facepoints.size();

        // Calculation of the new vertex
        Vertex newVertex;
        for(int coord = 0; coord < 3; coord++){
            newVertex.vertexCoord[coord] = 1 / vertex->valence * (4 * edge.vertexCoord[coord] - face.vertexCoord[coord] + (vertex->valence - 3) * vertex->vertexCoord[coord]);
        }
        alternativeVertices.append(newVertex);

        qDebug() << "real vertex dome";
    }

    qDebug() << "calc all alternatives done";
}
