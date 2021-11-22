// The source code is adapted after OpenGLBook.com

#include <windows.h>
#include <GL/freeglut.h>  
 
void Initialize(void)
{
	 // the vertices
    GLfloat Vertices[] = {
        -0.8f, -0.8f, 0.0f, 1.0f,
        0.0f,  0.8f, 0.0f, 1.0f,
        0.8f, -0.8f, 0.0f, 1.0f
    };

    // the colors, as attributes of the vertices
    GLfloat Colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f
    };
  
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // background color
}

void RenderFunction(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(20.0);
    glBegin(GL_LINE_LOOP);
  
    // the first vertex
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    glVertex4f(-0.8f, -0.8f, 0.0f, 1.0f);
  
    // the second vertex
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    glVertex4f(0.0f,  0.8f, 0.0f, 1.0f);

    // the third vertex
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    glVertex4f(0.8f, -0.8f, 0.0f, 1.0f);
    
    glEnd();
    glFlush();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition (100,100); // initial position of the window
    glutInitWindowSize(500,350); // window dimenstions
    glutCreateWindow("The first triangle - OpenGL << old >>"); // window title
    Initialize();
    glutDisplayFunc(RenderFunction);
    glutMainLoop();
}
