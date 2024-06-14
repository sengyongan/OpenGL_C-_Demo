#include"hzpch.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Opengl {
	Texture::Texture(const std::string& path)
		: m_Path(path)
	{
		// load and create a m_RendererID 
				// -------------------------
		stbi_set_flip_vertically_on_load(1);//��תΪ��ȷͼ��
		int width, height, channels;//����ͼ���ļ�������ͼ�����ݴ洢��dataָ���У���ȡͼ��Ŀ��ȡ��߶Ⱥ�ͨ������
			
		glGenTextures(1, &m_RendererID);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);//����������ȡͼ���ļ����������Ϊ�������ݡ�
		
		if(data)
		{
			m_IsLoaded = true;

			m_Width = width;
			m_Height = height;
			//channels
			GLenum  dataFormat = 0;//Format��ʽ
			if (channels == 1) {//ͼƬ��͸��ͨ��
				dataFormat = GL_RED;
			}
			else if (channels == 3) {
				dataFormat = GL_RGB;
			}
			else if (channels == 4) {
				dataFormat = GL_RGBA;
			}
			m_DataFormat = dataFormat;

			////////////////////////////////
			glBindTexture(GL_TEXTURE_2D, m_RendererID); // all upcoming GL_TEXTURE_2D operations now have effect on this m_RendererID object

			// set the m_RendererID wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// set m_RendererID filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//Ϊ�󶨵��������󴴽�����
			//(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
			
			glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}
	Texture::~Texture()
	{
		glDeleteTextures(1, &m_RendererID);
	}
	void Texture::Bind(uint32_t slot) const
	{
		glBindTexture(GL_TEXTURE_2D, m_RendererID);//solt��OpenGLTexture2D��д�г�ʼΪ0

	}
}