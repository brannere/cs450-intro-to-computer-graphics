#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#include "glew.h"
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include "bmptotexture.cpp"
#include "planet.h"

const int SCROLL_WHEEL_UP   = { 3 };
const int SCROLL_WHEEL_DOWN = { 4 };
const double RADIUS_SCALER = 500;
const int SUN_DIAM = 190000;
const int SUN_DIAM_DENOM = 15000;
const double ORBIT_SCALER = 0.01;
float POS = 0;
// equivalent mouse movement when we click a the scroll wheel:

const float SCROLL_WHEEL_CLICK_FACTOR = { 5. };

const int MS_PER_CYCLE = 70000; 
float Time;
GLuint Tex0, Tex1;
GLuint 	tex_mercury,tex_venus,tex_earth,tex_mars,tex_jupiter,
				tex_saturn, tex_uranus, tex_neptune;

bool texture;


float*
Array3( float a, float b, float c )
{
	static float array[4];
	array[0] = a;
	array[1] = b;
	array[2] = c;
	array[3] = 1.;
	return array;
}


void
SetSpotLight( int ilight, float x, float y, float z, float xdir, float ydir, float zdir, float r, float g, float b )
{
	glLightfv( ilight, GL_POSITION, Array3( x, y, z ) );
	glLightfv( ilight, GL_SPOT_DIRECTION, Array3(xdir,ydir,zdir) );
	glLightf( ilight, GL_SPOT_EXPONENT, 1. );
	glLightf( ilight, GL_SPOT_CUTOFF, 45. );
	glLightfv( ilight, GL_AMBIENT, Array3( 0., 0., 0. ) );
	glLightfv( ilight, GL_DIFFUSE, Array3( r, g, b ) );
	glLightfv( ilight, GL_SPECULAR, Array3( r, g, b ) );
	// glLightf ( ilight, GL_CONSTANT_ATTENUATION, 1. );
	// glLightf ( ilight, GL_LINEAR_ATTENUATION, 0. );
	// glLightf ( ilight, GL_QUADRATIC_ATTENUATION, 0. );
	glEnable( ilight );
}

// Planet mercury("mercury",	4879,		1407.6,		4222.6,	57.9,		88		,0.01);
// Planet venus	("venus",		12104,	-58325.5,	2802,		108.2,	224.7	,0.01);
// Planet earth	("earth",		12756,	23.9,			24,			149.6,	365.2	,0.01);
// Planet mars		("mars",		6792,		24.6,			24.7,		227.9,	687		,0.01);
// Planet jupiter("jupiter",	142984,	9.9,			9.9,		778.6,	4331	,0.01);
// Planet saturn	("saturn",	120536,	10.7,			10.7,		1433.5,	10747	,0.01);
// Planet uranus	("uranus",	51118,	-17.2,		17.2,		2872.5,	30589	,0.01);
// Planet neptune("neptune",	49528,	16.1,			16.1,		4495.1,	59800	,0.01);

Planet mercury("mercury",	4879,		1407.6,		4222.6,	57.9,		59800	,0.01);
Planet venus	("venus",		12104,	-58325.5,	2802,		108.2,	30589	,0.01);
Planet earth	("earth",		12756,	23.9,			24,			149.6,	10747	,0.01);
Planet mars		("mars",		6792,		24.6,			24.7,		227.9,	4331	,0.01);
Planet jupiter("jupiter",	142984,	9.9,			9.9,		778.6,	687		,0.01);
Planet saturn	("saturn",	120536,	10.7,			10.7,		1433.5,	365.2	,0.01);
Planet uranus	("uranus",	51118,	-17.2,		17.2,		2872.5,	224.7	,0.01);
Planet neptune("neptune",	49528,	16.1,			16.1,		4495.1,	88		,0.01);

/* This isn't the the real diameter of the sun... */
Planet sun		("sun",			SUN_DIAM,	0,			0, 			0, 			0			,0.01);


//	This is a sample OpenGL / GLUT program
//
//	The objective is to draw a 3d object and change the color of the axes
//		with a glut menu
//
//	The left mouse button does rotation
//	The middle mouse button does scaling
//	The user interface allows:
//		1. The axes to be turned on and off
//		2. The color of the axes to be changed
//		3. Debugging to be turned on and off
//		4. Depth cueing to be turned on and off
//		5. The projection to be changed
//		6. The transformations to be reset
//		7. The program to quit
//
//	Author:			Erick Branner

// NOTE: There are a lot of good reasons to use const variables instead
// of #define's.  However, Visual C++ does not allow a const variable
// to be used as an array size or as the case in a switch( ) statement.  So in
// the following, all constants are const variables except those which need to
// be array sizes or cases in switch( ) statements.  Those are #defines.


// title of these windows:

const char *WINDOWTITLE = { "OpenGL / Final Project; Solar System -- Erick Branner" };
const char *GLUITITLE   = { "User Interface Window" };


// what the glui package defines as true and false:

const int GLUITRUE  = { true  };
const int GLUIFALSE = { false };


// the escape key:

#define ESCAPE		0x1b


// initial window size:

const int INIT_WINDOW_SIZE = { 600 };


// size of the box:

const float BOXSIZE = { 2.f };



// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };


// minimum allowable scale factor:

const float MINSCALE = { 0.05f };


// active mouse buttons (or them together):

const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };


// which projection:

enum Projections
{
	ORTHO,
	PERSP
};


// which button:

