#pragma once
//��Ⱦ
#include"VertexArray.h"
#include"EditorCamera.h"

#include <string>
#include <glm/glm.hpp>

namespace Opengl {
	class Renderer {
	public:
		static void BeginScene(const EditorCamera& camera);//��Ϊ��Ⱦ�߼�ֻ����һ�ݣ�����Ҫ���ʵ��
		static void init();
		static void EndScene();

		static void SetClearColor(const glm::vec4& color);
		static void Clear();

		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);

	};
}