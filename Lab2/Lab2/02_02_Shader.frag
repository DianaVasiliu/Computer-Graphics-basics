#version 400

in vec4 ex_Color;
out vec4 out_Color;
uniform int colorCode;

void main(void)
{
    switch (colorCode) {
	case 0:
		out_Color = ex_Color;
		break;
	default:
		out_Color = vec4 (1.0, 0.0, 1.0, 0.0);
	}
}
 