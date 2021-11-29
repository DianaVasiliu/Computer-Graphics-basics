/* RECTANGLE WITH SATELLITE -- implemented with "new" OpenGL
	- uses different transformations and their composition, using the glm library
	- mouse functions
	- using uniform variables in the shader for colouring the primitives
*/

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "loadShaders.h"

#include "glm/glm/glm.hpp"  
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

using namespace std;
  
//////////////////////////////////////

GLuint
  VaoId,
  VboId,
  ColorBufferId,
  ProgramId,
  myMatrixLocation,
  colCodeLocation;
 
glm::mat4 myMatrix, resizeMatrix, matrTransl, matrScale1, matrScale2, matrRot, matrMove; 

int colCode;
float PI = 3.141592, angle = 0;
float tx = 0; float ty = 0; 
int width = 450, height = 300;
float i = 0.0, alpha = 0.0, beta = 0.2; 

void moveRight(void)
{
    i = i + alpha;
    if (i > 350.0)
        alpha = -10.0;
    else if (i < -350.0)
        alpha = 10.0;
    
    angle = angle - beta;

    glutPostRedisplay();
}

void moveLeft(void)
{   
    i = i + alpha;
    if (i < -350.0)
        alpha = 10.0;
    else if (i > 350.0)
        alpha = -10.0;
  
    angle = angle + beta;

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) 
{
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
            alpha = -10.0; 
        glutIdleFunc(moveLeft);
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
            alpha = 10.0; 
        glutIdleFunc(moveRight);
        break;
    default:
        break;
    }
}

void CreateVBO(void)
{
    GLfloat Vertices[] = {
        // vertices for the axes
	    -450.0f, 0.0f, 0.0f, 1.0f,
        450.0f, 0.0f, 0.0f, 1.0f,
        0.0f, -300.0f, 0.0f, 1.0f,
	    0.0f, 300.0f, 0.0f, 1.0f,
	    // verices for the rectangle
        -50.0f, -50.0f, 0.0f, 1.0f,
        50.0f, -50.0f, 0.0f, 1.0f,
	    50.0f, 50.0f, 0.0f, 1.0f,
	    -50.0f, 50.0f, 0.0f, 1.0f,
    };
   
    GLfloat Colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
	    1.0f, 0.0f, 0.0f, 1.0f,
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
    ProgramId = LoadShaders("05_02_Shader.vert", "05_02_Shader.frag");
    glUseProgram(ProgramId);
}
 
void DestroyShaders(void)
{
    glDeleteProgram(ProgramId);
}
 
void Initialize(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}

void RenderFunction(void)
{
    // TODO: change the transformations (for example, move the rectangle on Oy or another axis)
    resizeMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f/width, 1.f/height, 1.0)); // scaling, we bring the scene to the "standard rectangle" [-1,1]x[-1,1]
    matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(i, 0.0, 0.0)); // controls the translation along Ox
    matrMove = glm::translate(glm::mat4(1.0f), glm::vec3(0, 80.0, 0.0));  // placing the red square
    matrScale1 = glm::scale(glm::mat4(1.0f), glm::vec3(1.1, 0.3, 0.0));   // used to draw the blue rectangle
    matrScale2 = glm::scale(glm::mat4(1.0f), glm::vec3(0.25, 0.25, 0.0)); // used to draw the red quare
    matrRot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0, 0.0, 1.0));  // used to rotate the red quare

    glClear(GL_COLOR_BUFFER_BIT);
    CreateVBO();

    // resize matrix for the "fixed" elements
    myMatrix = resizeMatrix;  

    // creating the shaders + sending the uniform variables
    CreateShaders();
    myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);
  
    // drawing the axes
    glDrawArrays(GL_LINES, 0, 4);

    // the matrix for the blue rectangle
    myMatrix = resizeMatrix * matrTransl * matrScale1;

    // creating the shaders + sending the uniform variables
    CreateShaders();
    myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix"); 
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);
  
    colCode = 1;
    colCodeLocation = glGetUniformLocation(ProgramId, "colCode");
    glUniform1i(colCodeLocation, colCode);
  
    // drawing the rectangle
    glDrawArrays(GL_POLYGON, 4, 4);
  
    // the matrix for the red square
    myMatrix = resizeMatrix *  matrTransl * matrRot * matrMove * matrScale2;

    // creating the shaders + sending the uniform variables
    CreateShaders();
    myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix"); 
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);
    
    colCode = 2;
    colCodeLocation = glGetUniformLocation(ProgramId, "colCode");
    glUniform1i(colCodeLocation, colCode);
    
    // drawing the square
    glDrawArrays(GL_POLYGON, 4, 4);

    // TODO: add another elements
   
    glutSwapBuffers();
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100, 100); 
    glutInitWindowSize(800, 600); 
    glutCreateWindow("Transformations composition. Using the mouse"); 
    glewInit(); 

    Initialize();
    glutDisplayFunc(RenderFunction);
    glutMouseFunc(mouse);
    glutCloseFunc(Cleanup);
    glutMainLoop(); 
}
