#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "components/car.cpp"
#include "components/house.cpp"
#include "components/tree.cpp"
#include "texture.cpp"
using namespace std;

static float angle = 0.0, ratio;
static float x = 0.0f, y = 3.00f, z = 180.0f;
static float lx = 0.0f, ly = 0.03f, lz = -1.0f;
float theta = 0.01, fxincr = 0.1, fzincr = 0, temp, theta1, fx = -10, fz = 100;
static GLint car_display_list, house_display_list, tree_display_list, tree_night_display_list, tree_day_display_list, house_night_display_list;
/*	housevisible -> to make house visible
	movecarvar-> to enable or disable car movement
 	treevisible-> to make tree visible */
int xxxx = 0, yyyy = 0, housevisible = 1, movecarvar = 0, treevisible = 1;

/* 	instruct, movei, showi -> for different instruction views
	night -> for enable or disable night view */
int instruct = 1, movei = 0, showi = 1, night = 0;

// For colour of cars
int a[3] = {55, 97, 44};
int a_night[3] = {15, 30, 13};
int b[3] = {102, 194, 127};
int b_night[3] = {35, 65, 45};
int c[3] = {159, 243, 133};
int c_night[3] = {52, 75, 46};

void changeSize(int w, int h)
{
	if (h == 0)
		h = 1;
	ratio = 1.0f * w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45, ratio, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

//Function to show text on screen
void PrintString(string s, int x, int y, int r, int g, int b)
{
	int next_line = 0;
	glRasterPos2i(x, y);
	int len = s.length();
	for (int i = 0; i < len; i++)
	{
		char c = s[i];
		if (c == '\n')
		{
			next_line += 20;
			glRasterPos2i(x, y - next_line);
		}
		else
		{
			glColor3d(r, g, b);
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
		}
	}
}

//Function create car
GLuint createDL()
{
	GLuint carDL;
	carDL = glGenLists(1);		  // Create the id for the list
	glNewList(carDL, GL_COMPILE); // start list
	drawcar();					  // call the function that contains the rendering commands
	glEndList();				  // endList
	return (carDL);
}

//Function create house
GLuint createDL2()
{
	GLuint houseDL;
	houseDL = glGenLists(1);		// Create the id for the list
	glNewList(houseDL, GL_COMPILE); // start list
	drawhouse();					// call the function that contains the rendering commands
	glEndList();					// endList
	return (houseDL);
}

GLuint createnightDL2() //******************
{
	GLuint houseDL;
	houseDL = glGenLists(1);		// Create the id for the list
	glNewList(houseDL, GL_COMPILE); // start list
	drawnighthouse();				// call the function that contains the rendering commands
	glEndList();					// endList
	return (houseDL);
}

//Function create tree of type 1
GLuint createDL3()
{
	GLuint treeDL;
	treeDL = glGenLists(1);		   // Create the id for the list
	glNewList(treeDL, GL_COMPILE); // start list
	makeTree(4, 0.5);			   // call the function that contains the rendering commands
	glEndList();				   // endList
	return (treeDL);
}

//Function create tree of type 2
GLuint createDL4()
{
	GLuint treeDL;
	treeDL = glGenLists(1);		   // Create the id for the list
	glNewList(treeDL, GL_COMPILE); // start list
	makeTree1(4, 0.5);			   // call the function that contains the rendering commands
	glEndList();				   // endList
	return (treeDL);
}

void initScene()
{
	glEnable(GL_DEPTH_TEST);
	car_display_list = createDL();
	house_display_list = createDL2();
	house_night_display_list = createnightDL2();
	tree_day_display_list = createDL3();
	tree_night_display_list = createDL4();
}

//for different mode of tree
void treeMode()
{
	if (night)
		tree_display_list = tree_night_display_list;
	else
		tree_display_list = tree_day_display_list;
}

//Function to render text on screen
void renderStrings()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, 1080, 0.0, 700);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	PrintString("IITI CG project", 10, 660, 1.0, 0.0, 1.0);
	PrintString("Students:\nAshwini Jha\nJhalak Gupta", 10, 640, 1.0, 0.0, 1.0);

	if (instruct == 1 && showi == 1)
		PrintString("Instructions \nm - to move car \nUp key - to move camera forward\nDown key - to move camera backward\nLeft key - to rotate camera to the left\nRight key - to rotate camera to the right\nt - top view\na - to move left\nd - to move right\nw - to zoom in\ns - to zoom out\nn - toggle day/night mode\ni - to show or hide instructions\nq - quit", 750, 660, 1.0, 1.0, 0.1);

	if (movei == 1 && showi == 1)
		PrintString("If you wish to move the car, \nUp key - to move car forwards\nDown key - to move car backwards\nLeft key - to rotate car to the left\nRight key - to rotate car to the right \nm - to stop car movement\nn - toggle day/night mode\ni - to show or hide instructions", 750, 660, 1.0, 0.5, 0.5);
	if (showi == 0)
		PrintString("Press 'i' to show instructions", 750, 690, 1.0, 1.0, 0.5);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

