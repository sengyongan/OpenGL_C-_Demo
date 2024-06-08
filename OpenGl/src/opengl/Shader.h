#pragma once
//��ɫ��
#include "core.h"

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace  Opengl {
	class Shader
	{
	public:
		Shader(const char* vertexPath, const char* fragmentPath);//��ɫ��Դ��·��������ļ�
		~Shader();

		void Bind() const;//������ɫ������
		void Unbind() const;

		unsigned int GetShaderProgram() { return m_RendererID; }
		//
		//
		void SetInt(const std::string& name, int value);
		void SetIntArray(const std::string& name, int* values, uint32_t count);
		void SetFloat(const std::string& name, float value);
		void SetFloat2(const std::string& name, const glm::vec2& value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetFloat4(const std::string& name, const glm::vec4& value);
		void SetMat4(const std::string& name, const glm::mat4& value);
		//
		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);


	private:
		unsigned int m_RendererID;//��ɫ������
	private:
		void checkCompileErrors(unsigned int shader, std::string type);//����Ƿ�������

	};

}

