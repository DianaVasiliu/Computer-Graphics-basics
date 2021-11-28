#version 400

in vec4 ex_Color;
uniform int colCode;
 
out vec4 out_Color;

void main(void)
{
	if (colCode == 0)
		out_Color = ex_Color;
	if (colCode == 1)
		out_Color=vec4 (0.0, 0.0, 1.0, 0.0);
	if (colCode == 2)
		out_Color=vec4 (1.0, 0.0, 0.0, 0.0);
}
 