//Function to render all the components of the scene
void renderScene(void)
{
	int i, j;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(.7, 0.85, 1.0, 1.0);
	glPushMatrix();
	if (night == 0)
	{
		glColor4f(0.8, 0.8, 0.8, 1.0);
		drawClouds();
		drawClouds1();
		drawSky();
	}
	else
	{
		glColor4f(0.8, 0.8, 0.8, 1.0);
		drawNightClouds();
		drawNightClouds1();
		drawNightSky();
	}
	glPopMatrix();
	glPushMatrix();
	if (night == 0)
	{
		glColor4f(0.2, 0.35, 0.5, 1.0);
	}
	else
	{
		glColor4f(0.05, 0.20, 0.35, 1.0);
	}
	drawFloor();
	glPopMatrix();
	glPushMatrix();
	if (night == 0)
		glColor4f(0.3, 0.3, 0.3, 1.0);
	else
	{
		glColor4f(0.15, 0.15, 0.15, 1.0);
	}
	if (night == 0)
		drawRoad();
	else
		drawNightRoad();
	glPopMatrix();
	renderStrings();

	//for house
	if (housevisible)
	{
		glPushMatrix();
		glScalef(2.0, 2.0, 2.0);
		glTranslatef(-8.0, 1.0, 70.0);
		glRotatef(90, 0, 1, 0);
		if (night == 0)
			glCallList(house_display_list);
		else
			glCallList(house_night_display_list);
		glPopMatrix();
	}

	//for trees
	if (treevisible)
	{
		treeMode();
		glPushMatrix();
		glTranslatef(-10.0, 0.0, 80.0);
		glRotatef(170, 0, 1, 0);
		glCallList(tree_display_list);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-11.0, 0.0, 100.0);
		glRotatef(180, 0, 1, 0);
		glCallList(tree_display_list);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-14.0, 0.0, 90.0);
		glRotatef(190, 0, 1, 0);
		glCallList(tree_display_list);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-12.0, 0.0, -50.0);
		glRotatef(180, 0, 1, 0);
		glCallList(tree_display_list);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-14.0, 0.0, -60.0);
		glRotatef(195, 0, 1, 0);
		glCallList(tree_display_list);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-12.0, 0.0, -20.0);
		glRotatef(180, 0, 1, 0);
		glCallList(tree_display_list);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-14.0, 0.0, -10.0);
		glRotatef(175, 0, 1, 0);
		glCallList(tree_display_list);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(11.0, 0.0, -10.0);
		glRotatef(200, 0, 1, 0);
		glCallList(tree_display_list);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(15.0, 0.0, 0.0);
		glRotatef(190, 0, 1, 0);
		glCallList(tree_display_list);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(12.0, 0.0, 10.0);
		glRotatef(180, 0, 1, 0);
		glCallList(tree_display_list);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(14.0, 0.0, 20.0);
		glRotatef(160, 0, 1, 0);
		glCallList(tree_display_list);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(14.0, 0.0, 80.0);
		glRotatef(160, 0, 1, 0);
		glCallList(tree_display_list);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(11.0, 0.0, 100.0);
		glRotatef(180, 0, 1, 0);
		glCallList(tree_display_list);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(15.0, 0.0, 120.0);
		glRotatef(190, 0, 1, 0);
		glCallList(tree_display_list);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(12.0, 0.0, 130.0);
		glRotatef(170, 0, 1, 0);
		glCallList(tree_display_list);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(11.0, 0.0, 155.0);
		glRotatef(175, 0, 1, 0);
		glCallList(tree_display_list);
		glPopMatrix();
	}

	//for cars
	for (i = -1; i <= 1; i = i + 2)
	{
		for (j = -1; j <= 1; j++)
		{
			glPushMatrix();
			glScalef(1.5, 1.5, 1.5);
			glTranslatef((i)*3.0, 0, (3 * j) * 10.0);
			if (i == -1)
			{
				glRotatef(90, 0, 1, 0);
			}
			else
			{
				glRotatef(-90, 0, 1, 0);
			}
			if (night == 0)
				glColor3ub(a[i + 1], b[j + 1], c[i + 1]);
			else
				glColor3ub(a_night[i + 1], b_night[j + 1], c_night[i + 1]); // change colour of cars fron here
			glCallList(car_display_list);
			glPopMatrix();
		}
	}
	if (fxincr != 0)
	{
		theta1 = (atan(fzincr / fxincr) * 180) / 3.141;
	}

	else if (fzincr > 0)
	{
		theta1 = -90.0;
	}

	else
	{
		theta1 = 90.0;
	}

	if (fxincr > 0 && fzincr < 0)
	{
		theta1 = -theta1;
	}
	else if (fxincr < 0 && fzincr < 0)
	{
		theta1 = 180 - theta1;
	}
	else if (fxincr < 0 && fzincr > 0)
	{
		theta1 = -180 - theta1;
	}
	else if (fxincr > 0 && fzincr > 0)
	{
		theta1 = -theta1;
	}

	glPushMatrix();
	glScalef(1.5, 1.5, 1.5);
	glTranslatef(fx, 0, fz);
	glRotatef(theta1, 0, 1, 0);
	if (night == 0)
		glColor3f(0.8, 0.7, 0.1); //yellow color car
	else
		glColor3f(0.4, 0.35, 0.05);

	glCallList(car_display_list);
	glPopMatrix();
	glutSwapBuffers();
}

