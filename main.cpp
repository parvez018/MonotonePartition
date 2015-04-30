#include <windows.h>  // For MS Windows
#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h

#include "monotone.h" //
#include "testing.h" //

using namespace std;
double screenWidth = 900;
double screenHeight = 600;

double leftX = 0;
double topY = 0;

double mulX = 0.09;
double mulY = 0.09;
double delta = 1.0;

MonotonePartition mp;

void specialKeyListener(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_DOWN: //down arrow key
		topY -= delta;
		break;
	case GLUT_KEY_UP: // up arrow key
		topY += delta;
		break;

	case GLUT_KEY_RIGHT:
		leftX += delta;
		break;
	case GLUT_KEY_LEFT:
		leftX -= delta;
		break;

	case GLUT_KEY_PAGE_UP:
		if (mulX <= 1.0) {
			mulX += 0.1;
			mulY += 0.1;
		}
		break;
	case GLUT_KEY_PAGE_DOWN:
		//angle-=1;
		break;

	case GLUT_KEY_INSERT:
		if (mulX >= 0.0) {
			mulX -= 0.1;
			mulY -= 0.1;
		}
		break;

	case GLUT_KEY_HOME:
		//angle-=1;
		break;
	case GLUT_KEY_END:
		break;

	default:
		break;
	}
	glutPostRedisplay();
}

void drawAxes() {
    glColor3f(0,0,0);
	glBegin(GL_LINES);
	{
		//glColor3f(1, 0, 0);
		glVertex2f(leftX - screenWidth / 2.0, 0);
		glVertex2f(leftX + screenWidth / 2.0, 0);

		//glColor3f(0, 1, 0);
		glVertex2f(0, topY - screenHeight / 2.0);
		glVertex2f(0, topY + screenWidth / 2.0);
	}
	glEnd();
}

void display() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

	glMatrixMode(GL_PROJECTION); // Select the Projection matrix for operation
	glLoadIdentity(); // Reset Projection matrix
	gluOrtho2D((leftX - screenWidth * mulX / 2.0),
			(leftX + screenWidth * mulX / 2.0),
			(topY - screenHeight * mulY / 2.0),
			(topY + screenHeight * mulY / 2.0));



	drawAxes();
	//glColor3f(0,0,0);
	mp.draw();
	//drawChar(0,0,"hello");

	glFlush(); // Render now
}

void initGL() {
	glViewport(0, 0, screenWidth, screenHeight);
	//priority_queue_test();
	//stlSetTest();
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitWindowSize((int)screenWidth, (int)screenHeight);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Monotone Partition");
	initGL();

	glMatrixMode(GL_PROJECTION);

	glutDisplayFunc(display);
	glutSpecialFunc(specialKeyListener);

	glutMainLoop();
    return 0;
}
