#pragma once
//渲染
#include"VertexArray.h"
#include"opengl/core/EditorCamera.h"
#include"opengl/core/Debug.h"

#include <string>
#include <glm/glm.hpp>

namespace Opengl {
	class Renderer {
	public:
		static void BeginScene(const EditorCamera& camera);//因为渲染逻辑只存在一份，不需要多个实例
		static void init();
		static void EndScene();

		static void SetClearColor(const glm::vec4& color);
		static void Clear();

		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);
		static void DrawPoints(const std::shared_ptr<VertexArray>& vertexArray);
		static void DrawLines(const std::shared_ptr<VertexArray>& vertexArray);
		
		static void DrawScene();
		static void renderCube();

		static void processInput(GLFWwindow* window);

		static void SSAOKernel();

	};
}