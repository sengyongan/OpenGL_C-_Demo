#version 330 core
out vec4 FragColor;

in vec3 g_Color;

void main()
{
    FragColor = vec4(g_Color, 1.0);   
}