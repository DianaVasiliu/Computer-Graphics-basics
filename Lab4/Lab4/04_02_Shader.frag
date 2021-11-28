#version 400

in vec4 ex_Color;
uniform int colCode;
 
out vec4 out_Color;

void main(void)
{
	if (colCode == 0)
		out_Color = ex_Color;
	else
		out_Color = vec4(1.0, 0.0, 1.0, 0.0);
}
