#pragma once
//�û�����

#include"hzpch.h"

namespace Opengl {
	class Input
	{
    public:
        static bool IsKeyPressed(int keycode);//�Ƿ��¼���
        static bool IsMouseButtonPressed(int button);//�Ƿ������
        static std::pair<float, float>GetMousePosition();//��ȡ���λ��
        static float GetMouseX();//��ȡ���x
        static float GetMouseY();

	};

}
