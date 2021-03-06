#include "oglwidget.h"
#include <math.h>
#include <iostream>
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include"mainwindow.h"

#define PI 3.14159265358979323846
using namespace std;

static double alpha = 45.0;

int maxJ = 12;


// 4 control points for our cubic bezier curve
float PointsOld[4][3] = {
    { 10,10,0 },
    {  5,10,2 },
    { -5,0,0 },
    {-10,5,-2}
};

CgPoint Points[4][4] = {
    {
        { 10,0,10 },
        {  5,0,10 },
        { -5,0,10 },
        {-10,0,10 }
    },
    {
        { 10,0,5 },
        {  5,6,5 },
        { -5,6,5 },
        {-10,0,5 }
    },
    {
        { 10,0,-5 },
        {  5,6,-5 },
        { -5,6,-5 },
        {-10,0,-5 }
    },
    {
        { 10,0,-10 },
        {  5,0,-10 },
        { -5,0,-10 },
        {-10,0,-10 }
    }

};





// the level of detail of the curve
unsigned int LOD=1000;


GLfloat ctrlpoints[4][4][3] = {
   {{-1.5, -1.5, 4.0}, {-0.5, -1.5, 2.0},
    {0.5, -1.5, -1.0}, {1.5, -1.5, 2.0}},
   {{-1.5, -0.5, 1.0}, {-0.5, -0.5, 3.0},
    {0.5, -0.5, 0.0}, {1.5, -0.5, -1.0}},
   {{-1.5, 0.5, 4.0}, {-0.5, 0.5, 0.0},
    {0.5, 0.5, 3.0}, {1.5, 0.5, 4.0}},
   {{-1.5, 1.5, -2.0}, {-0.5, 1.5, -2.0},
    {0.5, 1.5, 0.0}, {1.5, 1.5, -1.0}}
};

void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
           0, 1, 12, 4, &ctrlpoints[0][0][0]);
   glEnable(GL_MAP2_VERTEX_3);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_FLAT);
}

void display(void)
{
    int i, j;
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix ();
   glRotatef(15.0, 1.0, 1.0, 1.0);
   for (j = 0; j <= maxJ; j++) {
      glColor3f(1.0, 0.0, 0.0);
      glBegin(GL_LINE_STRIP);
      for (i = 0; i <= 30; i++)
         glEvalCoord2f((GLfloat)i/30.0, (GLfloat)j/8.0);
      glEnd();
      glColor3f(0.0, 0.0, 1.0);
      glBegin(GL_LINE_STRIP);
      for (i = 0; i <= 30; i++)
         glEvalCoord2f((GLfloat)j/8.0, (GLfloat)i/30.0);
      glEnd();
   }
   glPopMatrix ();
   glFlush();
}

//------------------------------------------------------------	CalculateU()
// Given our 16 control points, we can imagine 4 curves travelling
// in a given surface direction. For example, if the parametric u
// value was 0.5, we could imagine evaluating 4 seperate curves
// at u=0.5.
//
// This function is basically here to perform that very task. each
// row of 4 points in the u-direction is evaluated to provide us
// with 4 new points. These new points then form a curve we can
// evaluate in the v direction to calculate our final output point.
//
CgPoint CalculateU(float t,int row) {

    // the final point
    CgPoint p;

    // the t value inverted
    float it = 1.0f-t;

    // calculate blending functions
    float b0 = t*t*t;
    float b1 = 3*t*t*it;
    float b2 = 3*t*it*it;
    float b3 =  it*it*it;

    // sum the effects of the Points and their respective blending functions
    p.x = b0*Points[row][0].x +
          b1*Points[row][1].x +
          b2*Points[row][2].x +
          b3*Points[row][3].x ;

    p.y = b0*Points[row][0].y +
          b1*Points[row][1].y +
          b2*Points[row][2].y +
          b3*Points[row][3].y ;

    p.z = b0*Points[row][0].z +
          b1*Points[row][1].z +
          b2*Points[row][2].z +
          b3*Points[row][3].z ;

    return p;
}

