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
#include "SOIL/SOIL.h"

using namespace std;

//////////////////////////////////////

GLuint
    VaoId,
    VboId,
    EboId,
    ProgramId,
    myMatrixLocation,
    viewLocation,
    projLocation;

GLuint texture;

float PI = 3.141592;
glm::mat4 myMatrix, matrRot;

float Obsx = 0.0, Obsy = 0.0, Obsz = 800.f;
float Refx = 0.0f, Refy = 0.0f;
float Vx = 0.0;
glm::mat4 view;

float width = 800, height = 600, xwmin = -800.f, xwmax = 800, ywmin = -600, ywmax = 600, znear = 0, zfar = 1000, fov = 45;
glm::mat4 projection;

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
        // znear += 10;
        // zfar += 10;
        Obsz += 10;
        break;
    case '-':
        // znear -= 10;
        // zfar -= 10;
        Obsz -= 10;
        break;
    }
    if (key == 27)
        exit(0);
}

void processSpecialKeys(int key, int xx, int yy) {

    switch (key) {
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
    GLfloat Vertices[] = {
        // vertex coordinates           // colors           // texturing coordinates
        -50.0f, -50.0f, 0.0f, 1.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // down left
        50.0f, -50.0f, 0.0f, 1.0f,      0.0f, 1.0f, 0.0f,	2.0f, 0.0f,  // down right
        50.0f, 50.0f, 0.0f, 1.0f,       1.0f, 1.0f, 0.0f,	2.0f, 2.0f, // up right
        -50.0f, 50.0f, 0.0f, 1.0f,      0.0f, 1.0f, 1.0f,	0.0f, 2.0f  // up left
    };

    GLuint Indices[] = {
      0, 1, 2,  // the first triangle
      0, 2, 3   // the second triangle
    };

    glGenVertexArrays(1, &VaoId);
    glGenBuffers(1, &VboId);
    glGenBuffers(1, &EboId);

    glBindVertexArray(VaoId);
    
    glBindBuffer(GL_ARRAY_BUFFER, VboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
}

void DestroyVBO(void)
{
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &VboId);
    glDeleteBuffers(1, &EboId);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VaoId);
}

void LoadTexture(void)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height;
    unsigned char* image = SOIL_load_image("text_smiley_face.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void CreateShaders(void)
{
    ProgramId = LoadShaders("06_04_Shader.vert", "06_04_Shader.frag");
    glUseProgram(ProgramId);
}

void DestroyShaders(void)
{
    glDeleteProgram(ProgramId);
}

void Initialize(void)
{
    myMatrix = glm::mat4(1.0f);
    matrRot = glm::rotate(glm::mat4(1.0f), PI / 8, glm::vec3(0.0, 0.0, 1.0));

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}

void RenderFunction(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // changing the observer's position
    glm::vec3 Obs = glm::vec3(Obsx, Obsy, Obsz);

    // changing the reference point position
    Refx = Obsx; Refy = Obsy;
    glm::vec3 PctRef = glm::vec3(Refx, Refy, -1.0f);

    // the vertical vector
    glm::vec3 Vert = glm::vec3(Vx, 1.0f, 0.0f);

    view = glm::lookAt(Obs, PctRef, Vert);
    myMatrix = view;

    //  projection = glm::ortho(xwmin, xwmax, ywmin, ywmax, znear, zfar);
    // projection = glm::frustum(xwmin, xwmax, ywmin, ywmax, znear, zfar);
    projection = glm::perspective(fov, GLfloat(width) / GLfloat(height), znear, zfar);
    myMatrix = glm::mat4(1.0f);
    CreateVBO();
    LoadTexture();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // myMatrix= matrRot;
    CreateShaders();

    // uniform variables for the vertex shader
    myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
    glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
    viewLocation = glGetUniformLocation(ProgramId, "view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
    projLocation = glGetUniformLocation(ProgramId, "projection");
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);
    
    // uniform variables for the fragment shader
    glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    DestroyVBO();
    DestroyShaders();

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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1200, 900);
    glutCreateWindow("Using textures");
    glewInit();

    Initialize();
    glutDisplayFunc(RenderFunction);
    glutIdleFunc(RenderFunction);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);
    glutCloseFunc(Cleanup);
    glutMainLoop();
}