enum ButtonVals
{
	RESET,
	QUIT
};


// window background color (rgba):

const GLfloat BACKCOLOR[ ] = { 0., 0., 0., 1. };


// line width for the axes:

const GLfloat AXES_WIDTH   = { 3. };


// the color numbers:
// this order must match the radio button order

enum Colors
{
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA,
	WHITE,
	BLACK
};

char * ColorNames[ ] =
{
	(char *)"Red",
	(char *)"Yellow",
	(char *)"Green",
	(char *)"Cyan",
	(char *)"Blue",
	(char *)"Magenta",
	(char *)"White",
	(char *)"Black"
};


// the color definitions:
// this order must match the menu order

const GLfloat Colors[ ][3] = 
{
	{ 1., 0., 0. },		// red
	{ 1., 1., 0. },		// yellow
	{ 0., 1., 0. },		// green
	{ 0., 1., 1. },		// cyan
	{ 0., 0., 1. },		// blue
	{ 1., 0., 1. },		// magenta
	{ 1., 1., 1. },		// white
	{ 0., 0., 0. },		// black
};


// fog parameters:

const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE     = { GL_LINEAR };
const GLfloat FOGDENSITY  = { 0.30f };
const GLfloat FOGSTART    = { 1.5 };
const GLfloat FOGEND      = { 4. };


// non-constant global variables:

int		ActiveButton;			// current button that is down
GLuint	AxesList;				// list to hold the axes
int		AxesOn;					// != 0 means to draw the axes
int		DebugOn;				// != 0 means to print debugging info
int		DepthCueOn;				// != 0 means to use intensity depth cueing
int		DepthBufferOn;			// != 0 means to use the z-buffer
int		DepthFightingOn;		// != 0 means to use the z-buffer
GLuint	BoxList;				// object display list
int		MainWindow;				// window id for main graphics window
float	Scale;					// scaling factor
int		WhichColor;				// index into Colors[ ]
int		WhichProjection;		// ORTHO or PERSP
int		Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;				// rotation angles in degrees


// function prototypes:

void	Animate( );
void	Display( );
void	DoAxesMenu( int );
void	DoColorMenu( int );
void	DoDepthBufferMenu( int );
void	DoDepthFightingMenu( int );
void	DoDepthMenu( int );
void	DoDebugMenu( int );
void	DoMainMenu( int );
void	DoProjectMenu( int );
void	DoRasterString( float, float, float, char * );
void	DoStrokeString( float, float, float, float, char * );
float	ElapsedSeconds( );
void	InitGraphics( );
void	InitLists( );
void	InitMenus( );
void	Keyboard( unsigned char, int, int );
void	MouseButton( int, int, int, int );
void	MouseMotion( int, int );
void	Reset( );
void	Resize( int, int );
void	Visibility( int );

void	Axes( float );
void	HsvRgb( float[3], float [3] );
bool	Distort;		// global -- true means to distort the texture

struct point {
	float x, y, z;		// coordinates
	float nx, ny, nz;	// surface normal
	float s, t;		// texture coords
};

int		NumLngs, NumLats;
struct point *	Pts;

struct point *
PtsPointer( int lat, int lng )
{
	if( lat < 0 )	lat += (NumLats-1);
	if( lng < 0 )	lng += (NumLngs-1);
	if( lat > NumLats-1 )	lat -= (NumLats-1);
	if( lng > NumLngs-1 )	lng -= (NumLngs-1);
	return &Pts[ NumLngs*lat + lng ];
}



void
DrawPoint( struct point *p )
{
	glNormal3f( p->nx, p->ny, p->nz );
	glTexCoord2f( p->s, p->t );
	glVertex3f( p->x, p->y, p->z );
}

