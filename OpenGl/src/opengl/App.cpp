#include "opengl/App.h"

#include"opengl/Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Opengl {
	// settings
	// timing
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	//float vertices[3 * 7] = {//position + color
	//	-0.5f, -0.5f, 0.0f,		0.8f, 0.2f, 0.8f, 1.0f,
	//	 0.5f, -0.5f, 0.0f,		0.2f, 0.3f, 0.8f, 1.0f,
	//	 0.0f,  0.5f, 0.0f,		0.8f, 0.8f, 0.2f, 1.0f
	//};
	//float squareVertices[] = {//position + texture���£����£����ϣ�����
	//	-0.75f, -0.75f, 0.0f,	0.0f, 0.0f,
	//	 0.75f, -0.75f, 0.0f,	1.0f, 0.0f,
	//	 0.75f,  0.75f, 0.0f,	1.0f, 1.0f,
	//	-0.75f,  0.75f, 0.0f,	0.0f, 1.0f
	//};

	////��������
	//unsigned int indices[3] = { 0, 1, 2 };

	//unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	App* App::s_Instance = nullptr;

	App::App()
	{
		//init
		s_Instance = this;

		m_Window = std::make_unique<Window>();
		//m_Window = new Window();
		m_Window->Init();
		Renderer::init();
		m_EditorCamera = std::make_unique<EditorCamera>(30.0f, 1.778f, 0.1f, 1000.0f);

		//shader
		//m_Shader.reset(new Shader("D:\\OpenGL_C++_Demo\\OpenGl_Demo\\OpenGl\\src\\shader\\vs.vs", "D:\\OpenGL_C++_Demo\\OpenGl_Demo\\OpenGl\\src\\shader\\fs.fs"));//��������ָ��ָ��Ķ���
		//m_BlueShader.reset(new Shader("D:\\OpenGL_C++_Demo\\OpenGl_Demo\\OpenGl\\src\\shader\\blue_Vertex_Shader.glsl", "D:\\OpenGL_C++_Demo\\OpenGl_Demo\\OpenGl\\src\\shader\\blue_Fragment_Shader.glsl"));
		
		//texture
		//m_Texture = std::make_unique<Texture>("D:\\OpenGL_C++_Demo\\OpenGl_Demo\\OpenGl\\src\\assest\\textures\\container.jpg");
		//m_Texture_Bround = std::make_unique<Texture>("D:\\OpenGL_C++_Demo\\OpenGl_Demo\\OpenGl\\src\\assest\\textures\\awesomeface.png");
		
		/////////////////////////////////////////////////////////////////////////
		///////Frist///////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////
		//m_VertexArray = std::make_unique<VertexArray>();

		//std::shared_ptr<VertexBuffer> vertexBuffer;//��Ϊȫ�ֱ���ֻ��ҪVAO����
		//vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		//BufferLayout layout = {
		//			{ ShaderDataType::Float3, "a_Position" },
		//			{ ShaderDataType::Float4, "a_Color" }
		//};

		//vertexBuffer->SetLayout(layout);
		////���в���
		//m_VertexArray->AddVertexBuffer(vertexBuffer);

		////m_IndexBuffer = std::make_unique<OpenGLIndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));
		//std::shared_ptr<IndexBuffer> indexBuffer;
		//indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));//��ȡ����
		//m_VertexArray->SetIndexBuffer(indexBuffer);
		/////////////////////////////////////////////////////////////////////////
		///////Second///////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////


		//m_SquareVA = std::make_unique<VertexArray>();

		//std::shared_ptr<VertexBuffer> squareVB;
		//squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		//squareVB->SetLayout(
		//	{
		//		{ ShaderDataType::Float3, "a_Position" },
		//		{ ShaderDataType::Float2, "a_TexCoord" }
		//	}
		//);

		//m_SquareVA->AddVertexBuffer(squareVB);

		//std::shared_ptr<IndexBuffer> squareIB;
		//squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

		//m_SquareVA->SetIndexBuffer(squareIB);

        // glsl------------------------------------------------------------------
		//m_BlueShader->Bind();
		//m_BlueShader->SetInt("texture1", 0);
		//m_BlueShader->SetInt("texture2", 1);
		
	}
	App::~App()
	{
		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		m_Window->~Window();

	}
	void App::Run()
	{
		while(!glfwWindowShouldClose(m_Window->GetNativeWindow())){
			// ts
			float currentFrame = static_cast<float>(glfwGetTime());//��ȡ��ǰʱ��
			deltaTime = currentFrame - lastFrame;//ʱ�=��ǰ - ��һ֡��
			lastFrame = currentFrame;//��һ֡

            // input
			//if (glfwGetKey(m_Window->m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			//	glfwSetWindowShouldClose(m_Window->GetNativeWindow(), true);
			// ------
			Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			Renderer::Clear();

			//// render����
			//float timeValue = glfwGetTime();
			//float greenValue = (sin(timeValue) / 2.0f) + 0.5f;//sinֵ��Ϊ��-1����1����/2+0.5-��0����1
			//glm::vec4 result = glm::vec4(0.0f, greenValue, 0.0f, 1.0f);
			//
			//glActiveTexture(GL_TEXTURE0);
			//m_Texture->Bind();
			//glActiveTexture(GL_TEXTURE1);
			//m_Texture_Bround->Bind();
			m_EditorCamera->OnUpdate(deltaTime);
			Renderer::BeginScene(*m_EditorCamera);
			//m_BlueShader->Bind();
			//m_BlueShader->SetFloat4("ourColor", result);

			//m_SquareVA->Bind();


			//Renderer::DrawIndexed(m_SquareVA);

			////render////////////////////

			////������
			//// create transformations
			//glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			//glm::mat4 view = glm::mat4(1.0f);
			//glm::mat4 projection = glm::mat4(1.0f);
			//model = glm::rotate(model, glm::radians(-70.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
			//projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);


   //         m_Shader->Bind();

			//m_Shader->SetMat4("model", model);
			//m_Shader->SetMat4("view", view);
			//m_Shader->SetMat4("projection", projection);

			//m_VertexArray->Bind();// seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			//Renderer::DrawIndexed(m_VertexArray);
			Renderer::EndScene();
			//
			m_Window->OnUpdate();
		}
	}
}