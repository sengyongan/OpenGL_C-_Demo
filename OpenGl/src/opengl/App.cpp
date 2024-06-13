#include "opengl/App.h"

#include"opengl/Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Opengl {

	// timing
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	App* App::s_Instance = nullptr;

	App::App()
	{
		//init
		s_Instance = this;

		m_Window = std::make_unique<Window>();
		m_Window->Init();
		Renderer::init();
		m_EditorCamera = std::make_unique<EditorCamera>(30.0f, 1.778f, 0.1f, 1000.0f);
		
	}
	App::~App()
	{
		m_Window->~Window();
	}
	void App::Run()
	{
		while(!glfwWindowShouldClose(m_Window->GetNativeWindow())){
			// ts
			float currentFrame = static_cast<float>(glfwGetTime());//��ȡ��ǰʱ��
			deltaTime = currentFrame - lastFrame;//ʱ�=��ǰ - ��һ֡��
			lastFrame = currentFrame;//��һ֡

			// ------
			Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			Renderer::Clear();

			m_EditorCamera->OnUpdate(deltaTime);
			Renderer::BeginScene(*m_EditorCamera);
			Renderer::EndScene();
			//
			m_Window->OnUpdate();
		}
	}
}