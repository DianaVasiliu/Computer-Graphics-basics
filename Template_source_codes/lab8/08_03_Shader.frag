#version 400

in vec4 ex_Color;
uniform int colCode;
 
out vec4 out_Color;

void main(void)
{
    switch (colCode) {
    case 1: 
        out_Color = vec4(0.0, 0.0, 0.0,0.0); 
        break;
    default: 
        out_Color = ex_Color;
    }
}
