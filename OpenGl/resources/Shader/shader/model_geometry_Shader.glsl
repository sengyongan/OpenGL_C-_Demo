#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 v_Position;
	vec2 v_TexCoord;
	vec3 v_Normal;
} gs_in[];

out vec3 v_Position;
out vec2 v_TexCoord;
out vec3 v_Normal;

uniform float time;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction, 0.0);
}

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main() {    
    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    v_TexCoord = gs_in[0].v_TexCoord;
    v_Position = gs_in[0].v_Position;
    v_Normal = gs_in[0].v_Normal;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    v_TexCoord = gs_in[1].v_TexCoord;
    v_Position = gs_in[0].v_Position;
    v_Normal = gs_in[0].v_Normal;

    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    v_TexCoord = gs_in[2].v_TexCoord;
    v_Position = gs_in[0].v_Position;
    v_Normal = gs_in[0].v_Normal;

    EmitVertex();
    EndPrimitive();
}