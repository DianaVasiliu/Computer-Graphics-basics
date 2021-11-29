#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include "loadShaders.h"

#include "glm/glm/glm.hpp"  
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "SOIL/SOIL.h"

using namespace std;

//////////////////////////////////////

GLuint
    VaoId,
    VboId,
    EboId,
    ProgramId,
    colCodeLocation,
    myMatrixLocation;

GLuint texture;

int colCode;
float width = 80.f, height = 60.f;
glm::mat4
myMatrix,
resizeMatrix = glm::ortho(-width, width, -height, height),
scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 0.5f, 0.0f)),
translMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 30.0f, 0.0f));

void CreateVBO(void)
{
    static const GLfloat vertex_pos[] =
    {
        // vertex coordinates           // colors               // texturing coordinates
        -5.0f, -5.0f, 0.0f, 1.0f,       0.0f, 0.0f, 0.0f,       0.0f, 0.0f,
        5.0f,  -5.0f, 0.0f, 1.0f,       0.0f, 0.0f, 0.0f,       0.3f, 0.0f,
        5.0f,  5.0f, 0.0f, 1.0f,        0.0f, 0.0f, 0.0f,       0.3f, 0.2f,
        -5.0f,  5.0f, 0.0f, 1.0f,       0.0f, 0.0f, 0.0f,       0.0f, 0.2f,
    };

    // indices to draw a rectangle
    static const GLuint vertex_index[] =
    {
        0, 1, 2, 2, 3, 0
    };

    glGenBuffers(1, &VboId);
    glGenBuffers(1, &EboId);
    glGenVertexArrays(1, &VaoId);

    glBindVertexArray(VaoId);

    glBindBuffer(GL_ARRAY_BUFFER, VboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_pos), vertex_pos, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_index), vertex_index, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
}

void LoadTexture(void)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height;
    unsigned char* image = SOIL_load_image("green_circle.png", &width, &height, 0, SOIL_LOAD_RGB);

    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "Texture loaded" << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
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
    ProgramId = LoadShaders("06_02_Shader_2.vert", "06_02_Shader_2.frag");
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
    LoadTexture();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Drawing
    
    // we apply the scaling first, then the translation
    myMatrix = resizeMatrix * translMatrix * scaleMatrix;
    colCode = 1;
    colCodeLocation = glGetUniformLocation(ProgramId, "colCode");
    glUniform1i(colCodeLocation, colCode);
    myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0));

    // we apply the translation first, then the scaling
    myMatrix = resizeMatrix * scaleMatrix * translMatrix;
    colCode = 2;
    colCodeLocation = glGetUniformLocation(ProgramId, "colCode");
    glUniform1i(colCodeLocation, colCode);
    myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
    glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0));

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
