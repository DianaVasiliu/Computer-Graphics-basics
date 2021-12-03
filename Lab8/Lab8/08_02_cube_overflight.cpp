/*
- object overflight using spheric coordinates
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

float const PI = 3.141592;

int colCode;

// variables for the view matrix
float Refx = 0.0f, Refy = 0.0f, Refz = 0.0f;
float alpha = 0.0f, beta = 0.0f, dist = 200.0f;
float Obsx, Obsy, Obsz;
float Vx = 0.0f, Vy = 0.0f, Vz = -1.0f;
float incr_alpha1 = 0.01, incr_alpha2 = 0.01;

// variables for the projection matrix
float 
	width = 800, height = 600, 
	xwmin = -200.f, xwmax = 200, 
	ywmin = -200, ywmax = 200, 
	znear = 1, 
	fov = 30;

glm::vec3 Obs, PctRef, Vert;
glm::mat4 view, projection;

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key) {
	case '-':
		dist -= 5.0;
		break;
	case '+':
		dist += 5.0;
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
		beta -= 0.01;
		break;
	case GLUT_KEY_RIGHT:
		beta += 0.01;
		break;
	case GLUT_KEY_UP:
		alpha += incr_alpha1;
		if (abs(alpha - PI / 2) < 0.05)
		{
			incr_alpha1 = 0.f;
		}
		else
		{
			incr_alpha1 = 0.01f;
		}
		break;
	case GLUT_KEY_DOWN:
		alpha -= incr_alpha2;
		if (abs(alpha + PI / 2) < 0.05)
		{
			incr_alpha2 = 0.f;
		}
		else
		{
			incr_alpha2 = 0.01f;
		}
		break;
	}
}

void CreateVBO(void)
{
	GLfloat Vertices[] =
	{
		// vertices in the z = -50 plane
		// coordinates					// colors	
		-50.0f, -50.0f, -50.0f, 1.0f,	0.0f, 1.0f, 0.0f,
		50.0f, -50.0f,  -50.0f, 1.0f,   0.0f, 0.9f, 0.0f,
		50.0f, 50.0f,  -50.0f, 1.0f,    0.0f, 0.6f, 0.0f,
		-50.0f, 50.0f, -50.0f, 1.0f,	0.0f, 0.2f, 0.0f,
		// vertices in the z = 50 plane
		// coordinates					// colors		
		-50.0f, -50.0f, 50.0f, 1.0f,	1.0f, 0.0f, 0.0f,
		50.0f, -50.0f,  50.0f, 1.0f,	0.7f, 0.0f, 0.0f,
		50.0f, 50.0f,  50.0f, 1.0f,		0.5f, 0.0f, 0.0f,
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

	glGenBuffers(1, &VboId);
	glGenBuffers(1, &EboId);

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
	ProgramId = LoadShaders("08_02_Shader.vert", "08_02_Shader.frag");
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

	// observer's position
	Obsx = Refx + dist * cos(alpha) * cos(beta);
	Obsy = Refy + dist * cos(alpha) * sin(beta);
	Obsz = Refz + dist * sin(alpha);

	glm::vec3 Obs = glm::vec3(Obsx, Obsy, Obsz);   // changing the observer's position
	glm::vec3 PctRef = glm::vec3(Refx, Refy, Refz); // reference point position
	glm::vec3 Vert = glm::vec3(Vx, Vy, Vz); // the vertical vector
	view = glm::lookAt(Obs, PctRef, Vert);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// projection matrix
	projection = glm::infinitePerspective(fov, GLfloat(width) / GLfloat(height), znear);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);

	// faces
	colCode = 0;
	glUniform1i(colCodeLocation, colCode);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
	
	// edges
	colCode = 1;
	glUniform1i(colCodeLocation, colCode);
	glLineWidth(3.5);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(36));
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, (void*)(40));
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_BYTE, (void*)(44));

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
	glutCreateWindow("Cube overflight");
	glewInit();

	Initialize();
	glutDisplayFunc(RenderFunction);
	glutIdleFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}
