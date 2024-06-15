#version 330 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

out vec3 v_Position;
out vec2 v_TexCoord;
out vec3 v_Normal;

uniform mat4 model;
uniform mat4 u_ViewProjection;

void main()
{
	v_TexCoord = a_TexCoord;
	v_Position = vec3(model * vec4(a_Position, 1.0));
    v_Normal = mat3(transpose(inverse(model))) * a_Normal;  

    gl_Position = u_ViewProjection * model * vec4(a_Position, 1.0);
}