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

const int NR_POINTS = 20;
float PI = 3.141592f;

GLuint
	VaoId,
	VboId,
	EboId,
	ColorBufferId,
	ProgramId,
	myMatrixLocation,
	colCodeLocation;

int colCode;
GLint winWidth = 1000, winHeight = 600;
float width = winWidth / 10, height = winHeight / 10;
float cx = 10.0f, cy = 5.0f, radius = 20.0;

glm::mat4 myMatrix;

void CreateVBO(void)
{
	glm::vec4 vertex_coords[NR_POINTS];

	for (int i = 0; i < NR_POINTS; i++)
	{
		// the parametric representation is used to generate the vertices (points located on the circle)
		vertex_coords[i] =
			glm::vec4(	cx + radius * cos(2 * PI * i / NR_POINTS),
						cy + radius * sin(2 * PI * i / NR_POINTS),
						0.0f, 1.0f);
	};

	// vertices colors
	glm::vec4 vertex_colors[NR_POINTS];

	for (int i = 0; i < NR_POINTS; i++)
	{
		vertex_colors[i] = glm::vec4(
								abs(0.4 + 0.3 * i / NR_POINTS + cos(0.1 * i)), 
								abs(0.6 + 0.4 * i / NR_POINTS + sin(0.1 * i)), 
								abs(0.4 + 0.5 * i / NR_POINTS - sin(0.1 * i)), 
								1.0f);
	};

	// indices
	GLuint vertex_indices[NR_POINTS + 1 + 3 * (NR_POINTS - 2)];

	// indices for drawing the contour
	for (int i = 0; i <= NR_POINTS; i++)
		vertex_indices[i] = i % NR_POINTS;

	// indices for drawing the interior
	for (int i = 0; i < NR_POINTS - 2; i++)
	{
		vertex_indices[NR_POINTS + 3 * i + 1] = 0;
		vertex_indices[NR_POINTS + 3 * i + 2] = i + 1;
		vertex_indices[NR_POINTS + 3 * i + 3] = i + 2;
	}

	// a new vertex buffer is created
	glGenBuffers(1, &VboId);
	// indices buffer
	glGenBuffers(1, &EboId);
	// creating a VAO (Vertex Array Object)
	glGenVertexArrays(1, &VaoId);

	// linking VAO
	glBindVertexArray(VaoId);

	// the buffer is set as the current buffer
	glBindBuffer(GL_ARRAY_BUFFER, VboId);

	// the buffer will contain both the vertices coordinates and the color data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_coords) + sizeof(vertex_colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_coords), vertex_coords);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertex_coords), sizeof(vertex_colors), vertex_colors);

	// indices buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);

	// the work with attributes is activated
	// attribute 0 = position
	// attribute 1 = color
	// these are indicated correctly in VBO
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)sizeof(vertex_coords));
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
	ProgramId = LoadShaders("09_01_Shader.vert", "09_01_Shader.frag");
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
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	colCodeLocation = glGetUniformLocation(ProgramId, "colCode");
}

void reshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	winWidth = newWidth;
	winHeight = newHeight;
	width = winWidth / 10, height = winHeight / 10;
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	myMatrix = glm::ortho(-width, width, -height, height);
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	// interior
	colCode = 0;
	glUniform1i(colCodeLocation, colCode);
	glDrawElements(GL_TRIANGLES, 3 * (NR_POINTS - 2), GL_UNSIGNED_INT, (GLvoid*)((NR_POINTS + 1) * sizeof(GLuint)));
	
	// the same effect can be achieved with GL_TRIANGLE_FAN, using the initial set of indexes
	// glDrawElements(GL_TRIANGLE_FAN, NR_POINTS, GL_UNSIGNED_INT, (GLvoid*)(0));
	
	// contour
	colCode = 1;
	glUniform1i(colCodeLocation, colCode);
	glLineWidth(2.0f);
	glDrawElements(GL_LINE_LOOP, NR_POINTS, GL_UNSIGNED_INT, (GLvoid*)(0));
	
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
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Drawing a disk");

	glewInit();
	Initialize();
	glutReshapeFunc(reshapeFcn);
	glutDisplayFunc(RenderFunction);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}