void
MjbSphere( float radius, int slices, int stacks )
{
	struct point top, bot;		// top, bottom points
	struct point *p;
	// fprintf(stdout, "sphere\n");
	// set the globals:

	NumLngs = slices;
	NumLats = stacks;

	if( NumLngs < 3 )
		NumLngs = 3;

	if( NumLats < 3 )
		NumLats = 3;


	// allocate the point data structure:

	Pts = new struct point[ NumLngs * NumLats ];


	// fill the Pts structure:
	bool flip = true;
	for( int ilat = 0; ilat < NumLats; ilat++ )
	{
		float lat = -M_PI/2.  +  M_PI * (float)ilat / (float)(NumLats-1);
		float xz = cos( lat );
		float y = sin( lat );
		for( int ilng = 0; ilng < NumLngs; ilng++ )
		{
			float lng = -M_PI  +  2. * M_PI * (float)ilng / (float)(NumLngs-1);
			float x =  xz * cos( lng );
			float z = -xz * sin( lng );
			p = PtsPointer( ilat, ilng );
			p->x  = radius * x;
			p->y  = radius * y;
			p->z  = radius * z;
			p->nx = x;
			p->ny = y;
			p->nz = z;
			if( Distort )
			{
				p->s = (( lng + M_PI    ) / ( 2.*M_PI ))*tan(lat*Time);

				// if(flip){
				// 	p->t = (( lat + M_PI/2. ) / M_PI)+(rand()%2)*0.001;
				// 	flip = false;
				// } else{ 
					p->t = (( lat + M_PI/2. ) / M_PI)-tan(lng*Time)*0.001;
				// 	flip = true; 
				// }
				// fprintf(stdout, "s: %f\tt: %f\n", p->s, p->t);
				// fprintf(stdout, "time: %f\n", Time);
			}
			else
			{
				p->s = ( lng + M_PI    ) / ( 2.*M_PI );
				p->t = ( lat + M_PI/2. ) / M_PI;
			}
		}
	}

	top.x =  0.;		top.y  = radius;	top.z = 0.;
	top.nx = 0.;		top.ny = 1.;		top.nz = 0.;
	top.s  = 0.;		top.t  = 1.;

	bot.x =  0.;		bot.y  = -radius;	bot.z = 0.;
	bot.nx = 0.;		bot.ny = -1.;		bot.nz = 0.;
	bot.s  = 0.;		bot.t  =  0.;


	// connect the north pole to the latitude NumLats-2:

	glBegin( GL_QUADS );
	for( int ilng = 0; ilng < NumLngs-1; ilng++ )
	{
		p = PtsPointer( NumLats-1, ilng );
		DrawPoint( p );

		p = PtsPointer( NumLats-2, ilng );
		DrawPoint( p );

		p = PtsPointer( NumLats-2, ilng+1 );
		DrawPoint( p );

		p = PtsPointer( NumLats-1, ilng+1 );
		DrawPoint( p );
	}
	glEnd( );

	// connect the south pole to the latitude 1:

	glBegin( GL_QUADS );
	for( int ilng = 0; ilng < NumLngs-1; ilng++ )
	{
		p = PtsPointer( 0, ilng );
		DrawPoint( p );

		p = PtsPointer( 0, ilng+1 );
		DrawPoint( p );

		p = PtsPointer( 1, ilng+1 );
		DrawPoint( p );

		p = PtsPointer( 1, ilng );
		DrawPoint( p );
	}
	glEnd( );


	// connect the other 4-sided polygons:

	glBegin( GL_QUADS );
	for( int ilat = 2; ilat < NumLats-1; ilat++ )
	{
		for( int ilng = 0; ilng < NumLngs-1; ilng++ )
		{
			p = PtsPointer( ilat-1, ilng );
			DrawPoint( p );

			p = PtsPointer( ilat-1, ilng+1 );
			DrawPoint( p );

			p = PtsPointer( ilat, ilng+1 );
			DrawPoint( p );

			p = PtsPointer( ilat, ilng );
			DrawPoint( p );
		}
	}
	glEnd( );

	delete [ ] Pts;
	Pts = NULL;
}


/* Erick's final project helper functions */

// radius, r
void drawCircle(double radius,float r, float g, float b){
	glBegin(GL_LINE_STRIP);
	glColor3f(r,g,b);
	for(int i = 0; i < 90; i++){
		glVertex3f(radius*cos(i*0.1),0, radius*sin(i*0.1));
	}
	glEnd();
	return;
}

