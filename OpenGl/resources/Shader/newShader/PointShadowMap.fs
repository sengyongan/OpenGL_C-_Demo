#version 330 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
    float lightDistance = length(FragPos.xyz - lightPos);//����Ƭ�����ֵ
    
    // תΪ [0;1] ֮��
    lightDistance = lightDistance / far_plane;
    
    // д�����
    gl_FragDepth = lightDistance;
}