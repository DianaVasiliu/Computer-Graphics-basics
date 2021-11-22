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
	VertexShaderId,
	FragmentShaderId,
	ProgramId;

///////////////////////////////////////////

void CreateVBO(void)
{
	GLfloat Vertices[] = {
		-0.95f, -0.95f, 0.0f, 1.0f,
		-0.945f, -0.945f, 0.0f, 1.0f,
		-0.94f, -0.94f, 0.0f, 1.0f,
		-0.93f, -0.928f, 0.0f, 1.0f,
		-0.8f, -0.7f, 0.0f, 1.0f,

		-0.8f, 0.2f, 0.0f, 1.0f,

		-0.55f, 0.4f, 0.0f, 1.0f,

		-1.0f, -0.7f, 0.0f, 1.0f,
		-0.4f, 0.4f, 0.0f, 1.0f,

		-0.4f, 0.1f, 0.0f, 1.0f,
		-0.05f, 0.4f, 0.0f, 1.0f,

		0.15f, 0.45f, 0.0f, 1.0f,
		0.05f, 1.0f, 0.0f, 1.0f
	};

	GLfloat Colors[] = {
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.2f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,

		0.5f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 1.0f, 1.0f,

		0.5f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 1.0f, 1.0f,
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
	ProgramId = LoadShaders("01_03_Shader.vert", "01_03_Shader.frag");
	glUseProgram(ProgramId);
}

void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	CreateVBO();
	CreateShaders();
}

void draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	CreateVBO();
	CreateShaders();

	glPointSize(6.0);
	glDrawArrays(GL_POINTS, 0, 7);

	glLineWidth(3.0);
	glDrawArrays(GL_LINES, 7, 2);
	glDrawArrays(GL_LINES, 9, 2);
	glDrawArrays(GL_LINES, 11, 2);

	glFlush();
}

void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 500);
	glutCreateWindow("Points & Segments - New OpenGL");
	glewInit();

	init();
	glClear(GL_COLOR_BUFFER_BIT);

	glutDisplayFunc(draw);
	glutCloseFunc(Cleanup);

	glutMainLoop();
}