void orientMe(float ang)
{
	lx = sin(ang);
	lz = -cos(ang);
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

void moveMeFlat(int i)
{
	if (xxxx == 1)
		y = y + i * (lz)*0.1;

	if (yyyy == 1)
	{
		x = x + i * (lz)*.1;
	}
	else
	{
		z = z + i * (lz)*0.5;
		x = x + i * (lx)*0.5;
	}
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

//For camera movement
void inputKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		angle -= 0.05f;
		orientMe(angle);
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.05f;
		orientMe(angle);
		break;
	case GLUT_KEY_UP:
		moveMeFlat(2);
		xxxx = 0, yyyy = 0;
		break;
	case GLUT_KEY_DOWN:
		moveMeFlat(-2);
		xxxx = 0, yyyy = 0;
		break;
	}
}

//For movement of car
void moveCar(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		temp = fxincr;
		fxincr = fxincr * cos(theta) + fzincr * sin(theta);
		fzincr = -temp * sin(theta) + fzincr * cos(theta);
		fx += fxincr;
		fz += fzincr;
		break;
	case GLUT_KEY_RIGHT:
		temp = fxincr;
		fxincr = fxincr * cos(-theta) + fzincr * sin(-theta);
		fzincr = -temp * sin(-theta) + fzincr * cos(-theta);
		fx += fxincr;
		fz += fzincr;
		break;
	case GLUT_KEY_UP:
		fx += fxincr;
		fz += fzincr;
		break;
	case GLUT_KEY_DOWN:
		fx -= fxincr;
		fz -= fzincr;
		break;
	}
	glutPostRedisplay();
}

//For other keys functions
void processNormalKeys(unsigned char key, int x, int y)
{
	if (key == 'n')
	{
		if (night == 1)
		{
			night = 0;
		}
		else
		{
			night = 1;
		}
		glutPostRedisplay();
	}

	if (key == 'm')
	{
		if (movecarvar == 0)
		{
			movei = 1;
			instruct = 0;
			showi = 1;
			glutSpecialFunc(moveCar);
			movecarvar = 1;
		}
		else
		{
			movei = 0;
			instruct = 1;
			showi = 1;
			glutSpecialFunc(inputKey);
			movecarvar = 0;
		}
	}
	if (key == 'q')
		exit(0);
	if (key == 'i')
	{
		if (showi == 1)
			showi = 0;
		else
		{
			showi = 1;
		}
		glutPostRedisplay();
	}
	if (key == 't')
	{
		glLoadIdentity();
		gluLookAt(0, 190, 30, 0, 0, 0, 0.0, 1.0, .0);
	}
	if (key == 'a')
		moveMeFlat(4);
	xxxx = 1, yyyy = 0;
	if (key == 's')
		moveMeFlat(-4);
	xxxx = 1, yyyy = 0;
	if (key == 'w')
		moveMeFlat(4);
	yyyy = 1;
	xxxx = 0;
	if (key == 'd')
		moveMeFlat(-4);
	yyyy = 1;
	xxxx = 0;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1920, 1080);
	glutCreateWindow("Car");
	initScene();
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(inputKey);
	textures();
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutMainLoop();
	return (0);
}