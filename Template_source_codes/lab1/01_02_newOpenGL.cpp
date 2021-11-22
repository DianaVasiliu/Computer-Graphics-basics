// The source code is adapted after OpenGLBook.com

#include <windows.h>
#include <stdlib.h> // useful for the shader reading
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
  
//////////////////////////////////////

GLuint
    VaoId,
    VboId,
    ColorBufferId,
    VertexShaderId,
    FragmentShaderId,
    ProgramId;

///////////////////////////////////////////

// the vertex shader (interpreted as a string)
const GLchar* VertexShader =
{
    "#version 400\n"\

    "layout(location=0) in vec4 in_Position;\n"\
    "layout(location=1) in vec4 in_Color;\n"\
    "out vec4 ex_Color;\n"\

    "void main(void)\n"\
    "{\n"\
    "  gl_Position = in_Position;\n"\
    "  ex_Color = in_Color;\n"\
    "}\n"
};

// the fragment shader (interpreted as a string)
const GLchar* FragmentShader =
{
    "#version 400\n"\

    "in vec4 ex_Color;\n"\
    "out vec4 out_Color;\n"\

    "void main(void)\n"\
    "{\n"\
    "  out_Color = ex_Color;\n"\
    "}\n"
};
 
void CreateVBO(void)
{
  // the vertices
    GLfloat Vertices[] = {
        -0.8f, -0.8f, 0.0f, 1.0f,
        0.0f,  0.8f, 0.0f, 1.0f,
        0.8f, -0.8f, 0.0f, 1.0f
    };

    // the colors, as attributes for the vertices
    GLfloat Colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f
    };
 

    // creating a new buffer
    glGenBuffers(1, &VboId);
  
    // setting the buffer as the current buffer
    glBindBuffer(GL_ARRAY_BUFFER, VboId);
  
    // the vertices are "copied" in the current buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  
    // creating / linking a VAO (Vertex Array Object) - useful when using more VBOs
    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);
    
    // activating working with attributes
    // attribute 0 = position
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
 
    // a new buffer, for colors
    glGenBuffers(1, &ColorBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  
    // attribute 1 = color
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
    VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShaderId, 1, &VertexShader, NULL);
    glCompileShader(VertexShaderId);

    FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShaderId, 1, &FragmentShader, NULL);
    glCompileShader(FragmentShaderId);

    ProgramId = glCreateProgram();
    glAttachShader(ProgramId, VertexShaderId);
    glAttachShader(ProgramId, FragmentShaderId);
    glLinkProgram(ProgramId);
    glUseProgram(ProgramId);
}

void DestroyShaders(void)
{
    glUseProgram(0);

    glDetachShader(ProgramId, VertexShaderId);
    glDetachShader(ProgramId, FragmentShaderId);

    glDeleteShader(FragmentShaderId);
    glDeleteShader(VertexShaderId);

    glDeleteProgram(ProgramId);
}
 
void Initialize(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // background color
}

void RenderFunction(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    CreateVBO();
    CreateShaders();
    
    glPointSize(20.0);
    glDrawArrays(GL_POINTS, 0, 3);
    
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
    glutInitWindowPosition (100,100); // window initial position
    glutInitWindowSize(700,500); // window dimenstions
    glutCreateWindow("Second triangle - OpenGL << new >>"); // window title
    glewInit(); // don't forget to initialize glew; it must be initialized before initializing the drawing
    Initialize();
    glutDisplayFunc(RenderFunction);
    glutCloseFunc(Cleanup);
    glutMainLoop();
}
