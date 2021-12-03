/*
- instanced rendering
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

#define INSTANCE_COUNT 40
const GLfloat PI = 3.141592;

GLuint
	VaoId,
	VBPos,
	VBCol,
	VBModelMat,
	EboId,
	ColorBufferId,
	ProgramId,
	viewLocation,
	projLocation,
	colCodeLocation,
	colCode;

// variables for the view matrix
float Obsx = 0.0, Obsy = 0.0, Obsz = -800.f;
float Refx = 0.0f, Refy = 0.0f;
float Vx = 0.0;

// variables for the projection matrix
float width = 800, height = 600, znear = 1, fovdeg = 90;

glm::vec3 Obs, PctRef, Vert;
glm::mat4 view, projection;

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
		Obsz += 10;
		break;
	case '-':
		Obsz -= 10;
		break;
	}
	if (key == 27)
		exit(0);
}

void processSpecialKeys(int key, int xx, int yy) {
	switch (key)
	{
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
	GLfloat Vertices[] =
	{
		// vertices in the z = -50 plane
		-50.0f, -50.0f, -50.0f, 1.0f,
		50.0f, -50.0f,  -50.0f, 1.0f,
		50.0f, 50.0f,  -50.0f, 1.0f,
		-50.0f, 50.0f, -50.0f, 1.0f,
		// vertices in the z = 50 plane                 		
		-50.0f,  -50.0f, 50.0f, 1.0f,
		50.0f,  -50.0f,  50.0f, 1.0f,
		50.0f,  50.0f,  50.0f, 1.0f,
		-50.0f,  50.0f, 50.0f, 1.0f,
	};

	// the colors of the instances
	glm::vec4 Colors[INSTANCE_COUNT];
	for (int n = 0; n < INSTANCE_COUNT; n++)
	{
		float a = float(n) / 4.0f;
		float b = float(n) / 5.0f;
		float c = float(n) / 6.0f;
		Colors[n][0] = 0.35f + 0.30f * (sinf(a + 2.0f) + 1.0f);
		Colors[n][1] = 0.25f + 0.25f * (sinf(b + 3.0f) + 1.0f);
		Colors[n][2] = 0.25f + 0.35f * (sinf(c + 4.0f) + 1.0f);
		Colors[n][3] = 1.0f;
	}

	// instances matrices
	glm::mat4 MatModel[INSTANCE_COUNT];
	for (int n = 0; n < INSTANCE_COUNT; n++)
	{
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), n * PI / 8, glm::vec3(n, 2 * n * n, n / 3));
		glm::mat4 translate = glm::translate(
									glm::mat4(1.0f), 
									glm::vec3(80 * n * sin(10.f * n * 180 / PI), 80 * n * cos(10.f * n * 180 / PI), 0.0));
		MatModel[n] = translate * rotate;
	}

	GLubyte Indices[] =
	{
	  1, 2, 0,   0, 2, 3,
	  2, 3, 6,   6, 3, 7,
	  7, 3, 4,   4, 3, 0,
	  4, 0, 5,   5, 0, 1,
	  1, 2, 5,   5, 2, 6,
	  5, 6, 4,   4, 6, 7,
	  0, 1, 2, 3,
	  4, 5, 6, 7,
	  0, 4,
	  1, 5,
	  2, 6,
	  3, 7,
	};

	glGenVertexArrays(1, &VaoId);
	glGenBuffers(1, &VBPos);
	glGenBuffers(1, &VBCol);
	glGenBuffers(1, &VBModelMat);
	glGenBuffers(1, &EboId);

	glBindVertexArray(VaoId);

	glBindBuffer(GL_ARRAY_BUFFER, VBPos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBCol);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid*)0);
	glVertexAttribDivisor(1, 1);  // the rate at which color distribution occurs per instance

	// 2..5 (2+i): position matrix
	glBindBuffer(GL_ARRAY_BUFFER, VBModelMat);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MatModel), MatModel, GL_STATIC_DRAW);
	for (int i = 0; i < 4; i++) // for each column
	{
		glEnableVertexAttribArray(2 + i);
		glVertexAttribPointer(2 + i,              // Location
			4, GL_FLOAT, GL_FALSE,                // vec4
			sizeof(glm::mat4),                    // Stride
			(void*)(sizeof(glm::vec4) * i));      // Start offset
		glVertexAttribDivisor(2 + i, 1);
	}

	// indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

void DestroyVBO(void)
{
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &VBPos);
	glDeleteBuffers(1, &VBCol);
	glDeleteBuffers(1, &VBModelMat);
	glDeleteBuffers(1, &EboId);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);
}

void CreateShaders(void)
{
	ProgramId = LoadShaders("08_03_Shader.vert", "08_03_Shader.frag");
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
	viewLocation = glGetUniformLocation(ProgramId, "viewMatrix");
	projLocation = glGetUniformLocation(ProgramId, "projectionMatrix");
	colCodeLocation = glGetUniformLocation(ProgramId, "colCode");
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// CreateVBO(); // uncomment, if needed!
	glBindVertexArray(VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, VBPos);
	glBindBuffer(GL_ARRAY_BUFFER, VBCol);
	glBindBuffer(GL_ARRAY_BUFFER, VBModelMat);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);

	// view matrix
	Obs = glm::vec3(Obsx, Obsy, Obsz);
	Refx = Obsx; Refy = Obsy;
	PctRef = glm::vec3(Refx, Refy, 800.0f);
	Vert = glm::vec3(Vx, 1.0f, 0.0f);
	view = glm::lookAt(Obs, PctRef, Vert);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// projection matrix
	projection = glm::infinitePerspective(fovdeg * PI / 180, GLfloat(width) / GLfloat(height), znear);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);

	// faces
	colCode = 0;
	glUniform1i(colCodeLocation, colCode);
	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0, INSTANCE_COUNT);
	
	// edges
	colCode = 1;
	glUniform1i(colCodeLocation, colCode);
	glLineWidth(2.5);
	glDrawElementsInstanced(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(36), INSTANCE_COUNT);
	glDrawElementsInstanced(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(40), INSTANCE_COUNT);
	glDrawElementsInstanced(GL_LINES, 8, GL_UNSIGNED_BYTE, (void*)(44), INSTANCE_COUNT);

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
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1200, 900);
	glutCreateWindow("Instanced rendering");
	glewInit();

	Initialize();
	glutDisplayFunc(RenderFunction);
	glutIdleFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}
