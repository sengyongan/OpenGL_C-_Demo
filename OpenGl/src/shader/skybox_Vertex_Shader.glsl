#version 330 core
layout (location = 0) in vec3 a_Position;

out vec3 v_TexCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    v_TexCoord = a_Position;
    vec4 pos = projection * view  * vec4(a_Position, 1.0);
    gl_Position = pos.xyww;
}  
//model:�����λ�ò��䣬��պ�û��model�������������ͬһ��λ�ã���Զ�����������������պ��ڣ������Ķ�����պ�
//view��������ת���������ת�ı���Ļ�ķ�ʽ����ʵ��������ת��������պл���ת���������񻷹�����
//project:���������ͶӰ����Ӱ�죬ͶӰ�������������ĵ���������ã�һ����������������Բ��䣬��Ҳ��Ҫ��ʱ��Ӱ��