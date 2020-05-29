// OpenGL/GLUT starter kit for Windows 7 and Visual Studio 2010
// Created spring, 2011
//
// This is a starting point for OpenGl applications.
// Add code to the "display" function below, or otherwise
// modify this file to get your desired results.
//
// For the first assignment, add this file to an empty Windows Console project
//		and then compile and run it as is.
// NOTE: You should also have glut.h,
// glut32.dll, and glut32.lib in the directory of your project.
// OR, see GlutDirectories.txt for a better place to put them.

#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>
#include <vector>
#include "glut.h"
#include "graphics.h"
#include "terrain.h"
#include "rat.h"
#include "Code_keys.h"



// Global Variables (Only what you need!)
double screen_x = 700;
double screen_y = 500;

Terrain gTerrain;
Rat gRat;
bool gLeftButtonDown = false;
bool gRightButtonDown = false;
bool gMiddleButtonDown = false;
double at_z = 0.0;
double tilt = .20;
viewtype current_view = perspective_view;

// 
// Functions that draw basic primitives
//


double getTime() {
	static clock_t start_time = clock();
	clock_t current_time = clock();
	double t = double(current_time - start_time) / CLOCKS_PER_SEC;
	return t;
}

void DrawRectangle(double x1, double y1, double x2, double y2)
{
	glBegin(GL_QUADS);
	glVertex2d(x1,y1);
	glVertex2d(x2,y1);
	glVertex2d(x2,y2);
	glVertex2d(x1,y2);
	glEnd();
}

