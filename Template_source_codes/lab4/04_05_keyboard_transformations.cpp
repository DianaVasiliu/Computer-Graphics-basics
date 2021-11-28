 /* TRANSFORMATIONS. Creating a 2D scene where elements move
 NEW ELEMENTS:
	- some primitives remain fixed, others change their position
	- keyboard functions: processNormalKeys, processSpecialKeys
	- for the animation: glutIdleFunc
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
 
glm::mat4 myMatrix, resizeMatrix, matrTransl, matrScale, matrRot; 

int colCode;
float PI = 3.141592, angle = 0;
float tx = 0; float ty = 0; 
int width = 400, height = 300;

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key) {
	case 'l':
		angle += 0.2f;
		break;
	case 'r':
		angle -= 0.2f;
		break;
	}

	if (key == 27)
		exit(0);
}

void processSpecialKeys(int key, int xx, int yy) 
{ 
	switch (key) {
	case GLUT_KEY_LEFT :
		tx -= 10;
		break;
	case GLUT_KEY_RIGHT :
		tx += 10;
		break;
	case GLUT_KEY_UP :
		ty += 10;
		break;
	case GLUT_KEY_DOWN :
		ty -= 10;
		break;
	}
}

void CreateVBO(void)
{
	GLfloat Vertices[] = {
		-390.0f, -290.0f, 0.0f, 1.0f,
		390.0f, -290.0f, 0.0f, 1.0f,
		390.0f, 290.0f, 0.0f, 1.0f,
		-390.0f, 290.0f, 0.0f, 1.0f,
		
		-400.0f, 0.0f, 0.0f, 1.0f,
		400.0f,  0.0f, 0.0f, 1.0f,
		0.0f, -300.0f, 0.0f, 1.0f,
		0.0f, 300.0f, 0.0f, 1.0f,
		
		-50.0f, -50.0f, 0.0f, 1.0f,
		50.0f, -50.0f, 0.0f, 1.0f,
		50.0f, 50.0f, 0.0f, 1.0f,
		-50.0f, 50.0f, 0.0f, 1.0f,
		
		0.0f,  0.0f, 0.0f, 1.0f,
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
	ProgramId = LoadShaders("04_03_Shader.vert", "04_03_Shader.frag");
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

	resizeMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f/width, 1.f/height, 1.0));
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, 0.0)); 
	matrRot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0, 0.0, 1.0));

	glClear(GL_COLOR_BUFFER_BIT);
	myMatrix = resizeMatrix;
 
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);
  
	colCode = 0;
	glUniform1i(colCodeLocation, colCode);
	glPointSize(10.0);
	glDrawArrays(GL_POINTS, 0, 4);
	glDrawArrays(GL_LINES, 4, 4);
 
	myMatrix = resizeMatrix * matrTransl * matrRot;

	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix"); 
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);

	colCode=1;
	colCodeLocation = glGetUniformLocation(ProgramId, "colCode");
	glUniform1i(colCodeLocation, colCode);
	
	glDrawArrays(GL_POLYGON, 8, 4);
  
	colCode = 2;
	glUniform1i(colCodeLocation, colCode);
	
	glDrawArrays(GL_POINTS, 12, 1);
  
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
	glutCreateWindow("Transformation composition. Using the keyboard"); 
	glewInit();

	Initialize();
	glutDisplayFunc(RenderFunction);
	glutIdleFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}
