// The source code is adapted after OpenGLBook.com

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "loadShaders.h"
  
//////////////////////////////////////

GLuint
  VaoId,
  VboId,
  ColorBufferId,
  ProgramId;

//////////////////////////////////////
 
void CreateVBO(void)
{
    GLfloat Vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f,
	    -0.5f, 0.5f, 0.0f, 1.0f,
	 
        -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f,
	    0.5f, -0.5f, 0.0f, 1.0f
    };

    GLfloat Colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
	    1.0f, 1.0f, 0.0f, 1.0f,
	
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.5f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,
	    0.0f, 0.0f, 0.0f, 1.0f
    };
 
    // creating a new buffer
    glGenBuffers(1, &VboId);

    // setting the buffer as the current buffer
    glBindBuffer(GL_ARRAY_BUFFER, VboId);

    // the vertices are "copied" in the current buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    // creating / linking a VAO (Vertex Array Object) - useful when using more VBOs
    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);

    // activating working with attributes
    // attribute 0 = position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // a new buffer, for colors
    glGenBuffers(1, &ColorBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);

    // attribute 1 = color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);  
 }

void DestroyVBO(void)
{
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &ColorBufferId);
    glDeleteBuffers(1, &VboId);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VaoId);   
}

void CreateShaders(void)
{
    ProgramId = LoadShaders("03_03_Shader.vert", "03_03_Shader.frag");
    glUseProgram(ProgramId);
}

void DestroyShaders(void)
{
    glDeleteProgram(ProgramId);
}
 
void Initialize(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    CreateVBO();
    CreateShaders();
}

void RenderFunction(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glShadeModel(GL_FLAT);

    glPointSize(20.0);
    glLineWidth(10.0);

    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
 
    glPolygonMode(GL_FRONT, GL_LINE);
    glPolygonMode(GL_BACK, GL_FILL);
  
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

    glFlush();
}

void Cleanup(void)
{
    DestroyShaders();
    DestroyVBO();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 400);
    glutCreateWindow("Polygon face");
    glewInit();

    Initialize();
    glutDisplayFunc(RenderFunction);
    glutCloseFunc(Cleanup);
    glutMainLoop();
}