// main program:
#include <time.h>
int
main( int argc, char *argv[ ] )
{
	srand(time(NULL));
	// turn on the glut package:
	// (do this before checking argc and argv since it might
	// pull some command line arguments out)

	glutInit( &argc, argv );


	// setup all the graphics stuff:

	InitGraphics( );


	// create the display structures that will not change:

	InitLists( );


	// init all the global variables used by Display( ):
	// this will also post a redisplay

	Reset( );


	// setup all the user interface stuff:

	InitMenus( );


	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutSetWindow( MainWindow );
	glutMainLoop( );


	// this is here to make the compiler happy:

	return 0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutMainLoop( ) do it

void
Animate( )
{
	// put animation stuff in here -- change some global variables
	// for Display( ) to find:

	// force a call to Display( ) next time it is convenient:
	int ms = glutGet( GLUT_ELAPSED_TIME );
	ms %= MS_PER_CYCLE;
	Time = (float)ms / (float)MS_PER_CYCLE;
	POS = 90*Time;
	// fprintf(stdout, "Animate\n");
	// fprintf(stdout, "Time: %f\n", Time);		

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// draw the complete scene:

void
Display( )
{
	if( DebugOn != 0 )
	{
		fprintf( stderr, "Display\n" );
	}


	// set which window we want to do the graphics into:

	glutSetWindow( MainWindow );


	// erase the background:

	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	if( DepthBufferOn != 0 )
		glEnable( GL_DEPTH_TEST );
	else
		glDisable( GL_DEPTH_TEST );


	// specify shading to be flat:

	glShadeModel( GL_FLAT );


	// set the viewport to a square centered in the window:

	GLsizei vx = glutGet( GLUT_WINDOW_WIDTH );
	GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT );
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	GLint xl = ( vx - v ) / 2;
	GLint yb = ( vy - v ) / 2;
	glViewport( xl, yb,  v, v );


	// set the viewing volume:
	// remember that the Z clipping  values are actually
	// given as DISTANCES IN FRONT OF THE EYE
	// USE gluOrtho2D( ) IF YOU ARE DOING 2D !

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	if( WhichProjection == ORTHO )
		glOrtho( -3., 3.,     -3., 3.,     0.1, 1000. );
	else
		gluPerspective( 90., 1.,	0.1, 1000. );


	// place the objects into the scene:

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );


	// set the eye position, look-at position, and up-vector:

	gluLookAt( 0., 0., 3.,     0., 0., 0.,     0., 1., 0. );


	// rotate the scene:

	glRotatef( (GLfloat)Yrot, 0., 1., 0. );
	glRotatef( (GLfloat)Xrot, 1., 0., 0. );


	// uniformly scale the scene:

	if( Scale < MINSCALE )
		Scale = MINSCALE;
	glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );


	// set the fog parameters:

	if( DepthCueOn != 0 )
	{
		glFogi( GL_FOG_MODE, FOGMODE );
		glFogfv( GL_FOG_COLOR, FOGCOLOR );
		glFogf( GL_FOG_DENSITY, FOGDENSITY );
		glFogf( GL_FOG_START, FOGSTART );
		glFogf( GL_FOG_END, FOGEND );
		glEnable( GL_FOG );
	}
	else
	{
		glDisable( GL_FOG );
	}


	// possibly draw the axes:

	if( AxesOn != 0 )
	{
		glColor3fv( &Colors[WhichColor][0] );
		glCallList( AxesList );
	}


	// since we are using glScalef( ), be sure normals get unitized:

	glEnable( GL_NORMALIZE );


	// draw the current object:

	// glCallList( BoxList );
	// glEnable(GL_TEXTURE_2D);
	// glBindTexture( GL_TEXTURE_2D, tex_mars );
	// glColor3f( 0., 1., 0. );
	
	// MjbSphere(1,100,100);
	// glDisable( GL_TEXTURE_2D );


	/* Put a point light at 0,0,0*/


	



	drawCircle(mercury.dist_from_sun+(SUN_DIAM/SUN_DIAM_DENOM), 1,1,1);
	drawCircle(venus.dist_from_sun	+(SUN_DIAM/SUN_DIAM_DENOM), 1,1,1);
	drawCircle(earth.dist_from_sun	+(SUN_DIAM/SUN_DIAM_DENOM), 1,1,1);
	drawCircle(mars.dist_from_sun		+(SUN_DIAM/SUN_DIAM_DENOM), 1,1,1);
	drawCircle(jupiter.dist_from_sun+(SUN_DIAM/SUN_DIAM_DENOM), 1,1,1);
	drawCircle(saturn.dist_from_sun	+(SUN_DIAM/SUN_DIAM_DENOM), 1,1,1);
	drawCircle(uranus.dist_from_sun	+(SUN_DIAM/SUN_DIAM_DENOM), 1,1,1);
	drawCircle(neptune.dist_from_sun+(SUN_DIAM/SUN_DIAM_DENOM), 1,1,1);

	glEnable( GL_LIGHTING );
	glLightfv( GL_LIGHT2, GL_POSITION, Array3(0,0,0));
	glLightfv( GL_LIGHT2, GL_SPOT_DIRECTION, Array3(0,0,0));
	// glLightf( GL_LIGHT2, GL_SPOT_EXPONENT, 1. );
	// glLightf( GL_LIGHT2, GL_SPOT_CUTOFF, 45. );
	glLightfv( GL_LIGHT2, GL_AMBIENT, Array3( 0., 0., 0. ) );
	glLightfv( GL_LIGHT2, GL_DIFFUSE, Array3( 1, 1, 0 ) );
	glLightfv( GL_LIGHT2, GL_SPECULAR, Array3( 1, 1, 1 ) );

	glEnable(GL_LIGHT2);

	glShadeModel( GL_FLAT );
	/* A Sun */
	// glEnable(GL_TEXTURE_2D);
	// glBindTexture( GL_TEXTURE_2D, tex_mercury );
	glPushMatrix();
		glColor3f( 1., 1., 0. );
		// glTranslatef(mercury.dist_from_sun,0,0);
		MjbSphere((sun.diameter)/RADIUS_SCALER,100,100);
	glPopMatrix();
	// glDisable( GL_TEXTURE_2D );
	/*******/


	/* A Mercury */
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, tex_mercury );
	glPushMatrix();
		glColor3f( 1., 0., 0. );
		glTranslatef(
			(mercury.dist_from_sun+(SUN_DIAM/SUN_DIAM_DENOM))*
			cos(POS*(mercury.orbital_period)*ORBIT_SCALER),0,
			
			(mercury.dist_from_sun+(SUN_DIAM/SUN_DIAM_DENOM))*
			sin(POS*(mercury.orbital_period)*ORBIT_SCALER)
		);
		MjbSphere((mercury.diameter)/RADIUS_SCALER,100,100);
	glPopMatrix();
	glDisable( GL_TEXTURE_2D );
	/*******/
	
	/* A Venus */
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, tex_venus );
	glPushMatrix();
		glColor3f( 1., 0., 0. );
		glTranslatef(
			(venus.dist_from_sun+(SUN_DIAM/SUN_DIAM_DENOM))*
			cos(POS*(venus.orbital_period)*ORBIT_SCALER),0,
			
			(venus.dist_from_sun+(SUN_DIAM/SUN_DIAM_DENOM))*
			sin(POS*(venus.orbital_period)*ORBIT_SCALER)
		);
		MjbSphere((venus.diameter)/RADIUS_SCALER,100,100);
	glPopMatrix();
	glDisable( GL_TEXTURE_2D );
	/*******/

	/* A Mars */
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, tex_mars );
	glPushMatrix();
		glColor3f( 1., 0., 0. );
		glTranslatef(
			(mars.dist_from_sun+(SUN_DIAM/SUN_DIAM_DENOM))*
			cos(POS*(mars.orbital_period)*ORBIT_SCALER),0,
			
			(mars.dist_from_sun+(SUN_DIAM/SUN_DIAM_DENOM))*
			sin(POS*(mars.orbital_period)*ORBIT_SCALER)
		);
		MjbSphere((mars.diameter)/RADIUS_SCALER,100,100);
	glPopMatrix();
	glDisable( GL_TEXTURE_2D );
	/*******/

	/* A Jupiter */
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, tex_jupiter );
	glPushMatrix();
		glColor3f( 1., 0., 0. );
		glTranslatef(
			(jupiter.dist_from_sun+(SUN_DIAM/SUN_DIAM_DENOM))*
			cos(POS*(jupiter.orbital_period)*ORBIT_SCALER),0,
			
			(jupiter.dist_from_sun+(SUN_DIAM/SUN_DIAM_DENOM))*
			sin(POS*(jupiter.orbital_period)*ORBIT_SCALER)
		);
		MjbSphere((jupiter.diameter)/RADIUS_SCALER,100,100);
	glPopMatrix();
	glDisable( GL_TEXTURE_2D );
	/*******/

	/* A Satrun */
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, tex_saturn );
	glPushMatrix();
		glColor3f( 1., 0., 0. );
		glTranslatef(
			(saturn.dist_from_sun+(SUN_DIAM/SUN_DIAM_DENOM))*
			cos(POS*(saturn.orbital_period)*ORBIT_SCALER),0,
			
			(saturn.dist_from_sun+(SUN_DIAM/SUN_DIAM_DENOM))*
			sin(POS*(saturn.orbital_period)*ORBIT_SCALER)
		);
		MjbSphere((saturn.diameter)/RADIUS_SCALER,100,100);
	glPopMatrix();
	glDisable( GL_TEXTURE_2D );
	/*******/

	/* A Uranus */
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, tex_uranus );
	glPushMatrix();
		glColor3f( 1., 0., 0. );
		glTranslatef(
			(uranus.dist_from_sun+(SUN_DIAM/SUN_DIAM_DENOM))*
			cos(POS*(uranus.orbital_period)*ORBIT_SCALER),0,
			
			(uranus.dist_from_sun+(SUN_DIAM/SUN_DIAM_DENOM))*
			sin(POS*(uranus.orbital_period)*ORBIT_SCALER)
		);
		MjbSphere((uranus.diameter)/RADIUS_SCALER,100,100);
	glPopMatrix();
	glDisable( GL_TEXTURE_2D );
	/*******/

	/* A Neptune */
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, tex_neptune );
	glPushMatrix();
		glColor3f( 1., 0., 0. );
		glTranslatef(
			(neptune.dist_from_sun+(SUN_DIAM/SUN_DIAM_DENOM))*
			cos(POS*(neptune.orbital_period)*ORBIT_SCALER),0,
			
			(neptune.dist_from_sun+(SUN_DIAM/SUN_DIAM_DENOM))*
			sin(POS*(neptune.orbital_period)*ORBIT_SCALER)
		);
		// glTranslatef(
		// 	(neptune.dist_from_sun+(SUN_DIAM/SUN_DIAM_DENOM))*
		// 	cos(POS*(neptune.orbital_period)*ORBIT_SCALER),0,
			
		// 	(neptune.dist_from_sun+(SUN_DIAM/SUN_DIAM_DENOM))*
		// 	sin(POS*(neptune.orbital_period)*ORBIT_SCALER)
		// );
		// glTranslatef(((POS % neptune.orbital_period)/neptune.orbital_period)*360,0
		// ,((POS % neptune.orbital_period)/neptune.orbital_period)*360);
		MjbSphere((neptune.diameter)/RADIUS_SCALER,100,100);
	glPopMatrix();
	glDisable( GL_TEXTURE_2D );
	/*******/

	glDisable(GL_LIGHTING);
	
	glEnd( );
	
	
	if( DepthFightingOn != 0 )
	{
		glPushMatrix( );
			glRotatef( 90.,   0., 1., 0. );
			glCallList( BoxList );
		glPopMatrix( );
	}


	// draw some gratuitous text that just rotates on top of the scene:

	// glDisable( GL_DEPTH_TEST );
	// glColor3f( 0., 1., 1. );
	// DoRasterString( 0., 1., 0., (char *)"Text That Moves" );


	// draw some gratuitous text that is fixed on the screen:
	//
	// the projection matrix is reset to define a scene whose
	// world coordinate system goes from 0-100 in each axis
	//
	// this is called "percent units", and is just a convenience
	//
	// the modelview matrix is reset to identity as we don't
	// want to transform these coordinates

	glDisable( GL_DEPTH_TEST );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluOrtho2D( 0., 100.,     0., 100. );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	// glColor3f( 1., 1., 1. );
	// DoRasterString( 5., 5., 0., (char *)"Text That Doesn't" );

	// glEnable(GL_TEXTURE_2D);
	// glBindTexture( GL_TEXTURE_2D, Tex0 );
	// // glBegin( GL_QUADS );
	// MjbSphere(10,10,10 );
	// glEnd( );
	// swap the double-buffered framebuffers:

	glutSwapBuffers( );


	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !

	glFlush( );
}


