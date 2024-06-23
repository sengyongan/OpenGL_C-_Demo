#pragma once
//layout&&VBO,EBO

#include"hzpch.h"
#include "opengl/core/core.h"

namespace  Opengl {
	//�Զ������ͱ�ǩ��float��Ӧvec����������
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};
	//��ǰ�������貽�������ֽڣ�
	static unsigned int ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return 4;
		case ShaderDataType::Float2:   return 4 * 2;//һ���ֽ��� * ����
		case ShaderDataType::Float3:   return 4 * 3;
		case ShaderDataType::Float4:   return 4 * 4;
		case ShaderDataType::Mat3:     return 4 * 3 * 3;
		case ShaderDataType::Mat4:     return 4 * 4 * 4;
		case ShaderDataType::Int:      return 4;
		case ShaderDataType::Int2:     return 4 * 2;
		case ShaderDataType::Int3:     return 4 * 3;
		case ShaderDataType::Int4:     return 4 * 4;
		case ShaderDataType::Bool:     return 1;
		}

		return 0;
	}
	//�Զ���layout�����ݽṹ��
	struct BufferElement
	{
		//λ��ֵIndex��ͨ��++������������1�������ͣ�����������������4���Ƿ����滯�����ܲ��������ƫ�ƣ�6������
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		std::string Name;
		ShaderDataType Type;//������3��
		unsigned int Size;
		unsigned int Offset;
		bool Normalized;

		BufferElement() {}

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		unsigned int GetComponentCount() const//����������2��
		{
			switch (Type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3 * 3;//����
			case ShaderDataType::Mat4:    return 4 * 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
			}

			return 0;
		}
	};
	//���岼�֣���ÿ���б��Ա���֣�
	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)//�б�ÿ����Ա�ṹ�壩
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();//��ʼ�����岼�֣��ͻ���㲽��
		}

		inline unsigned int GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:

		void CalculateOffsetsAndStride()//���㲽�����ܲ�����ƫ������
		{
			unsigned int offset = 0;//������6��
			m_Stride = 0;//������5��
			for (auto& element : m_Elements)//ѭ��ÿ��ֵ
			{
				element.Offset = offset;//��ǰelement�Ĳ���
				offset += element.Size;
				m_Stride += element.Size;//�����ܵĲ���
			}
		}
	private:
		std::vector<BufferElement> m_Elements;////�б�
		unsigned int m_Stride = 0;//��ʼ����
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		//
		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		//
		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual unsigned int GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t size);
	};
}
