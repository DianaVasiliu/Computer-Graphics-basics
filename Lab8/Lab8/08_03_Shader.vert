#version 400

layout (location = 0) in vec4 in_Position; // the position is a standard attribute
layout (location = 1) in vec4 in_Color; // the color is an instantiated attribute
layout (location = 2) in mat4 modelMatrix; // the transformation matrix is an instantiated attribute

out vec4 gl_Position; 
out vec4 ex_Color;
 
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
 
void main(void)
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * in_Position;
	ex_Color = in_Color;
}
