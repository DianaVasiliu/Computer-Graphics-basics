#version 400

in vec4 ex_Color;
in vec2 tex_Coord;
out vec4 out_Color;

vec4 green = vec4(0.2, 0.7, 0.2, 1.0);

uniform sampler2D myTexture;
uniform int colCode;

void main(void)
{
    if (colCode == 0)
        out_Color = ex_Color;
    if (colCode == 1)
        out_Color = green;
    if (colCode == 2)
        out_Color = texture(myTexture, tex_Coord);
}
