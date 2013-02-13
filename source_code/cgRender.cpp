#include "cgRender.h"
#include "cgFile.h"
#include "cgNormals.h"

#define AMOUNT_CHANGE 0.1
#define WIDTH 512
#define HEIGHT 512
#define PI 3.14

/* Global Camera Values ************************/
float r = 0.2;
float angle = 0;
GLfloat fovy    = 60.0f;
GLfloat aspect  = (GLfloat) WIDTH/HEIGHT;
GLfloat near    = 0.1f;
GLfloat far     = 100.0f;
GLfloat posX	= 0.3;
GLfloat posY	= -0.1;
GLfloat posZ	= -0.1;
GLfloat centerx = 0.07f;
GLfloat centery = -0.13f;
GLfloat centerz = -0.07f;
GLfloat upx     = 0.0f;
GLfloat upy     = 1.0f;
GLfloat upz     = 0.0f;
/*************************************************/

/* glLighting Values *****************************/
GLUquadricObj *lightSphere;
float rSphere = 0.2;
float angleSphere = -211.131;
GLfloat lightCenterX = -0.0645798;
GLfloat lightCenterZ = 0.0779469;
GLfloat lightX;		//Calculated during runtime
GLfloat lightY;		//Calculated during runtime
GLfloat lightZ;		//Calculated during runtime
GLfloat ambient [] = {0.1f, 0.1f, 0.1f, 0.0f};
GLfloat position [] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat specular [] = {0.1f, 0.1f, 0.1f, 0.3f};
GLfloat shininess [] = {120.0};
/*************************************************/

/* Declare global variables for processing *******/
int pointCount;
int polygonCount;

vector <point> points;
vector <polygon> polygons;
/************************************************/

/* Declare Texture Variables ********************/
int texWidth, texHeight, texMax;

unsigned char * ppmTex;

GLuint texture_id;
/************************************************/

/* Toggles for rendering modes *****************/
bool texturing = false;
bool axis = false;
bool bulb = false;
/***********************************************/

void initGL() 
{
	cout << "Initialising GL" << endl;
	glClearColor (0.0, 0.0, 0.0, 0.0); 						// Black Background

	//Instantiate "light bulb"
	lightSphere=gluNewQuadric();
	gluQuadricNormals(lightSphere, GLU_SMOOTH);
  
	// Enable smooth shading
	glShadeModel (GL_SMOOTH);
  
	//Enable Lighting
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE,  LightDiffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
  
	// Set material parameters
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  
	//Enable Z-Buffering
	glEnable(GL_DEPTH_TEST);							

	//Enable coloured materials reative to the ambient light source
	glEnable( GL_COLOR_MATERIAL );
	glColorMaterial(GL_FRONT, GL_AMBIENT);
}

void display(void)
{
	//Calculate camera x,z coordinates
	posX = centerx + r*cos(angle);
	posZ = centerz + r*sin(angle);
	
	//Calculate light source x,z coordinates
	lightX = lightCenterX + rSphere*cos(angleSphere);
	lightZ = lightCenterZ + rSphere*sin(angleSphere);
	position[0] = lightX;
	position[1] = lightY;
	position[2] = lightZ;

	//Bind textures, only run once when program starts
	if (texture_id == NULL)
	{
		cout << "Initialising Texture" << endl;
		glEnable( GL_TEXTURE_2D );

		//Generate texture address
		glGenTextures(1, &texture_id);

		//Bind texture to 2D
		glBindTexture(GL_TEXTURE_2D, texture_id);

		//Enable 2D texture wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		//Enable minifying and maxifying texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Push buffer into texture memory
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texWidth, texHeight, GL_RGB, GL_UNSIGNED_BYTE, ppmTex);
		
		//Free buffer memory
		delete(ppmTex);

		glDisable( GL_TEXTURE_2D );
	}

	//Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set colour to white
	glColor3f (1.0, 1.0, 1.0);
	
	//Reset model matrix
	glLoadIdentity ();

	//Set camera position
	gluLookAt(posX, posY, posZ, centerx, centery, centerz, upx, upy, upz);

	//Set light source position
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	//If texturing toggle set true, enable GL texturing
	if(texturing)
	  	glEnable(GL_TEXTURE_2D);

	//Iterate through and draw every polygon, fetching and passing texture coordinates iff texturing is enabled
	for (int i = 0; i < polygons.size(); i++)
	{
	    glBegin( GL_POLYGON );
	    for (int j = 0; j < polygons[i].pointlist.size(); j++)
		{
			if(texturing)
			{
		    	// Define texture coordinates of vertex
	    		glTexCoord2f(points[polygons[i].pointlist[j]].xTexture, points[polygons[i].pointlist[j]].yTexture);
			}
	    	// Define normal of vertex
	    	glNormal3f(points[polygons[i].pointlist[j]].xNormal, points[polygons[i].pointlist[j]].yNormal, points[polygons[i].pointlist[j]].zNormal);
		    // Define coordinates of vertex
			glVertex3f(points[polygons[i].pointlist[j]].x, points[polygons[i].pointlist[j]].y, points[polygons[i].pointlist[j]].z);
    	};
    glEnd();
	};

	//If GL textures were enabled, disable them again
	if(texturing)
		glDisable(GL_TEXTURE_2D);
	
	//If axis toggle is enabled, draw xyz axis in red, green and blue respectively
	if(axis)
	{
		glBegin (GL_LINES);
		glColor3f (1.0, 0.0, 0.0);
		glVertex3f(0.0, 0, 0.0);
		glVertex3f(0.5, 0.0, 0.0);
		glColor3f (0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.5, 0.0);
		glColor3f (0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.5);
		glEnd();
	}

	//If the physical "light bulb" rendering toggle is enabled, draw "bulb" at light source location (makes positioning light easier)
	if(bulb)
	{
		glColor3f (3.0, 3.0, 0.0);					//Yellow bulb
		glTranslatef( -lightX,-lightY, -lightZ );	//Translate "bulb" to GL ambient light source coordinates
		gluSphere(lightSphere, 0.01, 20, 20);		//Use glut function to render the sphere
	}
	
	//Push rendered scene to display
	glFlush ();
}

