/* RECTANGLE WITH SATELLITE -- a first animation
NEW ELEMENTS:
	- gluOrtho2D (indicates the rectangle that must be cropped - for the window to render)
	- glTranslate, glRotate, glPushMatrix, glPopMatrix (for transformations; DEPRECATED)
	- glutSwapBuffers (GLUT_DOUBLE); glutPostRedisplay; glutIdleFunc (animation)
*/

#include<windows.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <math.h>

static GLdouble i = 0.0;
static GLdouble j = 0.0;
static GLdouble alpha = 3.0, initialAlpha = 3.0;

void init(void) 
{
   glClearColor (1.0, 1.0, 1.0, 0.0);
   gluOrtho2D (0.0, 800.0, 0.0, 600.0);
}

void rectangle(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
   
    glPushMatrix();
    glTranslated(i, 200.0, 0.0);
   
    glPushMatrix();
    glRotated(j, 0.0, 0.0, 1.0);
   
    glColor3f(1.0, 0.0, 0.0);
    glRecti(-5, 30, 5, 40);
    // glPopMatrix();
    glPopMatrix();
    
    // glPushMatrix();
    // glTranslated(i, 200.0, 0.0);

    glColor3f(0.0, 0.0, 1.0);
    glRecti(-20, -12, 20, 12);
    glPopMatrix();
    
    glutSwapBuffers();
    glFlush ( );
}
 
void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   glOrtho(-20.0, 780.0, 0.0, 600.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void moveRight(void)
{
    i = i + alpha;
    if (i > 750.0)
        alpha = -initialAlpha;
    else if (i < 0.0)
        alpha = initialAlpha;
    
    j = j + initialAlpha;

    glutPostRedisplay ( );
}

void moveLeft(void)
{   
    i = i + alpha;
    if (i < 0.0)
        alpha = initialAlpha;
    else if (i > 750.0)
		alpha = -initialAlpha;
  
    j = j + initialAlpha;

    glutPostRedisplay ( );
}

void mouse(int button, int state, int x, int y) 
{
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
            alpha = -initialAlpha; glutIdleFunc(moveLeft);
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
            alpha = initialAlpha; glutIdleFunc(moveRight);
        break;
    default:
        break;
    }
}

/* 
 *  Request double buffer display mode.
 *  Register mouse input callback functions
 */
void main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600); 
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Rectangle with satellite");
    
    init();
    glutDisplayFunc(rectangle); 
    glutReshapeFunc(reshape); 
    glutMouseFunc(mouse);
    glutMainLoop();
}
