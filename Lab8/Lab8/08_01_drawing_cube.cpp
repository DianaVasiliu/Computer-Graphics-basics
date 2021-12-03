/*
- Using indices to draw the faces and the edges of a 3D object (cube)
- The role of the Depth Test
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

GLuint
	VaoId,
	VboId,
	EboId,
	ProgramId,
	viewLocation,
	projLocation,
	colCodeLocation;

int colCode;

// elements for the view matrix
float Obsx = 0.0, Obsy = 0.0, Obsz = -300.f;
float Refx = 0.0f, Refy = 0.0f;
float Vx = 0.0;

// elements for the projection matrix
float 
	width = 800, height = 600, 
	xwmin = -200.f, xwmax = 200, 
	ywmin = -200, ywmax = 200, 
	znear = 100, zfar = -100, 
	fov = 30;

glm::vec3 Obs, RefPoint, Vert;
glm::mat4 view, projection;

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
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

void processSpecialKeys(int key, int xx, int yy)
{
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
	/*
	GLfloat Vertices[] =
	{
		// vertices in the z = -50 plane
		// coordinates					// colors
		-50.0f, -50.0f, -50.0f, 1.0f,	0.0f, 1.0f, 0.0f,
		50.0f, -50.0f, -50.0f, 1.0f,	0.0f, 0.9f, 0.0f,
		50.0f, 50.0f, -50.0f, 1.0f,		0.0f, 0.6f, 0.0f,
		-50.0f, 50.0f, -50.0f, 1.0f,	0.0f, 0.2f, 0.0f,
		// vertices in the z = 50 plane
		// coordinates					// colors		
		-50.0f, -50.0f, 50.0f, 1.0f,	1.0f, 0.0f, 0.0f,
		50.0f, -50.0f, 50.0f, 1.0f,		0.7f, 0.0f, 0.0f,
		50.0f, 50.0f, 50.0f, 1.0f,		0.5f, 0.0f, 0.0f,
		-50.0f, 50.0f, 50.0f, 1.0f,		0.1f, 0.0f, 0.0f,
	};

	GLubyte Indices[] =
	{
		1, 0, 2,   2, 0, 3,  // bottom face
		2, 3, 6,   6, 3, 7,  // side face
		7, 3, 4,   4, 3, 0,  // side face
		4, 0, 5,   5, 0, 1,  // side face
		1, 2, 5,   5, 2, 6,  // side face
		5, 6, 4,   4, 6, 7, //  top face
		0, 1, 2, 3,  // bottom face contour 
		4, 5, 6, 7,  // top face contour
		0, 4, // side edge
		1, 5, // side edge
		2, 6, // side edge
		3, 7, // side edge
	};
	*/

	GLfloat Vertices[] =
	{
		// coordinates						// colors		
		// the triangle in the front
		-50.0f, -50.0f, -50.0f, 1.0f,		0.0f, 0.7f, 0.7f,
		50.0f, -50.0f,  -50.0f, 1.0f,		0.0f, 0.7f, 0.7f,
		0.0f, 50.0f, -50.0f, 1.0f,			0.7f, 0.7f, 0.0f,

		// the vertex at the top
		0.0f, -100.0f, 0.0f, 1.0f,			0.9f, 0.2f, 0.8f,

		// vertices on the top line
		100.0f, -50.0f, 0.0f, 1.0f,			0.0f, 0.7f, 0.7f,
		50.0f, -50.0f, 50.0f, 1.0f,			0.0f, 0.7f, 0.7f,
		-50.0f,  -50.0f, 50.0f, 1.0f,		0.0f, 0.7f, 0.7f,
		-100.0f, -50.0f, 0.0f, 1.0f,		0.0f, 0.7f, 0.7f,

		// vertices on the bottom line
		0.0f, 50.0f, 50.0f, 1.0f,			0.7f, 0.7f, 0.0f,
		-75.0f, 50.0f, 25.0f, 1.0f,			0.7f, 0.7f, 0.0f,
		-100.0f, 50.0f, 0.0f, 1.0f,			0.7f, 0.7f, 0.0f,
		100.0f, 50.0f, 0.0f, 1.0f,			0.7f, 0.7f, 0.0f,
		75.0f, 50.0f, 25.0f, 1.0f,			0.7f, 0.7f, 0.0f,

		// the vertex at the bottom
		0.0f, 100.0f, 0.0f, 1.0f,			0.9f, 0.3f, 0.3f
	};

	GLubyte Indices[] =
	{
		// top faces
		1, 3, 0,
		4, 3, 1,
		5, 3, 4,
		6, 3, 5,
		7, 3, 6,
		0, 3, 7,

		// side faces
		6, 5, 8,
		6, 8, 9,
		7, 6, 9,
		7, 9, 10,
		0, 7, 10,
		0, 10, 2,
		1, 0, 2,
		1, 2, 11,
		4, 1, 12,
		4, 11, 12,
		5, 4, 12,
		5, 12, 8,

		// bottom faces
		8, 12, 13,
		8, 9, 13,
		9, 10, 13,
		2, 10, 13,
		11, 2, 13,
		12, 11, 13,

		// the contours
		0, 3,
		1, 3,
		4, 3,
		5, 3,
		6, 3,
		7, 3,
		0, 1, 4, 5, 6, 7,
		2, 11, 12, 8, 9, 10,
		8, 13,
		9, 13,
		10, 13,
		2, 13,
		11, 13,
		12, 13,
		0, 2, 1, 11, 4, 12, 5, 8, 6, 9, 7, 10
	};

	glGenBuffers(1, &VboId); // buffer for attributes
	glGenBuffers(1, &EboId); // buffer for indices

	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
}

