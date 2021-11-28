 /*
NEW ELEMENTS:
- GL_QUADS as a primitive drawing mode 
- "old" OpenGL functions:
	gluLookAt
	glOrtho
	glMatrixMode
	// generate transformations for 3D visualization 
	glPolygonMode
- the "reshape" function, to keep the drawing proportions
*/

#include <windows.h>
#include <GL/freeglut.h>

GLint winWidth = 600, winHeight = 600;
GLfloat x0 = 0.0, y0 = 0.0, z0 = 0.0;
GLfloat xref = 0.0, yref = 0.0, zref = 40.0;
GLfloat Vx = 0.0, Vy = 1.0, Vz = 0.0;
GLfloat xwMin = -20.0, ywMin = -20.0, xwMax = 20.0, ywMax = 20.0;
GLfloat dnear = 0.0, dfar = 60.0;

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	gluLookAt(x0, y0, z0, xref, yref, zref, Vx, Vy, Vz);

	glMatrixMode(GL_PROJECTION);
	glOrtho(xwMin, xwMax, ywMin, ywMax, dnear, dfar);
}

void draw1(void)
{
	glClear (GL_COLOR_BUFFER_BIT);
 
	glLineWidth(4.0);
	glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);

	glBegin(GL_QUADS);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3i(5.0f, -5.0f, 5.0f);
		glVertex3i(-5.0f, -5.0f, 5.0f);
		glVertex3i(-5.0f, 5.0f, 5.0f);
		glVertex3i(5.0f, 5.0f, 5.0f);
		glColor3f(0.0f, 0.0f, 0.5f);
		glVertex3i(3.0f, 3.0f, 10.0f);
		glVertex3i(-3.0f, 3.0f, 10.0f);
		glVertex3i(-3.0f, -3.0f, 10.0f);
		glVertex3i(3.0f, -3.0f, 10.0f);
	glEnd();

	glFlush();
}

void draw2(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
 
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	
	glBegin(GL_QUADS);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3i(5.0f, -5.0f, 5.0f);
		glVertex3i(-5.0f, -5.0f, 5.0f);
		glVertex3i(-5.0f, 5.0f, 5.0f);
		glVertex3i(5.0f, 5.0f, 5.0f);
		glColor3f(0.0f, 0.0f, 0.5f);
		glVertex3i(3.0f, 3.0f, 10.0f);
		glVertex3i(-3.0f, 3.0f, 10.0f);
		glVertex3i(-3.0f, -3.0f, 10.0f);
		glVertex3i(3.0f, -3.0f, 10.0f);
	glEnd();
	
	glDisable(GL_CULL_FACE);
	glFlush();
}

void reshapeFnc(GLint newWidth, GLint newHeight)
{
	glViewport (0, 0, newWidth,newHeight);

	winWidth = newWidth;
	winHeight = newHeight;
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("3D Polygons");

	init();
	glutDisplayFunc(draw1);
	glutReshapeFunc(reshapeFnc);
	glutMainLoop();
}
