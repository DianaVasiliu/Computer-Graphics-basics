#version 400

in vec4 in_Position;
in vec4 in_Color;

out vec4 gl_Position; 
out vec4 ex_Color;
uniform mat4 myMatrix;
uniform mat4 resizeMatrix;
uniform mat4 matrMove;
uniform mat4 matrScale;
uniform int codeProduct;

void main(void)
{

    if (codeProduct == 0) 
        gl_Position = myMatrix * in_Position;
    
    if (codeProduct == 1) 
        gl_Position = 
            (resizeMatrix * matrMove * matrScale) 
            * in_Position;
    
    ex_Color = in_Color;
} 