void DestroyVBO(void)
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VboId);
	glDeleteBuffers(1, &EboId);
}

void CreateShaders(void)
{
	ProgramId = LoadShaders("08_01_Shader.vert", "08_01_Shader.frag");
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

	viewLocation = glGetUniformLocation(ProgramId, "viewShader");
	projLocation = glGetUniformLocation(ProgramId, "projectionShader");
	colCodeLocation = glGetUniformLocation(ProgramId, "colCode");
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// CreateVBO(); // uncomment, if needed!
	glBindVertexArray(VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);

	// view matrix
	Obs = glm::vec3(Obsx, Obsy, Obsz); // observer's position
	Refx = Obsx; Refy = Obsy;
	RefPoint = glm::vec3(Refx, Refy, 800.0f); // reference point position
	Vert = glm::vec3(Vx, 1.0f, 0.0f); // the vertical vector

	view = glm::lookAt(Obs, RefPoint, Vert);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// the projection matris
	projection = glm::perspective(fov, GLfloat(width) / GLfloat(height), znear, zfar);
	// projection = glm::ortho(xwmin, xwmax, ywmin, ywmax, znear, zfar);
	// projection = glm::frustum(xwmin, xwmax, ywmin, ywmax, 80.f, -80.f);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);

	// faces
	colCode = 0;
	glUniform1i(colCodeLocation, colCode);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
	glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_BYTE, 0);
	
	// edges
	colCode = 1;
	glUniform1i(colCodeLocation, colCode);
	glLineWidth(3.5);
	//glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(36));
	//glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(40));
	//glDrawElements(GL_LINES, 8, GL_UNSIGNED_BYTE, (void*)(44));
	glDrawElements(GL_LINES, 12, GL_UNSIGNED_BYTE, (void*)(72));
	glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_BYTE, (void*)(84));
	glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_BYTE, (void*)(90));
	glDrawElements(GL_LINES, 12, GL_UNSIGNED_BYTE, (void*)(96));
	glDrawElements(GL_LINE_LOOP, 12, GL_UNSIGNED_BYTE, (void*)(108));

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
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1200, 900);
	glutCreateWindow("Draw a cube using the depth test");
	glewInit();

	Initialize();
	glutDisplayFunc(RenderFunction);
	glutIdleFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}
