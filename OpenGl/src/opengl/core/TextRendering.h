#pragma once

#include"hzpch.h"
#include "core.h"
#include"opengl/renderer/Shader.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>


namespace Opengl {

	class BUILD_API TextRendering {

	public:
		TextRendering();
		void init();
		void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);

	public:
		FT_Library ft;
		FT_Face face;
		
		struct Character {
			GLuint     TextureID;  // ���������ID
			glm::ivec2 Size;       // ���δ�С
			glm::ivec2 Bearing;    // �ӻ�׼�ߵ�������/������ƫ��ֵ
			GLuint     Advance;    // ԭ�����һ������ԭ��ľ���
		};

		std::map<GLchar, Character> Characters;

		unsigned int VAO, VBO;
	};
}