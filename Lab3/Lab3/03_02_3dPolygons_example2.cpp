/*
NEW ELEMENTS:
- indicating the vertices in arrays
- using the mouse function glutMouseFunc 
*/

#include <windows.h>
#include <gl/freeglut.h>

GLint winWidth = 600, winHeight = 600;
GLfloat xx0 = 13.0, yy0 = 11.0, zz0 = 14.0;
GLfloat xref = 3.0, yref = 1.0, zref = 4.0;
GLfloat Vx = 8.0, Vy = 0.0, Vz = -8.0;
GLfloat xwMin = -10.0, ywMin = -10.0, xwMax = 10.0, ywMax = 10.0;
GLfloat dnear = 0.0, dfar = 20.0;

int p1[] = {6, 2, 0};
int p2[] = {-4, 4, 8};
int p3[] = {0, 0, 8};
int p4[] = {2, 2, 4};
int p5[] = {10, -2, 0};

float col1[] = { 1.0, 0.0, 0.0 };
float col2[] = { 0.0, 1.0, 0.0 };
float col3[] = { 0.0, 0.0, 1.0 };
float col4[] = { 1.0, 1.0, 0.0 };

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(xx0, yy0, zz0, xref, yref, zref, Vx, Vy, Vz);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(xwMin, xwMax, ywMin, ywMax, dnear, dfar);
}

// p1p2p3p4 polygon
void draw1(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	 
	glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
	glLineWidth(6.0);
	glBegin(GL_POLYGON);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3iv(p1);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3iv(p2);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3iv(p4);
		glColor3f(1.0, 1.0, 0.0);
		glVertex3iv(p3);
	glEnd();
	
	glFlush();
}

// p1p2p3p5 polygon
void draw2(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
	glLineWidth(6.0);
	glBegin(GL_POLYGON);
		glColor3f(col1[0], col1[1], col1[2]);
		glVertex3iv(p1);
		glColor3f(col2[0], col2[1], col2[2]);
		glVertex3iv(p2);
		glColor3f(col3[0], col3[1], col3[2]);
		glVertex3iv(p3);
		glColor3f(col4[0], col4[1], col4[2]);
		glVertex3iv(p5);
	glEnd();

	glFlush();
}

void mouseFnc(int button, int state, int x, int y) 
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			xx0 = 13.0, yy0 = 11.0, zz0 = 14.0;
			Vy = 0.0;
			col1[0] = 1.0, col1[1] = 0.0, col1[2] = 0.0;
			col2[0] = 0.0, col2[1] = 1.0, col2[2] = 0.0;
			col3[0] = 0.0, col3[1] = 0.0, col3[2] = 1.0;
			col4[0] = 1.0, col4[1] = 1.0, col4[2] = 0.0;
		}
		init();
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			xx0 = -7.0, yy0 = -9.0, zz0 = -6.0;
			Vy = 90.0;
			col1[0] = 1.0, col1[1] = 1.0, col1[2] = 0.0;
			col2[0] = 0.0, col1[1] = 1.0, col2[2] = 1.0;
			col3[0] = 1.0, col3[1] = 0.0, col3[2] = 1.0;
			col4[0] = 1.0, col4[1] = 0.5, col4[2] = 0.0;
		}
		init();
		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN) {
			if (col1[0] == 0.4f && col2[0] == 0.4f && col3[0] == 0.0f && col4[0] == 0.0f) {
				if (xx0 == -7.0) {
					col1[0] = 1.0, col1[1] = 1.0, col1[2] = 0.0;
					col2[0] = 0.0, col2[1] = 1.0, col2[2] = 1.0;
					col3[0] = 1.0, col3[1] = 0.0, col3[2] = 1.0;
					col4[0] = 1.0, col4[1] = 0.5, col4[2] = 0.0;
				}
				else {
					col1[0] = 1.0, col1[1] = 0.0, col1[2] = 0.0;
					col2[0] = 0.0, col2[1] = 1.0, col2[2] = 0.0;
					col3[0] = 0.0, col3[1] = 0.0, col3[2] = 1.0;
					col4[0] = 1.0, col4[1] = 1.0, col4[2] = 0.0;
				}
			}
			else {
				col1[0] = col1[1] = col1[2] = 0.4;
				col2[0] = col2[1] = col2[2] = 0.4;
				col3[0] = col3[1] = col3[2] = 0.0;
				col4[0] = col4[1] = col4[2] = 0.0;
			}
		}
	}
}

void reshapeFnc(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);

	winWidth = newWidth;
	winHeight = newHeight;
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("3D context. The face and the back of a polygon");

	init();
	glutMouseFunc(mouseFnc);
	glutDisplayFunc(draw2);
	glutMainLoop();
}