void
DoAxesMenu( int id )
{
	AxesOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoColorMenu( int id )
{
	WhichColor = id - RED;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDebugMenu( int id )
{
	DebugOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthBufferMenu( int id )
{
	DepthBufferOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthFightingMenu( int id )
{
	DepthFightingOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthMenu( int id )
{
	DepthCueOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// main menu callback:

void
DoMainMenu( int id )
{
	switch( id )
	{
		case RESET:
			Reset( );
			break;

		case QUIT:
			// gracefully close out the graphics:
			// gracefully close the graphics window:
			// gracefully exit the program:
			glutSetWindow( MainWindow );
			glFinish( );
			glutDestroyWindow( MainWindow );
			exit( 0 );
			break;

		default:
			fprintf( stderr, "Don't know what to do with Main Menu ID %d\n", id );
	}

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoProjectMenu( int id )
{
	WhichProjection = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// use glut to display a string of characters using a raster font:

void
DoRasterString( float x, float y, float z, char *s )
{
	glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );

	char c;			// one character to print
	for( ; ( c = *s ) != '\0'; s++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
	}
}


// use glut to display a string of characters using a stroke font:

void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
	glPushMatrix( );
		glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
		float sf = ht / ( 119.05f + 33.33f );
		glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
		char c;			// one character to print
		for( ; ( c = *s ) != '\0'; s++ )
		{
			glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
		}
	glPopMatrix( );
}


// return the number of seconds since the start of the program:

float
ElapsedSeconds( )
{
	// get # of milliseconds since the start of the program:

	int ms = glutGet( GLUT_ELAPSED_TIME );

	// convert it to seconds:

	return (float)ms / 1000.f;
}


// initialize the glui window:

void
InitMenus( )
{
	glutSetWindow( MainWindow );

	int numColors = sizeof( Colors ) / ( 3*sizeof(int) );
	int colormenu = glutCreateMenu( DoColorMenu );
	for( int i = 0; i < numColors; i++ )
	{
		glutAddMenuEntry( ColorNames[i], i );
	}

	int axesmenu = glutCreateMenu( DoAxesMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthcuemenu = glutCreateMenu( DoDepthMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthbuffermenu = glutCreateMenu( DoDepthBufferMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthfightingmenu = glutCreateMenu( DoDepthFightingMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int debugmenu = glutCreateMenu( DoDebugMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int projmenu = glutCreateMenu( DoProjectMenu );
	glutAddMenuEntry( "Orthographic",  ORTHO );
	glutAddMenuEntry( "Perspective",   PERSP );

	int mainmenu = glutCreateMenu( DoMainMenu );
	glutAddSubMenu(   "Axes",          axesmenu);
	glutAddSubMenu(   "Colors",        colormenu);
	glutAddSubMenu(   "Depth Buffer",  depthbuffermenu);
	glutAddSubMenu(   "Depth Fighting",depthfightingmenu);
	glutAddSubMenu(   "Depth Cue",     depthcuemenu);
	glutAddSubMenu(   "Projection",    projmenu );
	glutAddMenuEntry( "Reset",         RESET );
	glutAddSubMenu(   "Debug",         debugmenu);
	glutAddMenuEntry( "Quit",          QUIT );

// attach the pop-up menu to the right mouse button:

	glutAttachMenu( GLUT_RIGHT_BUTTON );
}



// initialize the glut and OpenGL libraries:
//	also setup display lists and callback functions

void
InitGraphics( )
{
	// request the display modes:
	// ask for red-green-blue-alpha color, double-buffering, and z-buffering:

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	// set the initial window configuration:

	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( INIT_WINDOW_SIZE, INIT_WINDOW_SIZE );

	// open the window and set its title:

	MainWindow = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );

	// set the framebuffer clear values:

	glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );

	// setup the callback functions:
	// DisplayFunc -- redraw the window
	// ReshapeFunc -- handle the user resizing the window
	// KeyboardFunc -- handle a keyboard input
	// MouseFunc -- handle the mouse button going down or up
	// MotionFunc -- handle the mouse moving with a button down
	// PassiveMotionFunc -- handle the mouse moving with a button up
	// VisibilityFunc -- handle a change in window visibility
	// EntryFunc	-- handle the cursor entering or leaving the window
	// SpecialFunc -- handle special keys on the keyboard
	// SpaceballMotionFunc -- handle spaceball translation
	// SpaceballRotateFunc -- handle spaceball rotation
	// SpaceballButtonFunc -- handle spaceball button hits
	// ButtonBoxFunc -- handle button box hits
	// DialsFunc -- handle dial rotations
	// TabletMotionFunc -- handle digitizing tablet motion
	// TabletButtonFunc -- handle digitizing tablet button hits
	// MenuStateFunc -- declare when a pop-up menu is in use
	// TimerFunc -- trigger something to happen a certain time from now
	// IdleFunc -- what to do when nothing else is going on

	glutSetWindow( MainWindow );
	glutDisplayFunc( Display );
	glutReshapeFunc( Resize );
	glutKeyboardFunc( Keyboard );
	glutMouseFunc( MouseButton );
	glutMotionFunc( MouseMotion );
	glutPassiveMotionFunc( NULL );
	glutVisibilityFunc( Visibility );
	glutEntryFunc( NULL );
	glutSpecialFunc( NULL );
	glutSpaceballMotionFunc( NULL );
	glutSpaceballRotateFunc( NULL );
	glutSpaceballButtonFunc( NULL );
	glutButtonBoxFunc( NULL );
	glutDialsFunc( NULL );
	glutTabletMotionFunc( NULL );
	glutTabletButtonFunc( NULL );
	glutMenuStateFunc( NULL );
	glutTimerFunc( -1, NULL, 0 );
	glutIdleFunc( Animate );

	// init glew (a window must be open to do this):

#ifdef WIN32
	GLenum err = glewInit( );
	if( err != GLEW_OK )
	{
		fprintf( stderr, "glewInit Error\n" );
	}
	else
		fprintf( stderr, "GLEW initialized OK\n" );
	fprintf( stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
	int width, height;
	unsigned char * TextureArray0 = BmpToTexture("bmps/mercury.bmp", &width, &height );
	unsigned char * TextureArray1 = BmpToTexture("bmps/venus.bmp", &width, &height );
	unsigned char * TextureArray2 = BmpToTexture("bmps/mars.bmp", &width, &height );
	unsigned char * TextureArray3 = BmpToTexture("bmps/jupiter.bmp", &width, &height );
	unsigned char * TextureArray4 = BmpToTexture("bmps/saturn.bmp", &width, &height );
	unsigned char * TextureArray5 = BmpToTexture("bmps/uranus.bmp", &width, &height );
	unsigned char * TextureArray6 = BmpToTexture("bmps/neptune.bmp", &width, &height );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	
	
	/******/
	glGenTextures( 1, &tex_mercury );
	glBindTexture( GL_TEXTURE_2D, tex_mercury);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray0);
	/******/
	/******/
	glGenTextures( 1, &tex_venus );
	glBindTexture( GL_TEXTURE_2D, tex_venus);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray1);
	/******/
		/******/
	glGenTextures( 1, &tex_mars );
	glBindTexture( GL_TEXTURE_2D, tex_mars);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray2);
	/******/
	/******/
	glGenTextures( 1, &tex_jupiter );
	glBindTexture( GL_TEXTURE_2D, tex_jupiter);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray3);
	/******/
	/******/
	glGenTextures( 1, &tex_saturn );
	glBindTexture( GL_TEXTURE_2D, tex_saturn);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray4);
	/******/
	/******/
	glGenTextures( 1, &tex_uranus );
	glBindTexture( GL_TEXTURE_2D, tex_uranus);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray5);
	/******/
	/******/
	glGenTextures( 1, &tex_neptune );
	glBindTexture( GL_TEXTURE_2D, tex_neptune);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray6);
	/******/

	// glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureArray1);

}


// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )

void
InitLists( )
{
	float dx = BOXSIZE / 2.f;
	float dy = BOXSIZE / 2.f;
	float dz = BOXSIZE / 2.f;
	glutSetWindow( MainWindow );

	// create the object:

	BoxList = glGenLists( 1 );
	// glNewList( BoxList, GL_COMPILE );

	// 	glBegin( GL_QUADS );

	// 		glColor3f( 0., 0., 1. );
	// 		glNormal3f( 0., 0.,  1. );
	// 			glVertex3f( -dx, -dy,  dz );
	// 			glVertex3f(  dx, -dy,  dz );
	// 			glVertex3f(  dx,  dy,  dz );
	// 			glVertex3f( -dx,  dy,  dz );

	// 		glNormal3f( 0., 0., -1. );
	// 			glTexCoord2f( 0., 0. );
	// 			glVertex3f( -dx, -dy, -dz );
	// 			glTexCoord2f( 0., 1. );
	// 			glVertex3f( -dx,  dy, -dz );
	// 			glTexCoord2f( 1., 1. );
	// 			glVertex3f(  dx,  dy, -dz );
	// 			glTexCoord2f( 1., 0. );
	// 			glVertex3f(  dx, -dy, -dz );

	// 		glColor3f( 1., 0., 0. );
	// 		glNormal3f(  1., 0., 0. );
	// 			glVertex3f(  dx, -dy,  dz );
	// 			glVertex3f(  dx, -dy, -dz );
	// 			glVertex3f(  dx,  dy, -dz );
	// 			glVertex3f(  dx,  dy,  dz );

	// 		glNormal3f( -1., 0., 0. );
	// 			glVertex3f( -dx, -dy,  dz );
	// 			glVertex3f( -dx,  dy,  dz );
	// 			glVertex3f( -dx,  dy, -dz );
	// 			glVertex3f( -dx, -dy, -dz );

	// 		glColor3f( 0., 1., 0. );
	// 		glNormal3f( 0.,  1., 0. );
	// 			glVertex3f( -dx,  dy,  dz );
	// 			glVertex3f(  dx,  dy,  dz );
	// 			glVertex3f(  dx,  dy, -dz );
	// 			glVertex3f( -dx,  dy, -dz );

	// 		glNormal3f( 0., -1., 0. );
	// 			glVertex3f( -dx, -dy,  dz );
	// 			glVertex3f( -dx, -dy, -dz );
	// 			glVertex3f(  dx, -dy, -dz );
	// 			glVertex3f(  dx, -dy,  dz );

	// 	glEnd( );

	// glEndList( );


	// create the axes:

	AxesList = glGenLists( 1 );
	glNewList( AxesList, GL_COMPILE );
		glLineWidth( AXES_WIDTH );
			Axes( 1.5 );
		glLineWidth( 1. );
	glEndList( );
}


// the keyboard callback:

void
Keyboard( unsigned char c, int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );

	switch( c )
	{
		case 't':
		case 'T':
			if(texture){
				texture = false;
			} else texture = true;		
			break;
		case 'd':
		case 'D':
			if(Distort){
				Distort = false;
			} else Distort = true;
			break;
		case 'o':
		case 'O':
			WhichProjection = ORTHO;
			break;

		case 'p':
		case 'P':
			WhichProjection = PERSP;
			break;

		case 'q':
		case 'Q':
		case ESCAPE:
			DoMainMenu( QUIT );	// will not return here
			break;				// happy compiler

		default:
			fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
	}

	// force a call to Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// called when the mouse button transitions down or up:

void
MouseButton( int button, int state, int x, int y )
{
	int b = 0;			// LEFT, MIDDLE, or RIGHT

	if( DebugOn != 0 )
		fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );

	
	// get the proper button bit mask:

	switch( button )
	{
		case GLUT_LEFT_BUTTON:
			b = LEFT;		break;

		case GLUT_MIDDLE_BUTTON:
			b = MIDDLE;		break;

		case GLUT_RIGHT_BUTTON:
			b = RIGHT;		break;

		case SCROLL_WHEEL_UP:
			Scale += SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
			// keep object from turning inside-out or disappearing:
			if (Scale < MINSCALE)
				Scale = MINSCALE;
			break;

		case SCROLL_WHEEL_DOWN:
			Scale -= SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
			// keep object from turning inside-out or disappearing:
			if (Scale < MINSCALE)
				Scale = MINSCALE;
			break;

		default:
			b = 0;
			fprintf( stderr, "Unknown mouse button: %d\n", button );
	}


	// button down sets the bit, up clears the bit:

	if( state == GLUT_DOWN )
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}
}


// called when the mouse moves while a button is down:

void
MouseMotion( int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "MouseMotion: %d, %d\n", x, y );


	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	if( ( ActiveButton & LEFT ) != 0 )
	{
		Xrot += ( ANGFACT*dy );
		Yrot += ( ANGFACT*dx );
	}


	if( ( ActiveButton & MIDDLE ) != 0 )
	{
		Scale += SCLFACT * (float) ( dx - dy );

		// keep object from turning inside-out or disappearing:

		if( Scale < MINSCALE )
			Scale = MINSCALE;
	}

	Xmouse = x;			// new current position
	Ymouse = y;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset( )
{
	ActiveButton = 0;
	AxesOn = 1;
	DebugOn = 0;
	DepthBufferOn = 1;
	DepthFightingOn = 0;
	DepthCueOn = 0;
	Scale  = 1.0;
	WhichColor = WHITE;
	WhichProjection = PERSP;
	Xrot = Yrot = 0.;
}


// called when user resizes the window:

void
Resize( int width, int height )
{
	if( DebugOn != 0 )
		fprintf( stderr, "ReSize: %d, %d\n", width, height );

	// don't really need to do anything since window size is
	// checked each time in Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// handle a change to the window's visibility:

void
Visibility ( int state )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Visibility: %d\n", state );

	if( state == GLUT_VISIBLE )
	{
		glutSetWindow( MainWindow );
		glutPostRedisplay( );
	}
	else
	{
		// could optimize by keeping track of the fact
		// that the window is not visible and avoid
		// animating or redrawing it ...
	}
}



///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////


// the stroke characters 'X' 'Y' 'Z' :

static float xx[ ] = {
		0.f, 1.f, 0.f, 1.f
	      };

static float xy[ ] = {
		-.5f, .5f, .5f, -.5f
	      };

static int xorder[ ] = {
		1, 2, -3, 4
		};

static float yx[ ] = {
		0.f, 0.f, -.5f, .5f
	      };

static float yy[ ] = {
		0.f, .6f, 1.f, 1.f
	      };

static int yorder[ ] = {
		1, 2, 3, -2, 4
		};

static float zx[ ] = {
		1.f, 0.f, 1.f, 0.f, .25f, .75f
	      };

static float zy[ ] = {
		.5f, .5f, -.5f, -.5f, 0.f, 0.f
	      };

static int zorder[ ] = {
		1, 2, 3, 4, -5, 6
		};

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)

