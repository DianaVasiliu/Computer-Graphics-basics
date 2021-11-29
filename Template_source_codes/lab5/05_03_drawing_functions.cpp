/* DRAWING FUNCTIONS
NEW ELEMENTS:
	- using indices: associated elements (matrix, buffer)
	- using the 4 drawing functions (glDrawArrays, glDrawElements, glDrawElementsBaseVertex, glDrawArraysInstanced) 
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
	EboId,
	ColorBufferId,
	ProgramId,
	myMatrixLocation;

glm::mat4 myMatrix, resizeMatrix, matrTransl; 

float PI = 3.141592, angle = 0;
float tx = 0; float ty = 0; 
float width = 90.f, height = 90.f;
float i = 0.0, alpha = 0.0, beta = 0.2;

void CreateVBO(void)
{
	static const GLfloat vertex_pos[] =
	{
		-10.0f, -10.0f, 0.0f, 1.0f,
		10.0f, -10.0f, 0.0f, 1.0f,
		-10.0f, 10.0f, 0.0f, 1.0f,
		10.0f, 10.0f, 0.0f, 1.0f,
	};  
	
	static const GLfloat vertex_color[] =
	{
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f
	};
	
	static const GLuint vertex_index[] =
	{
		0, 1, 2,
	};
	
	// buffer for vertices
	glGenBuffers(1, &VboId);
	
	// buffer for indices
	glGenBuffers(1, &EboId);
	
	//creating / linking a VAO
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
 
	glBindBuffer(GL_ARRAY_BUFFER, VboId);

	// the buffer will contain the vertex coordinates as well as the colors
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_color) + sizeof(vertex_pos), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_pos), vertex_pos);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertex_pos), sizeof(vertex_color), vertex_color);
 
    // the buffer for indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_index), vertex_index, GL_STATIC_DRAW);
 
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(vertex_pos));
    glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void DestroyVBO(void)
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &EboId);
	glDeleteBuffers(1, &ColorBufferId);
	glDeleteBuffers(1, &VboId);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);
}

void CreateShaders(void)
{
	ProgramId = LoadShaders("05_03_Shader.vert", "05_03_Shader.frag");
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
	resizeMatrix = glm::ortho(-width, width, -height, height);

	glClear(GL_COLOR_BUFFER_BIT);

	CreateVBO();
	CreateShaders();
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");

 	// drawing using glDrawArrays	
	/*
	myMatrix = resizeMatrix;  
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	*/

	// using the incices
	// the matrix: resize and translation
	
	glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-20.0, -20.0, 0.0));
	myMatrix = resizeMatrix * translateMatrix;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(0));
	
	
	// using the incices
	// the matrix: resize and other translation (vertically)
	/*
	glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 40.0, 0.0));
	myMatrix = resizeMatrix * translateMatrix;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE,  &myMatrix[0][0]);
	glDrawElementsBaseVertex(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL, 1);
	*/

	// instanced rendering
	// the matrix: resize and other translation (horizontally)
	/*
	myMatrix = resizeMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(70.0, 0.0, 0.0)); 
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1);
	*/
	
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
	glutCreateWindow("Buffers. Drawing functions"); 
	glewInit();

	Initialize();
	glutDisplayFunc(RenderFunction);
	glutCloseFunc(Cleanup);
	glutMainLoop(); 
}
