#pragma once
//��ΪӦ��API

#include "core.h"
#include"Window.h"


namespace Opengl {
	
	class BUILD_API App {
	public:
		App();
		~App();
		void Run();

		//App* Get() { return s_Instance; }
	private:
		//�����
		Window m_Window;

	};
}