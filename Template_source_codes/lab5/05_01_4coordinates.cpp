/* TRANSFORMATIONS. The source code is adapted after OpenGLBook.com
	- we are using two VBOs for the vertex coordinates
    - the transformations and operations with these can be used either in the main program or in the shader
    - we are implementing scaling and translating
*/

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
  ProgramId,
  matrLocation;

float m[4][4];
 
void CreateVBO(void)
{
    GLfloat Vertices[] = {
        // the first 4 vertices have the 4th coordinate equal to 1
        -100, -100, 0, 1,
        100, -100, 0, 1,
        100, 100, 0, 1,
        -100, 100, 0, 1,
        // the 5th vertex (down left) has the 4th coordinate equal to 0
        -100, -100, 0, 0,
        // the next 4 vertices have the 4th coordinate equal to 0.5
        // -100, -100, 0, 0.5,
        100, -100, 0, 0.5,
        100, 100, 0, 0.5,
        -100, 100, 0, 0.5,    
    };
 
    GLfloat Colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
    };
  
    glGenBuffers(1, &VboId);
    glBindBuffer(GL_ARRAY_BUFFER, VboId);
  
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  
    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);
  
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
 
    glGenBuffers(1, &ColorBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  
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
  ProgramId = LoadShaders("05_01_Shader.vert", "05_01_Shader.frag");
  glUseProgram(ProgramId);
}

void DestroyShaders(void)
{
    glDeleteProgram(ProgramId);
}
 
void Initialize(void)
{
    m[0][0] = 0.002; m[1][1] = 0.002; m[2][2] = 1; m[3][3] = 1;
	m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
	m[1][0] = 0; m[1][2] = 0; m[1][3] = 0;
	m[2][0] = 0; m[2][1] = 0; m[2][3] = 0;
	m[3][0] = 0; m[3][1] = 0; m[3][2] = 0;
 
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    CreateVBO();
    CreateShaders();
}

void RenderFunction(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUniformMatrix4fv(matrLocation, 1, GL_TRUE, &m[0][0]);
    glPointSize(20.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glDrawArrays(GL_LINE_LOOP, 4, 4);
    
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
    glutInitWindowSize(600, 600);
    glutCreateWindow("Homogeneous coordinates. Transformations and matrices");
    glewInit();

    Initialize();
    glutDisplayFunc(RenderFunction);
    glutCloseFunc(Cleanup);
    glutMainLoop();
}
