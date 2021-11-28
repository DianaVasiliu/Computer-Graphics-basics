 /* TRANSFORMATIONS
	- using the glm library
	- different transformations, made in the shader
	- colouring the primitives using uniform variables and the fragment shader
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
  matrScaleLocation,
  matrTranslLocation,
  matrRotLocation,
  colCodeLocation;
 
glm::mat4 myMatrix, resizeMatrix, matrTransl, matrScale, matrRot; 

int colCode;
float PI=3.141592;
int width=400, height=300;

void CreateVBO(void)
{
	GLfloat Vertices[] = {
		-390.0f, -290.0f, 0.0f, 1.0f,
		390.0f,  -290.0f, 0.0f, 1.0f,
		390.0f, 290.0f, 0.0f, 1.0f,
		-390.0f, 290.0f, 0.0f, 1.0f,
		
		-400.0f, 0.0f, 0.0f, 1.0f,
		400.0f,  0.0f, 0.0f, 1.0f,
		0.0f, -300.0f, 0.0f, 1.0f,
		0.0f, 300.0f, 0.0f, 1.0f,
		
		-50.0f,  -50.0f, 0.0f, 1.0f,
		50.0f, -50.0f, 0.0f, 1.0f,
		50.0f,  50.0f, 0.0f, 1.0f,
		-50.0f,  50.0f, 0.0f, 1.0f
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
	resizeMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f/width, 1.f/height, 1.0));
	matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(100.f, 100.f, 0.0));
	//matrTransl=glm::mat4(glm::translate(glm::mat4(1.0f), glm::vec3(50.f, -20.f, 0.0)));
	//myMatrix = resizeMatrix * matrTransl;	
	//matrTransl = glm::transpose(glm::mat4(glm::translate(glm::mat4(1.0f), glm::vec3(50.f, -20.f, 0.0))));
	
	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.25, 2.f, 1.0));
 
	//glm::mat4 mTransl = glm::translate(glm::mat4(1.0f), glm::vec3(1.f, 1.f, 1.0));
	//myMatrix=mTransl;

	matrRot = glm::rotate(glm::mat4(1.0f), PI/8, glm::vec3(0.0, 0.0, 1.0));
	
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	CreateVBO();
	CreateShaders();
}

void RenderFunction(void)
{
	glm::mat4 view;
	view = glm::lookAt(	glm::vec3(0.0f, 0.0f, 5.f), 
  						glm::vec3(0.0f, 0.0f, -20.0f), 
  						glm::vec3(0.0f, 1.0f, 0.0f));

	glClear(GL_COLOR_BUFFER_BIT);
	myMatrix = resizeMatrix;
  
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);
	
	glPointSize(10.0);
	glDrawArrays(GL_POINTS, 0, 4);

	colCode = 1;
	colCodeLocation = glGetUniformLocation(ProgramId, "colCode");
	glUniform1i(colCodeLocation, colCode);
	glDrawArrays(GL_LINES, 4, 4);
    
	myMatrix = resizeMatrix * matrTransl;
    //CreateShaders();
 
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix"); 
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);

	colCode = 2;
	colCodeLocation=glGetUniformLocation(ProgramId, "colCode"); 
	glUniform1i(colCodeLocation, colCode);
	
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
 
	glutPostRedisplay();
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
	glutInitWindowSize(800, 600); 
	glutCreateWindow("Using <<glm>> for transformations"); 
	glewInit(); 
	
	Initialize();
	glutDisplayFunc(RenderFunction);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}
