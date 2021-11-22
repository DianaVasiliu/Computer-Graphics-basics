#include <windows.h>
#include <GL/freeglut.h>

void init(void)  // initializing the viewport
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

    glMatrixMode(GL_PROJECTION);   // specify it is a 2D projection, made by orthogonal projection
	gluOrtho2D(0.0, 1200.0, 0.0, 700.0); // the extreme coordinates of the window
}

void draw(void) // the drawing function
{	
	glColor3f(0.0, 0.0, 1.0); // points' color: blue
	{
		glPointSize(1.0);
		glBegin(GL_POINTS); // represents points
			glVertex2i(20, 20);
			glVertex2i(21, 21);
			glVertex2i(22, 22);
			glVertex2i(23, 23);
			glVertex2i(24, 24);
			glVertex2i(27, 27);
			glVertex2i(100, 100);
		glEnd();		
	} 
    
	glColor3d(0.0, 0.05, 0.05);
	// glPointSize (6.0);
	glBegin(GL_POINTS);
	   glVertex2i(100, 400);
	   glColor3f(1.0, 0.0, 0.5);
	   glVertex2i(300, 500);
    glEnd();
	
	glColor3f(1.0, 0.0, 0.0); // the line color: red
    // glLineWidth (2.0);
	// glEnable (GL_LINE_STIPPLE);
	// glLineStipple (1, 0x1EED);
	   glBegin(GL_LINE_STRIP);	// represents a line
       glVertex2i(0,100);
	   glVertex2i(400, 500);
	glEnd();
	// glDisable (GL_LINE_STIPPLE);

    glColor3f(0.5, 0.0, 1.0);  
	// glLineWidth (6.0);
       glBegin(GL_LINES); // represents a region of segments
       glVertex2i(400,400);
	   glVertex2i(600, 500);
	   glVertex2i(700, 520);
	   glVertex2i(655, 690);
	glEnd();

	glFlush(); // processes OpenGL procedures as fast as possible
}

void main (int argc, char** argv)
{
	glutInit(&argc, argv); // initializing GLUT
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // a single buffer is used | RedGreenBlue coloring mode (= default)
	glutInitWindowPosition(100, 100); // initial position of the viewport (in screen coordinates)
	glutInitWindowSize(600, 350); // window dimensions
	glutCreateWindow("Points & Segments"); // creates the window, indicating the name of the viewport - appears at the top

	init(); // perform the initialization procedure
	glClear(GL_COLOR_BUFFER_BIT); // representation and coloring of the viewing window
	glutDisplayFunc(draw); // the drawing procedure is invoked whenever necessary
	glutMainLoop(); // the last instruction of the program, waiting for (possible) new input data
}
