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
ProgramId,
viewMatrixLocation,
projectionMatrixLocation;
 
glm::mat4 view, projection; 

float PI = 3.141592;
int width = 1000, height = 800;
GLfloat fov = 70.f, znear = 1.0f;

#define	checkImageWidth 64
#define	checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLuint texName;

void makeCheckImage(void)
{
    int i, j, c;

    for (i = 0; i < checkImageHeight; i++) {
        for (j = 0; j < checkImageWidth; j++) {
            c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 200;

            checkImage[i][j][0] = (GLubyte)c;
            checkImage[i][j][1] = (GLubyte)c;
            checkImage[i][j][2] = (GLubyte)c;
            checkImage[i][j][3] = (GLubyte)255;
        }
    }
}

void CreateVBO(void)
{
    GLfloat   Vertices[] = {
        // vertex coordinates           // texture coordinates
        -1.7f, -0.8f, 0.0f, 1.0f,       0.0f, 0.0f,
        -0.1f, -0.8f, 0.0f, 1.0f,       0.0f, 1.0f,
        -0.1f, 0.8f, 0.0f, 1.0f,        1.0f, 1.0f,
        -1.7f, 0.8f, 0.0f, 1.0f,        1.0f, 0.0f,
         1.0f, -1.6f, -3.0f, 1.0f,      0.0f, 0.0f,
         2.41421f, -1.6f, -1.41421f,    1.0f, 1.0f, 0.0f,
         2.41421f, 1.6f, -1.41421f,     1.0f, 1.0f, 1.0f,
         1.0f, 1.6f, -3.0f, 1.0f,       0.0f, 1.0f
    };
 
    GLuint Indices[] = {
        0, 1, 2,  
        0, 2, 3,  
        4, 5, 6,
        4, 6, 7
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
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
 
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
 
    // attribute = 2 => texture coordinates
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
}

void DestroyVBO(void)
{   
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VboId);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VaoId);

    glDeleteBuffers(1, &EboId);
}

void CreateShaders(void)
{
    ProgramId = LoadShaders("06_03_Shader.vert", "06_03_Shader.frag");
    glUseProgram(ProgramId);
}
 
void DestroyShaders(void)
{
    glDeleteProgram(ProgramId);
}
 
void LoadTexture()
{   
    makeCheckImage();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight,
        0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Initialize(void)
{
   glClearColor(0.0f, 0.1f, 9.0f, 0.0f);
   CreateVBO();
   CreateShaders(); 
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    projection = glm::infinitePerspective(fov, (GLfloat)w / (GLfloat)h, znear);
    projectionMatrixLocation = glGetUniformLocation(ProgramId, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projection[0][0]);
}

void RenderFunction(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    view = glm::lookAt( glm::vec3(0.0f, 0.0f, 3.f), 
  		                glm::vec3(0.0f, 0.0f, -20.0f), 
  		                glm::vec3(0.0f, 1.0f, 0.0f));
     
    viewMatrixLocation = glGetUniformLocation(ProgramId, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE,  &view[0][0]);
    
    glEnable(GL_TEXTURE_2D);
    LoadTexture();
    glActiveTexture(GL_TEXTURE0);
 
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texName);
    glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
    glDisable(GL_TEXTURE_2D);

    glutPostRedisplay();
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
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition(100, 100); 
    glutInitWindowSize(800,600); 
    glutCreateWindow("2D Textures"); 
    glewInit(); 
    
    Initialize();
    glutDisplayFunc(RenderFunction);
    glutReshapeFunc(reshape);
    glutCloseFunc(Cleanup);
    glutMainLoop();
}