void DrawLine(double x1, double y1, double x2, double y2)
{
	glBegin(GL_LINES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();
}

void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
{
	glBegin(GL_TRIANGLES);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glVertex2d(x3,y3);
	glEnd();
}

// Outputs a string of text at the specified location.
void DrawText(double x, double y, const char *string)
{
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	
	int len, i;
	glRasterPos2d(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) 
	{
		glutBitmapCharacter(font, string[i]);
	}

    glDisable(GL_BLEND);
}


void SetTopView(int w, int h)
{
	// go into 2D mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double world_margin_x = 0.5;
	double world_margin_y = 0.5;
	gluOrtho2D(-world_margin_x, WIDTH + world_margin_x,
		-world_margin_y, HEIGHT + world_margin_y);
	glMatrixMode(GL_MODELVIEW);
}

void SetPerspectiveView(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (GLdouble)w / (GLdouble)h;
	gluPerspective(
		/* field of view in degree */ 40.0,
		/* aspect ratio */ aspectRatio,
		/* Z near */ .1, /* Z far */ 200);
	glMatrixMode(GL_MODELVIEW);
}

std::vector<double> CrossProduct(std::vector<double> a, std::vector<double> b) {

	// [x, y, z]
	// [0, 1, 2 ]
	double x = (a[1] * b[2]) - (a[2] * b[1]);
	double y = (a[0] * b[2]) - (a[2] * b[0]);
	double z = (a[0] * b[1]) - (a[1] * b[0]);
	std::vector<double> tmp_v(3);
	tmp_v[0] = x;
	tmp_v[1] = y;
	tmp_v[2] = z;
	return tmp_v;
}

std::vector<double> normalize(std::vector<double> a) {
	double distance = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / distance;
	a[1] = a[1] / distance;
	a[2] = a[2] / distance;
	return a;
}
//
// GLUT callback functions
//

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	if (IsKeyPressed('a')) {
		gRat.SpinLeft();
	}

	if (IsKeyPressed('d')) {
		gRat.SpinRight();
	}

	if (IsKeyPressed('w')) {
		
		gRat.ScurryForward();
	}

	if (IsKeyPressed('r')) {
		current_view = rat_view;
		SetPerspectiveView(screen_x, screen_y);
	}

	if (IsKeyPressed('p')) {
		current_view = perspective_view;
		SetPerspectiveView(screen_x, screen_y);
	}
	
	if (IsKeyPressed('t')) {
		current_view = top_view;
		SetTopView(screen_x, screen_y);
	}

	if (IsKeyPressed('u')) {
		if (current_view == rat_view) {
			tilt -= .1;
		}
	}
	if (IsKeyPressed('s')) {
		if (current_view == rat_view) {
			tilt += .1;
		}
	}


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (current_view == perspective_view)
	{
		glEnable(GL_DEPTH_TEST);
		glLoadIdentity();
		//gluLookAt(-10, -10, 30, 50, 50, 0, 0, 0, 1);
		gluLookAt( gRat.getX() - 10, gRat.getY() - 10, 20, gRat.getX(), gRat.getY(), 0, 0, 0, 1);
	}
	else if (current_view == top_view)
	{
		glDisable(GL_DEPTH_TEST);
		glLoadIdentity();
	}
	else // current_view == rat_view
	{
		glEnable(GL_DEPTH_TEST);
		glLoadIdentity();

		//double zratio =  .9*sin(getTime()) / 1 - 0; 
		//double z_level = .5*sin(getTime()) * zratio;

		double z_constant = .30;
		
		
		double x = gRat.getX();
		double y = gRat.getY();
		double z = generateZLevel(x, y) + z_constant;
		if (z < WaterHeight) {
			z = WaterHeight + z_constant;
		}
		double dx = gRat.getDx();
		double dy = gRat.getDy();
		
		double at_x = x + dx;
		double at_y = y + dy;

		double z_level = generateZLevel(at_x, at_y) + z_constant;
		if (z_level < WaterHeight) {

			z_level = WaterHeight + z_constant;

		}
		at_z =  z_level - tilt;


		/*std::vector<double> up = { 0, 0, 1 };
		up = normalize(up);
		std::vector<double> forward = { at_x, at_y, at_z};
		forward = normalize(forward);
		std::vector<double> right = CrossProduct(forward, up);
		right = normalize(right);
		std::vector<double> new_up = CrossProduct(right, forward);
		new_up = normalize(new_up);*/

		
		gluLookAt(x, y, z, at_x, at_y, at_z, 0, 0, 1);
		//gluLookAt(x, y, z_level, at_x, at_y, new_up[2], 0, 0, 1);
	}

	glColor3ub(100,100,255);
	gTerrain.Draw();
	//DrawRectangle(5, 5, 10, 10);

	gRat.Draw();

	glutSwapBuffers();
	glutPostRedisplay();
	
}





// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyboard(unsigned char c, int x, int y)
{
	switch (c) 
	{
		case 'r':
			current_view = rat_view;
			SetPerspectiveView(screen_x, screen_y);
			break;
		case 'p':
			current_view = perspective_view;
			SetPerspectiveView(screen_x, screen_y);
			break;
		case 't':
			current_view = top_view;
			SetTopView(screen_x, screen_y);
			break;
		default:
			return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{
	screen_x = w;
	screen_y = h;
	glViewport(0, 0, w, h);

	if(current_view == top_view)
	{
		SetTopView(w,h);
	}
	else if(current_view == perspective_view)
	{
		SetPerspectiveView(w,h);
	}
	else // current_view == rat_view
	{
		SetPerspectiveView(w,h);
	}
}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y)
{
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
		gLeftButtonDown = true;
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
		gLeftButtonDown = false;
	}


	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		gRightButtonDown = true;
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		gRightButtonDown = false;
	}

	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
		gMiddleButtonDown = true;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
		gMiddleButtonDown = false;
	}
	glutPostRedisplay();
}

// Your initialization code goes here.
void InitializeMyStuff()
{
	InitKeyboard();
	gRat.InitRat(50, 50, 5, 90, gTerrain);
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(50, 50);

	int fullscreen = 0;
	if (fullscreen) 
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} 
	else 
	{
		glutCreateWindow("Terrain Manipulation");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	glColor3d(0,0,0); // forground color
	glClearColor(1, 1, 1, 0); // background color
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}