//------------------------------------------------------------	CalculateV()
// Having generated 4 points in the u direction, we need to
// use those points to generate the final point on the surface
// by calculating a final bezier curve in the v direction.
//     This function takes the temporary points and generates
// the final point for the rendered surface
//
CgPoint CalculateV(float t, CgPoint* pnts) {
    CgPoint p;

    // the t value inverted
    float it = 1.0f-t;

    // calculate blending functions
    float b0 = t*t*t;
    float b1 = 3*t*t*it;
    float b2 = 3*t*it*it;
    float b3 =  it*it*it;

    // sum the effects of the Points and their respective blending functions
    p.x = b0*pnts[0].x +
          b1*pnts[1].x +
          b2*pnts[2].x +
          b3*pnts[3].x ;

    p.y = b0*pnts[0].y +
          b1*pnts[1].y +
          b2*pnts[2].y +
          b3*pnts[3].y ;

    p.z = b0*pnts[0].z +
          b1*pnts[1].z +
          b2*pnts[2].z +
          b3*pnts[3].z ;

    return p;
}

//------------------------------------------------------------	Calculate()
// On our bezier patch, we need to calculate a set of points
// from the u and v parametric range of 0,0 to 1,1. This calculate
// function performs that evaluation by using the specified u
// and v parametric coordinates.
//
CgPoint Calculate(float u,float v) {

    // first of all we will need to evaluate 4 curves in the u
    // direction. The points from those will be stored in this
    // temporary array
    CgPoint temp[4];

    // calculate each point on our final v curve
    temp[0] = CalculateU(u,0);
    temp[1] = CalculateU(u,1);
    temp[2] = CalculateU(u,2);
    temp[3] = CalculateU(u,3);

    // having got 4 points, we can use it as a bezier curve
    // to calculate the v direction. This should give us our
    // final point
    //
    return CalculateV(v,temp);
}


void OnDraw() {

    // clear the screen & depth buffer
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

    // clear the previous transform
    glLoadIdentity();

    // set the camera position
    /* gluLookAt(	5,16,20,	//	eye pos
                0,0,0,	//	aim point
                0,1,0);	//	up direction */

    glColor3f(1,0,1);
    glPointSize(2);
    glBegin(GL_POINTS);

    // use the parametric time value 0 to 1
    for(int i=0;i!=LOD;++i) {

        // calculate the parametric u value
        float u = (float)i/(LOD-1);

        for(int j=0;j!=LOD;++j) {

            // calculate the parametric v value
            float v = (float)j/(LOD-1);

            // calculate the point on the surface
            CgPoint p = Calculate(u,v);

            // draw point
            glVertex3f(p.x,p.y,p.z);
        }
    }

    glEnd();
}

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
    // glFrustum( -10, 10, -8, 8, 2, 20); // perspective projektion

    // init();
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

}

void OGLWidget::updateFinished()
{
    qDebug() << "Update finished !";
}

void OGLWidget::initializeGL() // initializations to be called once
{
    initializeOpenGLFunctions();
    InitLightingAndProjection(); // define light sources and projection
}

void stuff() {
    GLfloat grid2x2[2][2][3] = {
      {{-2.0, -2.0, 0.0}, {4.0, -2.0, 0.0}},
      {{-2.0, 3.0, 0.0}, {3.0, 4.0, 0.0}}
    };

    glEnable(GL_MAP2_VERTEX_3);
     glMap2f(GL_MAP2_VERTEX_3,
       0.0, 1.0,  /* U ranges 0..1 */
       3,         /* U stride, 3 floats per coord */
       2,         /* U is 2nd order, ie. linear */
       0.0, 1.0,  /* V ranges 0..1 */
       2 * 3,     /* V stride, row is 2 coords, 3 floats per coord */
       2,         /* V is 2nd order, ie linear */
       &(grid2x2[0][0][0]));  /* control points */

     glMapGrid2f(
       5, 0.0, 1.0,
       6, 0.0, 1.0);

     glEvalMesh2(GL_LINE,
       0, 5,   /* Starting at 0 mesh 5 steps (rows). */
       0, 6);  /* Starting at 0 mesh 6 steps (columns). */

     glEnable(GL_FILL);
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
    glTranslated(0, 0 ,-10.0);     // Move 10 units backwards in z, since camera is at origin
    glScaled( 2.0, 2.0, 2.0);       // scale objects
    glRotated(alpha, 0, 3, 1);     // continuous rotation
    alpha += 5;

    // define color: 1=front, 2=back, 3=both, followed by r, g, and b
    SetMaterialColor( 1, 1.0, .2, .2);  // front color is red
    SetMaterialColor( 2, 0.2, 0.2, 1.0); // back color is blue

    glEnable(GL_AUTO_NORMAL);

    // display();
    OnDraw();

    // make it appear (before this, it's hidden in the rear buffer)
    glFlush();
}



void OGLWidget::resizeGL(int w, int h) // called when window size is changed
{
    // adjust viewport transform
    glViewport(0,0,w,h);
}


