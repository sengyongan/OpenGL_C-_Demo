	#include "DrawCube.h"

namespace Opengl {
	//cube(postion, TexCoord, normal)
	float cube_Vertices[] = {//后，前，左，右，下，上（左下，右下，右上，右上，左上，左下）
		//-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
		// 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
		// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
		//-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,

		//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
		// 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f,  1.0f,
		// 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f,
		//-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f,

		//-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		//-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		//-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,

		// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
		// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
		// 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
		// 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,

		//-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		// 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		// 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,

		//-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		//-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f
			 // back face
			-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,    0.0f,  0.0f, -1.0f, // bottom-left
			 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,    0.0f,  0.0f, -1.0f, // top-right
			 0.5f, -0.5f, -0.5f,   1.0f, 0.0f,    0.0f,  0.0f, -1.0f, // bottom-right         
			 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,    0.0f,  0.0f, -1.0f, // top-right
			-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,    0.0f,  0.0f, -1.0f, // bottom-left
			-0.5f,  0.5f, -0.5f,   0.0f, 1.0f,    0.0f,  0.0f, -1.0f, // top-left
			// front face		  			    
			-0.5f, -0.5f,  0.5f,   0.0f, 0.0f,    0.0f,  0.0f,  1.0f, // bottom-left
			 0.5f, -0.5f,  0.5f,   1.0f, 0.0f,    0.0f,  0.0f,  1.0f, // bottom-right
			 0.5f,  0.5f,  0.5f,   1.0f, 1.0f,    0.0f,  0.0f,  1.0f, // top-right
			 0.5f,  0.5f,  0.5f,   1.0f, 1.0f,    0.0f,  0.0f,  1.0f, // top-right
			-0.5f,  0.5f,  0.5f,   0.0f, 1.0f,    0.0f,  0.0f,  1.0f, // top-left
			-0.5f, -0.5f,  0.5f,   0.0f, 0.0f,    0.0f,  0.0f,  1.0f, // bottom-left
			// left face		  			    
			-0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   -1.0f,  0.0f,  0.0f, // top-right
			-0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   -1.0f,  0.0f,  0.0f, // top-left
			-0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   -1.0f,  0.0f,  0.0f, // bottom-left
			-0.5f, -0.5f, -0.5f,   0.0f, 1.0f,   -1.0f,  0.0f,  0.0f, // bottom-left
			-0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   -1.0f,  0.0f,  0.0f, // bottom-right
			-0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   -1.0f,  0.0f,  0.0f, // top-right
			// right face		  			    
			 0.5f,  0.5f,  0.5f,   1.0f, 0.0f,    1.0f,  0.0f,  0.0f, // top-left
			 0.5f, -0.5f, -0.5f,   0.0f, 1.0f,    1.0f,  0.0f,  0.0f, // bottom-right
			 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,    1.0f,  0.0f,  0.0f, // top-right         
			 0.5f, -0.5f, -0.5f,   0.0f, 1.0f,    1.0f,  0.0f,  0.0f, // bottom-right
			 0.5f,  0.5f,  0.5f,   1.0f, 0.0f,    1.0f,  0.0f,  0.0f, // top-left
			 0.5f, -0.5f,  0.5f,   0.0f, 0.0f,    1.0f,  0.0f,  0.0f, // bottom-left     
			 // bottom face		  			    
			-0.5f, -0.5f, -0.5f,   0.0f, 1.0f,    0.0f, -1.0f,  0.0f, // top-right
			 0.5f, -0.5f, -0.5f,   1.0f, 1.0f,    0.0f, -1.0f,  0.0f, // top-left
			 0.5f, -0.5f,  0.5f,   1.0f, 0.0f,    0.0f, -1.0f,  0.0f, // bottom-left
			 0.5f, -0.5f,  0.5f,   1.0f, 0.0f,    0.0f, -1.0f,  0.0f, // bottom-left
			-0.5f, -0.5f,  0.5f,   0.0f, 0.0f,    0.0f, -1.0f,  0.0f, // bottom-right
			-0.5f, -0.5f, -0.5f,   0.0f, 1.0f,    0.0f, -1.0f,  0.0f, // top-right
			// top face			  			    
			-0.5f,  0.5f, -0.5f,   0.0f, 1.0f,    0.0f,  1.0f,  0.0f, // top-left
			 0.5f,  0.5f , 0.5f,   1.0f, 0.0f,    0.0f,  1.0f,  0.0f, // bottom-right
			 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,    0.0f,  1.0f,  0.0f, // top-right     
			 0.5f,  0.5f,  0.5f,   1.0f, 0.0f,    0.0f,  1.0f,  0.0f, // bottom-right
			-0.5f,  0.5f, -0.5f,   0.0f, 1.0f,    0.0f,  1.0f,  0.0f, // top-left
			-0.5f,  0.5f,  0.5f,   0.0f, 0.0f,    0.0f,  1.0f,  0.0f  // bottom-left       
	};
	//cube
	//unsigned int cube_Indices[36] = {
	//	0, 1, 2, 2, 3, 0 ,
	//	4,5,6,6,7,4,
	//	8, 9, 10, 10, 11, 8,
	//	12, 13, 14, 14, 15, 12,
	//	16, 17, 18, 18, 19, 16,
	//	20, 21, 22, 22, 23, 20
	//};

	struct CubeData {
		std::shared_ptr<VertexArray> CubeVertexArray;
	};
	static CubeData s_CubeData;

	void DrawCube::Bind() const
	{
		s_CubeData.CubeVertexArray = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> CubeVertexBuffer;
		CubeVertexBuffer.reset(VertexBuffer::Create(cube_Vertices, sizeof(cube_Vertices)));

		CubeVertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float2, "a_TexCoord" },
				{ ShaderDataType::Float3, "a_Normal" },
			}
		);

		s_CubeData.CubeVertexArray->AddVertexBuffer(CubeVertexBuffer);

		//std::shared_ptr<IndexBuffer> squareIB;
		//squareIB.reset(IndexBuffer::Create(cube_Indices, sizeof(cube_Indices) / sizeof(unsigned int)));

		//s_CubeData.CubeVertexArray->SetIndexBuffer(squareIB);

	}
	void DrawCube::OnDraw(const std::shared_ptr<Shader>& shader) const
	{
		s_CubeData.CubeVertexArray->Bind();
		//Renderer::DrawIndexed(s_CubeData.CubeVertexArray);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		s_CubeData.CubeVertexArray->Unbind();

	}
}