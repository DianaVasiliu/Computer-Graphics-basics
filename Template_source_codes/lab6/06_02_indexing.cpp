/* INDEXING
NEW ELEMENTS:
    - using the indices: associated elements (matrices, buffer)
    - using the 4 drawing functions (glDrawArrays, glDrawElements, glDrawElementsBaseVertex, glDrawArraysInstanced) 
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

//////////////////////////////////////

GLuint
    VaoId,
    VboId,
    EboId,
    ColorBufferId,
    ProgramId,
    myMatrixLocation;

float width = 80.f, height = 60.f;
glm::mat4 myMatrix, resizeMatrix = glm::ortho(-width, width, -height, height);

void CreateVBO(void)
{
    static const GLfloat vertex_pos[] =
    {
        -5.0f, -5.0f, 0.0f, 1.0f,
         5.0f,  -5.0f, 0.0f, 1.0f,
         5.0f,  5.0f, 0.0f, 1.0f,
        -5.0f,  5.0f, 0.0f, 1.0f,
    };

    
    static const GLfloat vertex_color[] =
    {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f,
    };
    
    // indices for drawing a triangle
    static const GLuint vertex_index[] =
    {
        0, 1, 2, 2, 3, 0
    };
 
    glGenBuffers(1, &VboId);
    glGenBuffers(1, &EboId);
    
    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);

    glBindBuffer(GL_ARRAY_BUFFER, VboId);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_color) + sizeof(vertex_pos), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_pos), vertex_pos);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertex_pos), sizeof(vertex_color), vertex_color);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_index), vertex_index, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)sizeof(vertex_pos));
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
    ProgramId = LoadShaders("06_02_Shader.vert", "06_02_Shader.frag");
    glUseProgram(ProgramId);
}

void DestroyShaders(void)
{
    glDeleteProgram(ProgramId);
}

void Initialize(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    CreateShaders();
}

void RenderFunction(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    CreateVBO();
    myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
    
    // drawing
    myMatrix = resizeMatrix;
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0));
    
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
    glutCreateWindow("Using the vertex indexing");
    glewInit();
    
    Initialize();
    glutDisplayFunc(RenderFunction);
    glutCloseFunc(Cleanup);
    glutMainLoop();
}
