#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;//��Ⱦ��6������

uniform mat4 shadowMatrices[6];

out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; //������Ⱦ������������
        for(int i = 0; i < 3; ++i) // �����ε�3������
        {
            FragPos = gl_in[i].gl_Position;//����ռ�����
            gl_Position = shadowMatrices[face] * FragPos;//��Ⱦ��6������
            EmitVertex();
        }    
        EndPrimitive();
    }
} 