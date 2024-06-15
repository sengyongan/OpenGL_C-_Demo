#pragma once
#include"Shader.h"
#include"hzpch.h"
#include"VertexArray.h"
#include"Renderer.h"

namespace Opengl {
	class Draw
	{
	public:
		virtual ~Draw() {}

		virtual void Bind() const = 0;//����VAO��VBO��EBO��
		virtual void OnDraw(const std::shared_ptr<Shader>&  shader) const = 0;//��VAO������(��shared_ptr���Ͷ�������ã�
	};


}