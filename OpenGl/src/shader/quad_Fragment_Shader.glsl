#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoord;

uniform vec4 ourColor;
uniform sampler2D texture1;

void main()
{
    vec4 texColor = texture(texture1, v_TexCoord);
    if(texColor.a < 0.1)
        discard;
	FragColor = texColor * ourColor;
}