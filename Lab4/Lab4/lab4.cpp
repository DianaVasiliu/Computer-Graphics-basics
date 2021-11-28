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
	resizeMatrixLocation,
	matrMoveLocation,
	codeProductLocation,
	colCodeLocation;

glm::mat4 myMatrix, resizeMatrix, matrTransl1, matrTransl2, matrMove, matrRot, matrScale;

int colCode, codeProduct;
float PI = 3.141592;
int width = 400, height = 300;
float X = 525.0f, Y = 450.0f;

void CreateVBO(void)
{
	GLfloat Vertices[] = {
		// the background
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, (float)height * 2, 0.0f, 1.0f,
		(float)width * 2, (float)height * 2, 0.0f, 1.0f,
		(float)width * 2, 0.0f, 0.0f, 1.0f,
		// rectangle D
		200.0f, 100.0f, 0.0f, 1.0f,
		200.0f, 300.0f, 0.0f, 1.0f,
	    300.0f, 300.0f, 0.0f, 1.0f,
		300.0f, 100.0f, 0.0f, 1.0f,
		// convex polygon P1
		400.0f, 400.0f, 0.0f, 1.0f,
		450.0f, 350.0f, 0.0f, 1.0f,
		500.0f, 400.0f, 0.0f, 1.0f,
		450.0f, 450.0f, 0.0f, 1.0f,
		400.0f, 450.0f, 0.0f, 1.0f,
		// concave polygon P2
		700.0f, 450.0f, 0.0f, 1.0f,
		750.0f, 350.0f, 0.0f, 1.0f,
		700.0f, 400.0f, 0.0f, 1.0f,
		650.0f, 350.0f, 0.0f, 1.0f,
		// rotation point P
		X, Y, 0.0f, 1.0f,
	};

	GLfloat Colors[] = {
		// background colors
		1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		// rectangle D colors
		0.0f, 0.4f, 0.6f, 1.0f,
		0.0f, 0.4f, 0.6f, 1.0f,
		0.0f, 0.4f, 0.6f, 1.0f,
		0.0f, 0.4f, 0.6f, 1.0f,
		// P1 polygon colors
		0.3f, 0.3f, 0.3f, 1.0f,
		0.3f, 0.3f, 0.3f, 1.0f,
		0.3f, 0.3f, 0.3f, 1.0f,
		0.3f, 0.3f, 0.3f, 1.0f,
		0.3f, 0.3f, 0.3f, 1.0f,
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
	ProgramId = LoadShaders("shader.vert", "shader.frag");
	glUseProgram(ProgramId);
}

void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

void Initialize(void)
{
	resizeMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / width, 1.f / height, 1.0));
	matrMove = glm::translate(glm::mat4(1.0f), glm::vec3(-width, -height, 0.0));
	
	matrRot = glm::rotate(glm::mat4(1.0f), -PI / 4, glm::vec3(0.0, 0.0, 1.0));
	
	// we bring the P point in the origin
	matrTransl1 = glm::translate(glm::mat4(1.0f), glm::vec3(-X, -Y, 0.0));
	matrTransl2 = glm::translate(glm::mat4(1.0f), glm::vec3(X, Y, 0.0));

	matrScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.2f, 1.2f, 1.0f));

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

	// bringing the window in the [0,800]x[0,600] interval
	// resizeMatrix - used for zoom out
	// matrMove - used to bring the window in the 1st quadrant
	myMatrix = resizeMatrix * matrMove;
	
	// resetting the type of product
	// once the first call to the shader is made
	codeProduct = 0;
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	codeProductLocation = glGetUniformLocation(ProgramId, "codeProduct");
	glUniform1i(codeProductLocation, codeProduct);
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	colCode = 0;
	colCodeLocation = glGetUniformLocation(ProgramId, "colCode");
	glUniform1i(colCodeLocation, colCode);

	// drawing the background
	glDrawArrays(GL_POLYGON, 0, 4);

	// drawing the D rectangle
	glDrawArrays(GL_POLYGON, 4, 4);

	// drawing the convex polygon P1
	glDrawArrays(GL_POLYGON, 8, 5);

	// drawing the concave polygon P2
	glDrawArrays(GL_POLYGON, 13, 4);

	colCode = 2;
	colCodeLocation = glGetUniformLocation(ProgramId, "colCode");
	glUniform1i(colCodeLocation, colCode);

	// drawing the P point
	glPointSize(8.0);
	glEnable(GL_POINT_SMOOTH);
	glDrawArrays(GL_POINTS, 17, 1);

	// drawing rotated P1 and P2
	myMatrix = resizeMatrix * matrMove * matrTransl2 * matrRot * matrTransl1;
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glLineWidth(4.0);
	glDrawArrays(GL_LINE_LOOP, 8, 5);
	glDrawArrays(GL_LINE_LOOP, 13, 4);

	colCode = 1;
	colCodeLocation = glGetUniformLocation(ProgramId, "colCode");
	glUniform1i(colCodeLocation, colCode);

	// drawing scaled rectangle
	// setting the type of product for the shader
	// (only for the scaled rectangle)
	codeProduct = 1;
	codeProductLocation = glGetUniformLocation(ProgramId, "codeProduct");
	resizeMatrixLocation = glGetUniformLocation(ProgramId, "resizeMatrix");
	matrMoveLocation = glGetUniformLocation(ProgramId, "matrMove");
	matrScaleLocation = glGetUniformLocation(ProgramId, "matrScale");
	glUniform1i(codeProductLocation, codeProduct);
	glUniformMatrix4fv(resizeMatrixLocation, 1, GL_FALSE, &resizeMatrix[0][0]);
	glUniformMatrix4fv(matrMoveLocation, 1, GL_FALSE, &matrMove[0][0]);
	glUniformMatrix4fv(matrScaleLocation, 1, GL_FALSE, &matrScale[0][0]);
	
	// drawing the scaled rectangle
	glLineWidth(4.0);
	glDrawArrays(GL_LINE_LOOP, 4, 4);

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
	glutCreateWindow("Transformations");
	glewInit();
	Initialize();
	glutDisplayFunc(RenderFunction);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}
