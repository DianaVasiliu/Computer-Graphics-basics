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

//////////////////////////////////////

void CreateVBO1(void)
{
    GLfloat Vertices[] = {
        -0.2f, -0.2f, 0.0f, 1.0f,
        0.0f, 0.2f, 0.0f, 1.0f,
        0.2f, -0.2f, 0.0f, 1.0f,
	    -0.8f, -0.8f, 0.0f, 1.0f,
        0.0f, 0.8f, 0.0f, 1.0f,
        0.8f, -0.8f, 0.0f, 1.0f
    };
   
    GLfloat Colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
	    1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
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

void CreateVBO2(void)
{
    GLfloat Vertices[] = {
        -20.0f, -2.0f, 0.0f, 1.0f,
         0.0f,  2.0f, 0.0f, 1.0f,
         20.0f, -2.0f, 0.0f, 1.0f,
	    -80.0f, -8.0f, 0.0f, 1.0f,
         0.0f,  8.0f, 0.0f, 1.0f,
         80.0f, -8.0f, 0.0f, 1.0f
    };

    GLfloat Colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
	    1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
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
    ProgramId = LoadShaders("04_01_Shader.vert", "04_01_Shader.frag");
    glUseProgram(ProgramId);
}

void DestroyShaders(void)
{
    glDeleteProgram(ProgramId);
}

void Initialize(void)
{
    m[0][0] = 0.01; m[1][1] = 0.1; m[2][2] = 1; m[3][3] = 1;
	m[0][1] = 0; m[0][2] = 0; m[0][3] = 0.4;
	m[1][0] = 0; m[1][2] = 0; m[1][3] = -0.3;
	m[2][0] = 0; m[2][1] = 0; m[2][3] = 0.0f;
	m[3][0] = 0; m[3][1] = 0; m[3][2] = 0;
	
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    CreateVBO1();
    // CreateVBO2();
    CreateShaders();
}

void RenderFunction(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUniformMatrix4fv(matrLocation, 1, GL_TRUE, &m[0][0]);
    glPointSize(20.0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
    glDrawArrays(GL_POINTS, 3, 3);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
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
    glutCreateWindow("Scaling without <<glm>>");
    glewInit();
    Initialize();
    glutDisplayFunc(RenderFunction);
    glutCloseFunc(Cleanup);
    glutMainLoop();
}
