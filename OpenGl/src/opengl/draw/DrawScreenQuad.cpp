#include "DrawScreenQuad.h"

namespace Opengl {
	//quad
	float ScreenVertices[] = {//position + texture���ϣ����£����£�����
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f 
	};
	//quad
	unsigned int screen_Indices[6] = { 0, 1, 2, 2, 3, 0 };


	struct ScreenData {
		std::shared_ptr<VertexArray> ScreenVertexArray;
	};
	static ScreenData s_ScreenData;

	void DrawScreenQuad::Bind() const
	{
		s_ScreenData.ScreenVertexArray = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> point_VertexBuffer;
		point_VertexBuffer.reset(VertexBuffer::Create(ScreenVertices, sizeof(ScreenVertices)));

		point_VertexBuffer->SetLayout(
			{
					{ ShaderDataType::Float2, "a_Position" },
					{ ShaderDataType::Float2, "a_TexCoord" }
			}
		);

		s_ScreenData.ScreenVertexArray->AddVertexBuffer(point_VertexBuffer);

		std::shared_ptr<IndexBuffer> point_IndexBuffer;
		point_IndexBuffer.reset(IndexBuffer::Create(screen_Indices, sizeof(screen_Indices) / sizeof(unsigned int)));

		s_ScreenData.ScreenVertexArray->SetIndexBuffer(point_IndexBuffer);

	}
	void DrawScreenQuad::OnDraw(const std::shared_ptr<Shader>& shader) const
	{
		//s_ScreenData.ScreenVertexArray->Bind();
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		Renderer::DrawIndexed(s_ScreenData.ScreenVertexArray);

	}
}