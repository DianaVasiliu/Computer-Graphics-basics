#version 400

in vec4 ex_Color;
out vec4 out_Color;
uniform int colorCode;

void main(void)
{
    if ( colorCode == 0 )
		out_Color = ex_Color;
    else
		out_Color = vec4(1.0, 0.0, 1.0, 0.0);
}
 