void
Axes( float length )
{
	glBegin( GL_LINE_STRIP );
		glVertex3f( length, 0., 0. );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., length, 0. );
	glEnd( );
	glBegin( GL_LINE_STRIP );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., 0., length );
	glEnd( );

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 4; i++ )
		{
			int j = xorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 5; i++ )
		{
			int j = yorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 6; i++ )
		{
			int j = zorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
		}
	glEnd( );

}


// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//		glColor3fv( rgb );

void
HsvRgb( float hsv[3], float rgb[3] )
{
	// guarantee valid input:

	float h = hsv[0] / 60.f;
	while( h >= 6. )	h -= 6.;
	while( h <  0. ) 	h += 6.;

	float s = hsv[1];
	if( s < 0. )
		s = 0.;
	if( s > 1. )
		s = 1.;

	float v = hsv[2];
	if( v < 0. )
		v = 0.;
	if( v > 1. )
		v = 1.;

	// if sat==0, then is a gray:

	if( s == 0.0 )
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}

	// get an rgb from the hue itself:
	
	float i = floor( h );
	float f = h - i;
	float p = v * ( 1.f - s );
	float q = v * ( 1.f - s*f );
	float t = v * ( 1.f - ( s * (1.f-f) ) );

	float r, g, b;			// red, green, blue
	switch( (int) i )
	{
		case 0:
			r = v;	g = t;	b = p;
			break;
	
		case 1:
			r = q;	g = v;	b = p;
			break;
	
		case 2:
			r = p;	g = v;	b = t;
			break;
	
		case 3:
			r = p;	g = q;	b = v;
			break;
	
		case 4:
			r = t;	g = p;	b = v;
			break;
	
		case 5:
			r = v;	g = p;	b = q;
			break;
	}


	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}
