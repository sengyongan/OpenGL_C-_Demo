#include "VertexArray.h"

#include <glad/glad.h>

namespace Opengl {
	static GLenum ShaderDataTypeToBaseType(ShaderDataType type)//ShaderDataType�Զ������ͣ�תΪGL����
	{
		switch (type)
		{
		case Opengl::ShaderDataType::Float:    return GL_FLOAT;
		case Opengl::ShaderDataType::Float2:   return GL_FLOAT;
		case Opengl::ShaderDataType::Float3:   return GL_FLOAT;
		case Opengl::ShaderDataType::Float4:   return GL_FLOAT;
		case Opengl::ShaderDataType::Mat3:     return GL_FLOAT;
		case Opengl::ShaderDataType::Mat4:     return GL_FLOAT;
		case Opengl::ShaderDataType::Int:      return GL_INT;
		case Opengl::ShaderDataType::Int2:     return GL_INT;
		case Opengl::ShaderDataType::Int3:     return GL_INT;
		case Opengl::ShaderDataType::Int4:     return GL_INT;
		case Opengl::ShaderDataType::Bool:     return GL_BOOL;
		}

		return 0;
	}

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{

		glBindVertexArray(m_RendererID);//VAO��ʼ��-����������
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)//�Ե�ǰ����-����������-�����ò���
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);//��Ա����
	}

	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);//VAO��ʼ��-����������
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}