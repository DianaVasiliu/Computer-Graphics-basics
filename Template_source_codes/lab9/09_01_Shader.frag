#version 400

in vec3 ex_Color; 
out vec3 out_Color;
uniform int colCode;

void main(void)
{
    switch (colCode) {
    case 1: 
        out_Color = vec3(0.0, 0.0, 0.0); 
        break;
    default: 
        out_Color = ex_Color;
    }
}
