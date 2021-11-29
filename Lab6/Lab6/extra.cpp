#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <cmath>
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
    ProgramId,
    myMatrixLocation;

const float TWO_PI = 6.28;
const int n = 8;
const GLint nrVertices = 600;
const int NMAX = n * 4 * 2; // n points, 4 coordinates each, 2 polygons
float width = 800.f, height = 600.f;
glm::mat4 myMatrix, resizeMatrix;
GLfloat points[NMAX];
GLfloat circle1[nrVertices];
GLfloat circle2[nrVertices];

void CreateVBO(void)
{
    static GLfloat vertex_color[] =
    {
        0.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.95f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.95f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.95f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.95f, 0.0f, 1.0f,

        1.0f, 0.95f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.95f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.95f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.95f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,
    };

    // indices to draw the star
    static const GLuint vertex_index[] =
    {
        0, 8, 9, 9, 0, 1,
        1, 9, 10, 10, 1, 2,
        2, 10, 11, 11, 2, 3,
        3, 11, 12, 12, 3, 4,
        4, 12, 13, 13, 4, 5,
        5, 13, 14, 14, 5, 6,
        6, 14, 15, 15, 6, 7,
        7, 15, 8, 8, 7, 0,
    };

    glGenBuffers(1, &VboId);
    glGenBuffers(1, &EboId);
    glGenVertexArrays(1, &VaoId);

    glBindVertexArray(VaoId);

    glBindBuffer(GL_ARRAY_BUFFER, VboId);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_color) + sizeof(points), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(vertex_color), vertex_color);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_index), vertex_index, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)sizeof(points));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void DestroyVBO(void)
{
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &EboId);
    glDeleteBuffers(1, &VboId);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VaoId);
}

void CreateShaders(void)
{
    ProgramId = LoadShaders("06_02_Shader.vert", "06_02_Shader.frag");
    glUseProgram(ProgramId);
}

void DestroyShaders(void)
{
    glDeleteProgram(ProgramId);
}

void CreateCircleArray(float radius, GLfloat circle[]) {
    float x, y;
    float centerX = 0, centerY = 0;
    float theta = 0;
    
    for (int i = 0; i < nrVertices; i++) {
        theta = i * TWO_PI / nrVertices;
        x = radius * cos(theta);
        y = radius * sin(theta);
        circle[i * 3] = centerX + x;
        circle[i * 3 + 1] = centerY + y;
        circle[i * 3 + 2] = 0.0f;
    }
}

void CreatePointArray(float radius1, float radius2) {
    float x1, y1, x2, y2;
    float centerX = 0, centerY = 0;
    float theta = 0;
    for (int i = 0; i < n; i++) {
        theta = i * TWO_PI / n;
        x1 = radius1 * cos(theta);
        y1 = radius1 * sin(theta);
        x2 = radius2 * cos(theta);
        y2 = radius2 * sin(theta);        
        points[i * 4] = centerX + x1;
        points[i * 4 + 1] = centerY + y1;
        points[i * 4 + 2] = 0.0f;
        points[i * 4 + 3] = 1.0f;
        points[4 * n + i * 4] = centerX + x2;
        points[4 * n + i * 4 + 1] = centerY + y2;
        points[4 * n + i * 4 + 2] = 0.0f;
        points[4 * n + i * 4 + 3] = 1.0f;
    }
}

void Initialize(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    CreateShaders();
}

void DrawCircle(GLfloat circle[]) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, circle);
    glDrawArrays(GL_LINE_LOOP, 0, nrVertices);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void RenderFunction(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    resizeMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / width, 1.f / height, 1.0));
    myMatrix = resizeMatrix;
    myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

    glLineWidth(5.0f);

    CreateCircleArray(300.0f, circle1);
    DrawCircle(circle1);

    CreateCircleArray(150.0f, circle2);
    DrawCircle(circle2);

    CreatePointArray(300.0f, 150.0f);
    CreateVBO();
    glDrawElements(GL_LINE_LOOP, NMAX, GL_UNSIGNED_INT, (void*)(0));

    DestroyVBO();
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
    glutCreateWindow("Using vertex indexing");
    glewInit();

    Initialize();
    glutDisplayFunc(RenderFunction);
    glutCloseFunc(Cleanup);
    glutMainLoop();
}
