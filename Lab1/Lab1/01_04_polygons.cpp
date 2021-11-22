#include <windows.h>
#include <GL/freeglut.h>

void init(void)  // initializing the window
{
	glClearColor(1.0, 1.0, 1.0, 0.0); // background color of the window

    glMatrixMode(GL_PROJECTION);  // specify it is a 2D projection, made by orthogonal projection
	gluOrtho2D(0, 800.0, 0.0, 700.0); // the extreme coordinates of the window
}

void draw(void) // the drawing function
{
	// rectangle: direct drawing
	glColor3f(0.0, 0.7, 1.0);
	glRecti(20, 130, 140, 310);	 
 
    // convex polygon
	glColor3f(1.0, 6.0, 0.0);  
	glBegin(GL_POLYGON);  
		glVertex2i(0,0);
		glVertex2i(100, 10);
		glVertex2i(200, 120);
		glVertex2i(155, 290);
	glEnd();

	// triangle fan
	glColor3d(1.0, 0.0, 0.5);  
		glBegin(GL_TRIANGLE_FAN);
		glVertex2i(250, 300);
		glVertex2i(50, 300);
		glVertex2i(200, 350);
		glVertex2i(250, 500);
		glVertex2i(350, 200);
	glEnd();

	// triangle strip
	glColor3d(1.0, 0.9, 0.6);  
		glBegin(GL_TRIANGLE_STRIP);
		glVertex2i(650, 300);
		glVertex2i(450, 300);
		glVertex2i(600, 350);
		glVertex2i(650, 500);
		glVertex2i(750, 200);
	glEnd();

	glFlush(); // processes OpenGL procedures as fast as possible
}

void draw2(void)
{
	glColor3f(1, 0, 0);
	glRecti(-30, 20, 40, 10);
	
	glColor3f(1,1,0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(-10.0, -20.0, 0.0);
	glScalef(0.5, 2.0, 0.0);
	glRecti(-30, 20, 40, 10);
	glPopMatrix();

	glPointSize(4.0);
	glColor3f(0, 0, 0);
	glBegin(GL_POINTS);
	glVertex3f(40, 20, 0);
	glEnd();

	glPointSize (8.0);
	glColor3f(0, 0, 0);
	glBegin(GL_POINTS);
	glVertex3f(10, 20, 0);
	glEnd();

	glFlush(); 
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv); // initializing GLUT
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // a single buffer is used | RedGreenBlue coloring mode (= default)
	glutInitWindowPosition(100, 100); // initial position of the viewport (in screen coordinates)
	glutInitWindowSize(800, 600); // window dimensions
	glutCreateWindow("Polygons"); // creates the window, indicating the name of the viewport - appears at the top

	init(); // perform the initialization procedure
	glClear(GL_COLOR_BUFFER_BIT); // representation and coloring of the viewing window
	glutDisplayFunc(draw); // the drawing procedure is invoked whenever necessary
	// glutDisplayFunc(draw2);
	glutMainLoop(); // the last instruction of the program, waiting for (possible) new input data
}
