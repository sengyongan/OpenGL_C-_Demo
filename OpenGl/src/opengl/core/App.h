#pragma once
//��ΪӦ��API

#include "core.h"
#include"Window.h"
#include"EditorCamera.h"

namespace Opengl {
	
	class BUILD_API App {
	public:
		App();
		~App();
		void Run();

		static App& Get() { return *s_Instance; }
		Window& GetWindow() { return  *m_Window; }//���������ָ������ã���ֹ��������
		EditorCamera& GetCamera() { return *m_EditorCamera; }

	private:
		//�����
		static App* s_Instance;//Ψһ��Ӧ�ó�����

		std::unique_ptr<Window> m_Window;//

		std::unique_ptr<EditorCamera> m_EditorCamera;

		//Window* m_Window;

	};
}