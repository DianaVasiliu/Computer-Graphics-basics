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

float const PI = 3.141592f;

// Elements for surface representation
// intervals for the considered parameters (u and v)
float const
U_MIN = -PI / 2,
U_MAX = PI / 2,
V_MIN = 0,
V_MAX = 2 * PI;
// the number of parallels / meridians, in fact the number of values for the parameters
int const
NR_PARR = 10,
NR_MERID = 20;
// the step with which we will increment u, respectively v
float
step_u = (U_MAX - U_MIN) / NR_PARR,
step_v = (V_MAX - V_MIN) / NR_MERID;

// other variables
int colCode;
float radius = 50;
int index, index_aux;

// variables for the view matrix
float Refx = 0.0f, Refy = 0.0f, Refz = 0.0f;
float alpha = 0.0f, beta = 0.0f, dist = 150.0f;
float Obsx, Obsy, Obsz;
float Vx = 0.0f, Vy = 0.0f, Vz = -1.0f;
float incr_alpha1 = 0.03f, incr_alpha2 = 0.03f;

// variables for the projection matrix
float width = 800, height = 600, znear = 1, fov = 30;

// for the viewport
GLint winWidth = 1000, winHeight = 600;

// vectors
glm::vec3 Obs, RefPoint, Vert;

// matrices used
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
		beta -= 0.03f;
		break;
	case GLUT_KEY_RIGHT:
		beta += 0.03f;
		break;
	case GLUT_KEY_UP:
		alpha += incr_alpha1;
		if (abs(alpha - PI / 2) < 0.05)
		{
			incr_alpha1 = 0.f;
		}
		else
		{
			incr_alpha1 = 0.03f;
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
			incr_alpha2 = 0.03f;
		}
		break;
	}
}

void CreateVBO(void)
{
	// vertices
	// Matrices for vertices, colors, indices
	glm::vec4 Vertices[(NR_PARR + 1) * NR_MERID];	// not all vertices are distinct; the two poles are obtained using multiple vertices
	glm::vec3 Colors[(NR_PARR + 1) * NR_MERID];
	GLushort Indices[2 * (NR_PARR + 1) * NR_MERID + 4 * (NR_PARR + 1) * NR_MERID];
	// 2 * (NR_PARR + 1) * NR_MERID -> once we draw the meridians, then we draw the parallels
	// 4 * (NR_PARR + 1) * NR_MERID -> for each meridian and parallel, we need 4 indices to draw a square face

	for (int merid = 0; merid < NR_MERID; merid++)
	{
		for (int parr = 0; parr < NR_PARR + 1; parr++)
		{
			// implementation of parametric representation
			float u = U_MIN + parr * step_u;
			float v = V_MIN + merid * step_v;

			float x_coord = radius * cosf(u) * cosf(v);	// coordinates of the vertex corresponding to (u, v)
			float y_coord = radius * cosf(u) * sinf(v);
			float z_coord = radius * sinf(u);

			// vertex identifier
			// coordinates + color + index when traversing the meridians
			index = merid * (NR_PARR + 1) + parr;
			Vertices[index] = glm::vec4(x_coord, y_coord, z_coord, 1.0);
			Colors[index] = glm::vec3(0.1f + sinf(u), 0.1f + cosf(v), 0.1f + -1.5 * sinf(u));
			Indices[index] = index;

			// index for the same vertex when traversing parallels
			index_aux = parr * (NR_MERID)+merid;
			Indices[(NR_PARR + 1) * NR_MERID + index_aux] = index;

			// indices for drawing the faces, for the current vertex 4 new vertices are defined
			if ((parr + 1) % (NR_PARR + 1) != 0)	// the vertex considered is not the North Pole
			{
				int AUX = 2 * (NR_PARR + 1) * NR_MERID;
				int index1 = index;						// the v vertex
				int index2 = index + (NR_PARR + 1);		// to the right of v, on the next meridian
				int index3 = index2 + 1;				// top right of v
				int index4 = index + 1;					// above v, on the same meridian

				if (merid == NR_MERID - 1)  // at the last meridian, it must return to the first meridian
				{
					index2 = index2 % (NR_PARR + 1);
					index3 = index3 % (NR_PARR + 1);
				}

				Indices[AUX + 4 * index] = index1;  // some values of the Indices, corresponding to the North Pole, have inadequate values
				Indices[AUX + 4 * index + 1] = index2;
				Indices[AUX + 4 * index + 2] = index3;
				Indices[AUX + 4 * index + 3] = index4;
			}
		}
	};

	// generate VAO / buffer
	glGenBuffers(1, &VboId); // attributes
	glGenBuffers(1, &EboId); // indices

	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices) + sizeof(Colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices), Vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertices), sizeof(Colors), Colors);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	// attributes
	// attribute 0 = position
	// attribute 1 = color
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)sizeof(Vertices));
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
	ProgramId = LoadShaders("09_03_Shader.vert", "09_03_Shader.frag");
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

void reshapeFcn(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	winWidth = newWidth;
	winHeight = newHeight;
	width = winWidth / 10, height = winHeight / 10;
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// CreateVBO(); // uncomment if needed
	glBindVertexArray(VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);

	// observer's position
	Obsx = Refx + dist * cos(alpha) * cos(beta);
	Obsy = Refy + dist * cos(alpha) * sin(beta);
	Obsz = Refz + dist * sin(alpha);

	glm::vec3 Obs = glm::vec3(Obsx, Obsy, Obsz);   // changing observer's position
	glm::vec3 RefPoint = glm::vec3(Refx, Refy, Refz); // reference point position
	glm::vec3 Vert = glm::vec3(Vx, Vy, Vz); // vertical vector
	view = glm::lookAt(Obs, RefPoint, Vert);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// projection matrix
	projection = glm::infinitePerspective(fov, GLfloat(width) / GLfloat(height), znear);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);

	// drawing points / edges / faces

	// drawing points
	// glPointSize(3.0);
	// glDrawArrays(GL_POINTS, 0, NR_PARR);

	// drawing faces
	colCode = 0;
	glUniform1i(colCodeLocation, colCode);

	for (int patr = 0; patr < (NR_PARR + 1) * NR_MERID; patr++)
	{
		if ((patr + 1) % (NR_PARR + 1) != 0)		// faces with the North Pole at the bottom left are not considered
			glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, (GLvoid*)((2 * (NR_PARR + 1) * (NR_MERID)+4 * patr) * sizeof(GLushort)));
	}

	// drawing edges (meridians and parallels)
	colCode = 1;
	glUniform1i(colCodeLocation, colCode);

	glLineWidth(2.0);
	glDrawElements(GL_LINE_STRIP, (NR_PARR + 1) * (NR_MERID), GL_UNSIGNED_SHORT, 0); // meridians

	for (int parr = 1; parr < NR_PARR; parr++)	// each parallel circle separately
	{
		glDrawElements(GL_LINE_LOOP, (NR_MERID), GL_UNSIGNED_SHORT, (GLvoid*)(((NR_PARR + 1) * (NR_MERID)+parr * NR_MERID) * sizeof(GLushort)));
	}

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
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Drawing and flying over a sphere");

	glewInit();
	Initialize();
	glutDisplayFunc(RenderFunction);
	glutIdleFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutReshapeFunc(reshapeFcn);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}