void reshape (int w, int h)
{
	cout << "reshape" << endl;
	
	//Resize window
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	//Change matrix mode
	glMatrixMode (GL_PROJECTION);
	//Clear matrix
	glLoadIdentity();
	//Set Perspective values from global variables
	gluPerspective(fovy, aspect, near, far);
	//Revert to modelview matrix
	glMatrixMode (GL_MODELVIEW);
	//Clear matrix
	glLoadIdentity();
	//Reset camera to our settings
	gluLookAt(posX, posY, posZ, centerx, centery, centerz, upx, upy, upz);
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 27: // ESC - exit render
    exit(0);
    break;
  case 113: // q - lower camera
	posY = posY - AMOUNT_CHANGE;
	glutPostRedisplay();
	break;
  case 97: // a - rotate camera left
	angle += 0.5/(2*PI);
	if (angle < 0)
		angle -= 2*PI;
	glutPostRedisplay();
	break;
  case 119: // w - move towards face
	r -= AMOUNT_CHANGE;
	glutPostRedisplay();
	break;
  case 115: // s - move away from face
	r += AMOUNT_CHANGE;
	glutPostRedisplay();
	break;
  case 101: // e - raise camera
	posY = posY + AMOUNT_CHANGE;
	glutPostRedisplay();
	break;
  case 100: // d - rotate camera right
	angle -= 0.5/(2*PI);
	if (angle > 2*PI)
		angle += 2*PI;
	glutPostRedisplay();
	break;
  case 117: // u - lower light source
	lightY += AMOUNT_CHANGE;
	glutPostRedisplay();
	break;
  case 106: // j - rotate light source left
	angleSphere += 0.5/(2*PI);
	if (angleSphere < 0)
		angleSphere -= 2*PI;
	glutPostRedisplay();
	break;
  case 105: // i - move light source towards face
	rSphere -= AMOUNT_CHANGE;
	glutPostRedisplay();
	break;
  case 107: // k - move light source away from face
	rSphere += AMOUNT_CHANGE;
	glutPostRedisplay();
	break;
  case 111: // o - raise light source
	lightY -= AMOUNT_CHANGE;
	glutPostRedisplay();
	break;
  case 108: // l - rotate light source right
	angleSphere -= 0.5/(2*PI);
	if (angleSphere > 2*PI)
		angleSphere += 2*PI;
	glutPostRedisplay();
	break;
  case 116: // t - toggle texturing of face
	texturing = !texturing;
	glutPostRedisplay();
	break;
  case 121: // y - toggle rendering of the axis lines
	axis = !axis;
	glutPostRedisplay();
	break;
  case 114: // r - toggle rendering of the "light bulb"
	bulb = !bulb;
	glutPostRedisplay();
	break;
  }
}

int main(int argc, char** argv)
{
	cout << "intitialising Window" << endl;

	//Initialise glut and set display mode
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); 

	//Create rendering window
	glutInitWindowSize (WIDTH, HEIGHT); 
	glutInitWindowPosition (0, 0);
	glutCreateWindow (argv[0]);

	//Initialise GL render engine
	initGL();
	
	//Parse the data files
	readVTK();
	readTexture();

	cout << "Data fetch complete - Processing" << endl;

	//Process vertex normals
	normaliseVNormals();

	cout << "Ready To Render" << endl;

	// Initialize callback functions
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	// Start rendering 
	glutMainLoop();
	return 0;
}
