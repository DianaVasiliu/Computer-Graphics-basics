#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
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
float alpha = 0.0f, beta = 0.0f, dist = 300.0f;
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

// the number of vertices, colors and indices to draw a sphere and a cylinder
glm::vec4 Vertices[NR_PARR * NR_MERID + (NR_PARR + 1) * NR_MERID];
glm::vec3 Colors[NR_PARR * NR_MERID + (NR_PARR + 1) * NR_MERID];
GLushort Indices[6 * NR_PARR * NR_MERID + 6 * (NR_MERID - 2) + 6 * (NR_PARR + 1) * NR_MERID];
// 2 * NR_PARR * NR_MERID -> cylinder parallels and meridians
// 4 * NR_PARR * NR_MERID -> cylinder square faces
// 6 * (NR_MERID - 2) -> 
//		NR_MERID - 2 triangles for a cylinder base
//		3 indices for each triangle
//		2 bases
// 2 * (NR_PARR + 1) * NR_MERID -> sphere parallels and meridians
// 4 * (NR_PARR + 1) * NR_MERID -> sphere square faces

int INDICES_OFFSET = 6 * NR_PARR * NR_MERID + 6 * (NR_MERID - 2);	// offset for indices
int OFFSET = NR_PARR * NR_MERID;	// offset for vertices

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
	// CYLINDER
	radius = 30;
	for (int merid = 0; merid < NR_MERID; merid++)
	{
		for (int parr = 0; parr < NR_PARR; parr++)
		{
			// implementation of parametric representation
			float u = U_MIN + parr * step_u;
			float v = V_MIN + merid * step_v;

			// inverted u and v in the formula, to draw the points on the vertical first,
			// then going to the next meridian
			// thus, when traversing a meridian, we want to keep x and y and modify z
			// so z must be variable with parr
			float x_coord = radius * cosf(v); // the coordinates of the vertex corresponding to (u, v)
			float y_coord = radius * sinf(v);
			float z_coord = 50 * u - 90;

			// vertex identifier
			// coordinates + color + index when traversing the meridians
			index = merid * NR_PARR + parr;
			Vertices[index] = glm::vec4(x_coord, y_coord, z_coord, 1.0);
			Colors[index] = glm::vec3(0.1f + sinf(v), 0.1f + cosf(v), 0.1f + -1.5 * sinf(v));
			Indices[index] = index;

			// index for the same vertex when traversing parallels
			index_aux = parr * NR_MERID + merid;
			Indices[NR_PARR * NR_MERID + index_aux] = index;

			int AUX;
			AUX = 2 * NR_PARR * NR_MERID;

			// indices for drawing the faces, for the current vertex 4 new vertices are defined
			if (parr != NR_PARR - 1)	// don't create a rectangle (practically non-existent) above the cylinder
			{
				int index1 = index;				// the v vertex
				int index2 = index + NR_PARR;	// to the right of v, on the next meridian
				int index3 = index2 + 1;		// top right of v
				int index4 = index + 1;			// above v, on the same meridian

				if (merid == NR_MERID - 1)  // at the last meridian, it must return to the first meridian
				{
					index2 = index2 % NR_PARR;
					index3 = index3 % NR_PARR;
				}

				Indices[AUX + 4 * index] = index1;
				Indices[AUX + 4 * index + 1] = index2;
				Indices[AUX + 4 * index + 2] = index3;
				Indices[AUX + 4 * index + 3] = index4;
			}

			// indices for the top and bottom base
			if (index == 0)
			{
				// we add a 4 * term, because we added previous indices, for faces
				AUX = 2 * NR_PARR * NR_MERID + 4 * NR_PARR * NR_MERID;

				// we want to have indices in the following order:
				// 0 a b
				// 0 b c
				// 0 c d etc
				// we have step 3, because in one step we complete 3 indices
				// we have (NR_MERID - 2), because we will create NR_MERID - 2 triangles
				// because 0 will not create another triangle with the last meridian
				// we have * 3 because, for each triangle, we add 3 indices
				for (int c = 0; c < (NR_MERID - 2) * 3; c += 3) {
					// c / 3 + 1 = return to the real meridian and add an offset of 1
					// because we add at least 1 parallel
					// meaning:
					// c = 0 => NR_PARR * (0 + 1)
					// c = 3 => NR_PARR * (1 + 1)
					Indices[AUX + c] = index;
					Indices[AUX + c + 1] = index + NR_PARR * (c / 3 + 1);
					Indices[AUX + c + 2] = index + NR_PARR * (c / 3 + 2);
				}
			}

			if (index == NR_PARR - 1)
			{
				// we add a term, because we added previous indices, for the top base
				AUX = 2 * NR_PARR * NR_MERID + 4 * NR_PARR * NR_MERID + (NR_MERID - 2) * 3;

				for (int c = 0; c < (NR_MERID - 2) * 3; c += 3) {
					Indices[AUX + c] = index;
					Indices[AUX + c + 1] = index + NR_PARR * (c / 3 + 1);
					Indices[AUX + c + 2] = index + NR_PARR * (c / 3 + 2);
				}
			}
		}
	};

	// SPHERE
	radius = 100;
	for (int merid = 0; merid < NR_MERID; merid++)
	{
		for (int parr = 0; parr < NR_PARR + 1; parr++)
		{
			// implementation of parametric representation
			float u = U_MIN + parr * step_u;
			float v = V_MIN + merid * step_v;

			float x_coord = radius * cosf(u) * cosf(v); // coordinates of the vertex corresponding to (u, v)
			float y_coord = radius * cosf(u) * sinf(v);
			float z_coord = radius * sinf(u) + 25;

			// vertex identifier
			// coordinates + color + index when traversing the meridians
			// we take care to add the necessary offset
			index = merid * (NR_PARR + 1) + parr;
			Vertices[OFFSET + index] = glm::vec4(x_coord, y_coord, z_coord, 1.0);
			Colors[OFFSET + index] = glm::vec3(0.1f + sinf(u), 0.1f + cosf(v), 0.1f + -1.5 * sinf(u));
			Indices[INDICES_OFFSET + index] = OFFSET + index;

			// index for the same vertex when traversing parallels
			index_aux = parr * NR_MERID + merid;
			Indices[INDICES_OFFSET + (NR_PARR + 1) * NR_MERID + index_aux] = OFFSET + index;

			// indices for drawing the faces, for the current vertex 4 new vertices are defined
			if ((parr + 1) % (NR_PARR + 1) != 0)	// the vertex considered is not the North Pole
			{
				int AUX = INDICES_OFFSET + 2 * (NR_PARR + 1) * NR_MERID;
				int index1 = index;						// the v vertex
				int index2 = index + (NR_PARR + 1);		// to the right of v, on the next meridian
				int index3 = index2 + 1;				// top right of v
				int index4 = index + 1;					// above v, on the same meridian

				if (merid == NR_MERID - 1)  // at the last meridian, it must return to the first meridian
				{
					index2 =  index2 % (NR_PARR + 1);
					index3 =  index3 % (NR_PARR + 1);
				}

				// we take care to add the necessary offset
				Indices[AUX + 4 * index] = OFFSET + index1;		// some values of the Indices, corresponding to the North Pole, have inadequate values
				Indices[AUX + 4 * index + 1] = OFFSET + index2;
				Indices[AUX + 4 * index + 2] = OFFSET + index3;
				Indices[AUX + 4 * index + 3] = OFFSET + index4;
			}
		}
	}

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
	ProgramId = LoadShaders("tree_Shader.vert", "tree_Shader.frag");
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

	// CreateVBO(); // uncomment, if needed
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

	colCode = 2;	// brown, for the tree trunk
	glUniform1i(colCodeLocation, colCode);

	// CYLINDER
	// drawing faces
	for (int square = 0; square < NR_PARR * NR_MERID; square++)
	{
		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, (GLvoid*)((2 * NR_PARR * NR_MERID + 4 * square) * sizeof(GLushort)));
	}
	for (int tr = 0; tr < 2 * (NR_MERID - 2); tr++) {
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (GLvoid*)((6 * NR_PARR * NR_MERID + 3 * tr) * sizeof(GLushort)));
	}	

	colCode = 3;	// green, for the leaves
	glUniform1i(colCodeLocation, colCode);
	
	// SPHERE
	for (int square = 0; square < (NR_PARR + 1) * NR_MERID; square++)
	{
		if ((square + 1) % (NR_PARR + 1) != 0)    // faces with the North Pole at the bottom left are not considered
		{
			glDrawElements(GL_QUADS, 4, GL_UNSIGNED_SHORT, (GLvoid*)((INDICES_OFFSET + (NR_PARR + 1) * NR_MERID * 2 + 4 * square) * sizeof(GLushort)));
		}
	}

	// drawing edges (meridians and parallels)
	colCode = 1;	// black, for the edges
	glUniform1i(colCodeLocation, colCode);
	glLineWidth(2.0);

	// CYLINDER
	// only the top and bottom parallels
	glDrawElements(GL_LINE_LOOP, NR_MERID, GL_UNSIGNED_SHORT, (GLvoid*)((NR_PARR * NR_MERID) * sizeof(GLushort)));
	glDrawElements(GL_LINE_LOOP, NR_MERID, GL_UNSIGNED_SHORT, (GLvoid*)((NR_PARR * NR_MERID + (NR_PARR - 1) * NR_MERID) * sizeof(GLushort)));

	// all edges
	// meridians
	for (int i = 0; i < NR_MERID; i++) {
		glDrawElements(GL_LINE_STRIP, NR_PARR, GL_UNSIGNED_SHORT, (void*)(2 * i * NR_PARR));
	}
	// parallels
	/*for (int parr = 0; parr < NR_PARR; parr++)
	{
		glDrawElements(GL_LINE_LOOP, NR_MERID, GL_UNSIGNED_SHORT, (GLvoid*)((NR_PARR * NR_MERID + parr * NR_MERID) * sizeof(GLushort)));
	}*/

	// SPHERE
	// meridians
	//glDrawElements(GL_LINE_STRIP, (NR_PARR + 1) * (NR_MERID), GL_UNSIGNED_SHORT, (void*)(INDICES_OFFSET * sizeof(GLushort)));
	// parallels
	for (int parr = 0; parr < NR_PARR; parr++) // each parallel circle separately
	{
		glDrawElements(GL_LINE_LOOP, NR_MERID, GL_UNSIGNED_SHORT, (GLvoid*)((INDICES_OFFSET + (NR_PARR + 1) * (NR_MERID) + parr * NR_MERID) * sizeof(GLushort)));
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
	glutCreateWindow("3D Tree");

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
