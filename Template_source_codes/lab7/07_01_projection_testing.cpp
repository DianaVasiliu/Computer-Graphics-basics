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

GLuint
	VaoId,
	VboId,
	ColorBufferId,
	ProgramId,
	myMatrixLocation,
	colCodeLocation;

int colCode;
float PI = 3.141592;

glm::mat4 myMatrix, matrRot;

// elements for the view matrix
glm::mat4 view;
float Obsx = 0.0, Obsy = 0.0, Obsz = -800.f;
float Refx = 0.0f, Refy = 0.0f;
float Vx = 0.0;

// elements for the projection matrix
glm::mat4 projection;
float 
	width = 800, height = 600, 
	xwmin = -800.f, xwmax = 800, 
	ywmin = -600, ywmax = 600, 
	znear = 200, zfar = 2000, 
	fov = 30;

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key) {
	case 'l':
		Vx += 0.1;
		break;
	case 'r':
		Vx -= 0.1;
		break;
	case '+':
		// znear += 10;
		// zfar += 10;
		Obsz += 10;
		break;
	case '-':
		// znear -= 10;
		// zfar -= 10;
		Obsz -= 10;
		break;
	}
	if (key == 27)
		exit(0);
}

void processSpecialKeys(int key, int xx, int yy) 
{
	switch (key) {
	case GLUT_KEY_LEFT:
		Obsx -= 20;
		break;
	case GLUT_KEY_RIGHT:
		Obsx += 20;
		break;
	case GLUT_KEY_UP:
		Obsy += 20;
		break;
	case GLUT_KEY_DOWN:
		Obsy -= 20;
		break;
	}
}

void CreateVBO(void)
{
	GLfloat Vertices[] = {
		// the vertices in the corners
		-390.0f, -290.0f, 0.0f, 1.0f,
		390.0f, -290.0f, 0.0f, 1.0f,
		390.0f, 290.0f, 0.0f, 1.0f,
		-390.0f, 290.0f, 0.0f, 1.0f,
		// vertices for the axes
		-400.0f, 0.0f, 0.0f, 1.0f,
		400.0f, 0.0f, 0.0f, 1.0f,
		0.0f, -300.0f, 0.0f, 1.0f,
		0.0f, 300.0f, 0.0f, 1.0f,
		// vertices for the square
		-50.0f, -50.0f, 0.0f, 1.0f,
		50.0f, -50.0f, 0.0f, 1.0f,
		50.0f, 50.0f, 0.0f, 1.0f,
		-50.0f, 50.0f, 0.0f, 1.0f,
	};

	// colors for the vertices in the corners
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
	ProgramId = LoadShaders("07_01_Shader.vert", "07_01_Shader.frag");
	glUseProgram(ProgramId);
}

void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

void Initialize(void)
{
	myMatrix = glm::mat4(1.0f);
	matrRot = glm::rotate(glm::mat4(1.0f), PI / 8, glm::vec3(0.0, 0.0, 1.0));
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f); 
	CreateVBO();
	CreateShaders();
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// observer's position
	glm::vec3 Obs = glm::vec3(Obsx, Obsy, Obsz);

	// reference point position
	Refx = Obsx; Refy = Obsy;
	glm::vec3 PctRef = glm::vec3(Refx, Refy, -1.0f);

	// the vertical
	glm::vec3 Vert = glm::vec3(Vx, 1.0f, 0.0f);

	// the view matrix
	view = glm::lookAt(Obs, PctRef, Vert);
	
	// the projection matrix
	projection = glm::ortho(xwmin, xwmax, ywmin, ywmax, znear, zfar);
	// projection = glm::frustum(xwmin, xwmax, ywmin, ywmax, znear, zfar);
	// projection = glm::perspective(fov, GLfloat(width) / GLfloat(height), znear, zfar);

	// matrix for the "fixed" elements - the points in the corners and the axes
	myMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / GLfloat(width), 1.f / GLfloat(height), 1.0));
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	// drawing the points in the corners and the axes
	colCode = 0;
	colCodeLocation = glGetUniformLocation(ProgramId, "colCode");
	glUniform1i(colCodeLocation, colCode);
	glPointSize(10.0);
	glDrawArrays(GL_POINTS, 0, 4);
	glDrawArrays(GL_LINES, 4, 4);

	// matrix for the square, which is seen from different angles
	myMatrix = projection * view;
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	// drawing the square
	colCode = 1;
	colCodeLocation = glGetUniformLocation(ProgramId, "colCode");
	glUniform1i(colCodeLocation, colCode);
	glDrawArrays(GL_POLYGON, 8, 4);

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
	glutCreateWindow("Visualization and projection transformations");
	glewInit();
	
	Initialize();
	glutDisplayFunc(RenderFunction);
	glutIdleFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}
