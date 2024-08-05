#include "Renderer.h"

#include"Shader.h"
#include"Texture.h"
#include"opengl/draw/Model.h"
#include"opengl/core/App.h"
#include"Framebuffer.h"
#include"Uniform.h"
//
#include"opengl/draw/DrawCube.h"
#include"opengl/draw/DrawPointLight.h"
#include"opengl/draw/DrawPoint.h"
#include"opengl/draw/DrawQuad.h"
#include"opengl/draw/DrawTriangles.h"
#include"opengl/draw/DrawScreenQuad.h"
#include"opengl/draw/DrawSkybox.h"
#include"opengl/draw/DrawGeometry.h"
#include"opengl/draw/DrawPlanet.h"
#include"opengl/draw/DrawGamma.h"
#include"opengl/draw/DrawTBNQuad.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//temp
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <random>

namespace Opengl {
	bool blinn = true;
	//
	bool gammaEnabled = false;
	bool gammaKeyPressed = false;
	//ShadowMap
	glm::mat4 lightProjection, lightView;//光空间矩阵
	glm::mat4 lightSpaceMatrix;//视图投影矩阵

	glm::vec3 lightPos(0.5f, 1.0f, 0.3f);
	float near_plane = 1.0f;
	float far_plane = 25.0f;
	//
	bool shadows = true;
	bool shadowsKeyPressed = false;
	//HDR
	GLboolean hdr = true; // Change with 'Space'
	GLfloat exposure = 0.1f; // Change with Q and E

	float ourLerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}

	unsigned int noiseTexture;

	//multiple * 10 positions
	glm::vec3 cubePositions[] = {
		glm::vec3(3.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	//
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -5.0f),
		glm::vec3(1.7f,  0.2f,  2.3f),
		glm::vec3(2.5f, -3.3f, -6.0f),
		glm::vec3(4.0f,  2.0f, -3.0f),
		glm::vec3(0.7f,  1.0f,  2.0f),
		glm::vec3(2.1f, -3.0f, -1.0f),
		glm::vec3(-4.0f,  2.0f, -1.0f),
		glm::vec3(0.7f,  0.2f,  1.0f),
		glm::vec3(1.0f, 3.1f, -4.0f),
		glm::vec3(2.0f,  2.0f, 5.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	vector<glm::vec3> windows
	{
		glm::vec3(-7.5f, -3.5f, -2.0f),
			glm::vec3(-7.3f, -3.5f, 1.0f),
			glm::vec3(-7.0f, -3.5f, 0.9f),
			glm::vec3(-7.3f, -3.5f, -0.0f),
			glm::vec3(-7.0f, -3.5f, -0.6f)
	};
	glm::vec3 lightPositions[] = {
	   glm::vec3(-3.0f, -5.0f, 30.0f),
	   glm::vec3(-1.0f, -5.0f, 30.0f),
	   glm::vec3(1.0f,  -5.0f, 30.0f),
	   glm::vec3(3.0f,  -5.0f, 30.0f)
	};
	glm::vec3 lightColors[] = {
		glm::vec3(5.0f,   5.0f,  5.0f),
		glm::vec3(10.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,   0.0f,  15.0f),
		glm::vec3(0.0f,   5.0f,  0.0f),
		glm::vec3(5.0f,   5.0f,  5.0f),

		glm::vec3(15.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,   0.0f,  10.0f),
		glm::vec3(0.0f,   5.0f,  0.0f),
		glm::vec3(0.0f,   0.0f,  20.0f),
		glm::vec3(0.0f,   10.0f,  0.0f)
	};
	// Light sources
	// - Positions
	vector<glm::vec3> HDR_LightPositions {
		glm::vec3(0.0f, 0.0f, 49.5f),
			glm::vec3(-1.4f, -1.9f, 9.0f),
			glm::vec3(0.0f, -1.8f, 4.0f),
			glm::vec3(0.8f, -1.7f, 6.0f)
	};
	// - Colors
	vector<glm::vec3> HDR_lightColors{
		glm::vec3(200.0f, 200.0f, 200.0f),
			glm::vec3(0.1f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.2f),
			glm::vec3(0.0f, 0.1f, 0.0f)
	};
	vector<glm::vec3> objectPositions{
		glm::vec3(-10.0, -15.0, -10.0),
		glm::vec3(-10.0, -15.0, 0.0),
		glm::vec3(-10.0, -15.0, 10.0),
						  
		glm::vec3(0.0,   -15.0, -10.0),
		glm::vec3(0.0,   -15.0, 0.0),
		glm::vec3(0.0,   -15.0, 10.0),
						  
		glm::vec3(10.0,  -15.0, 0.0),
		glm::vec3(10.0,  -15.0, -10.0),
		glm::vec3(10.0,  -15.0, 10.)
	};
		
	std::vector<glm::vec3> Temp_LightPositions;
	std::vector<glm::vec3> Temp_LightColors;
	//RendererData
	struct RendererData {
		//Draw/////////////////////////////////////////////////////
		std::shared_ptr<DrawCube> m_DrawCube;
		std::shared_ptr<DrawPointLight> m_DrawPointLight;
		std::shared_ptr<DrawPoint> m_DrawPoint;
		std::shared_ptr<DrawQuad> m_DrawQuad;
		std::shared_ptr<DrawTriangles> m_DrawTriangles;

		std::shared_ptr<DrawPointLight> m_DrawStencil;

		std::shared_ptr<DrawScreenQuad> m_DrawScreenQuad;
		//Texture//////////////////////////////////////////////////
		std::shared_ptr<Texture> Texture1;
		std::shared_ptr<Texture> Texture2;
		std::shared_ptr<Texture> Texture3;
		std::shared_ptr<Texture> grass_Texture;
		std::shared_ptr<Texture> window_Texture;
		std::shared_ptr<Texture> metal_Texture;
		std::shared_ptr<Texture> metalGamma_Texture;
		//skybox///////////////////////////////////////////////////
		vector<std::string> CubeTexturePath{
			"../OpenGl/resources/skybox/right.jpg",
				"../OpenGl/resources/skybox/left.jpg",
				"../OpenGl/resources/skybox/top.jpg",
				"../OpenGl/resources/skybox/bottom.jpg",
				"../OpenGl/resources/skybox/back.jpg",
				"../OpenGl/resources/skybox/front.jpg"
		};
		std::shared_ptr<DrawSkybox> m_DrawSkybox;
		std::shared_ptr<Texture> cube_Texture;
		std::shared_ptr<Shader> SkyboxShader;
		//geometry///////////////////////////////////////////////////
		std::shared_ptr<DrawGeometry> m_DrawGeometry;
		std::shared_ptr<Shader> GeometryShader;
		std::shared_ptr<Shader> ModelNormalShader;
		//Planet && rock///////////////////////////////////////////////////
		std::shared_ptr<DrawPlanet> m_DrawPlanet;
		std::shared_ptr<Shader> PlanetShader;
		std::shared_ptr<Shader> RockShader;

		//FrameBuffer//////////////////////////////////////////////
		std::shared_ptr<Framebuffer> Multisample_FrameBuffer;
		std::shared_ptr<Texture> Screen_Texture;
		std::shared_ptr<Shader> SceneShader;
		//Shadow//////////////////////////////////////////////

		std::shared_ptr<Framebuffer> Shadow_FrameBuffer;
		std::shared_ptr<DrawPoint> m_ShadowDrawPoint;
		std::shared_ptr<Shader> ShadowShader;
		//Point_ShadowMap//////////////////////////////////////////////
		std::shared_ptr<Shader> Point_ShadowMapShader;//万象阴影贴图
		std::shared_ptr<Shader> Point_ShadowShader;//正常渲染 + 阴影绘制

		//Gamma//////////////////////////////////////////////
		std::shared_ptr<Shader> GammaShader;
		std::shared_ptr<DrawGamma> m_DrawGamma;
		//Shader///////////////////////////////////////////////////
		//quad
		std::shared_ptr<Shader> QuadShader;
		//Triangles
		std::shared_ptr<Shader> TrianglesShader;
		//cube
		std::shared_ptr<Shader> CubeShader;
		//whilte_Cube
		std::shared_ptr<Shader> PointLightShader;
		std::vector<glm::vec3> lightColors;/////////////
		//point
		std::shared_ptr<Shader> PointShader;
		//HDR///////////////////////////////////////////////////
		std::shared_ptr<Shader> HDRShader;
		std::shared_ptr<Shader> HDRCubeShader;
		//MRT///////////// //////////////////////////////////////
		std::shared_ptr<Shader> MRTShader;
		//UniformBuffer////////////////////////////////////////////
		std::shared_ptr<Shader> PingPongShader;

		//modle///////////////////////////////////////////////////
		std::shared_ptr<Shader> ModelShader;
		std::shared_ptr<Model> m_Model;
		//UniformBuffer////////////////////////////////////////////
		std::shared_ptr<Uniform> uniformBuffer;
		//TBN////////////////////////////////////////////
		std::shared_ptr<Shader> TBNQuadShader;
		std::shared_ptr<DrawTBNQuad> m_DrawTBNQuad;
		std::shared_ptr<Texture> brickWall_Texture;
		std::shared_ptr<Texture> brickWall_Normal_Texture;
		std::shared_ptr<Texture> brickWall_Depth_Texture;
		//G_Buffer && deferred_Light///////////////////////////////////////////////////
		std::shared_ptr<Shader> G_BufferShader;
		std::shared_ptr<Shader> deferred_lightShader;
		std::shared_ptr<Model> m_BackModel;
		//SSAO///////////////////////////////////////////////////
		std::shared_ptr<Shader> G_BufferSSAOShader;
		std::shared_ptr<Shader> G_BufferCubeSSAOShader;
		std::shared_ptr<Shader> SAOShader;
		std::shared_ptr<Shader> SAOBlurShader;
		std::shared_ptr<Shader> SSAOLightShader;
		std::vector<glm::vec3> ssaoKernel;
		std::vector<glm::vec3> ssaoNoise;
	};
	static RendererData s_Data;


	void Renderer::BeginScene(const EditorCamera& camera)
	{
		//uniformData_BindPoint
		glm::mat4 ViewProjection = camera.GetViewProjection();
		s_Data.uniformBuffer->SetData(glm::value_ptr(ViewProjection), sizeof(glm::mat4), 0);

	}

	void Renderer::init()
	{
		//Shader
		s_Data.QuadShader.reset(new Shader("../OpenGl/src/shader/quad_Vertex_Shader.glsl", "../OpenGl/src/shader/quad_Fragment_Shader.glsl"));//设置智能指针指向的对象
		s_Data.TrianglesShader.reset(new Shader("../OpenGl/src/shader/triangles_Vertex_Shader.glsl", "../OpenGl/src/shader/triangles_Fragment_Shader.glsl"));//设置智能指针指向的对象
		s_Data.CubeShader.reset(new Shader("../OpenGl/src/shader/multiple_lights.vs_glsl", "../OpenGl/src/shader/multiple_lights.fs_glsl"));
		s_Data.PointLightShader.reset(new Shader("../OpenGl/src/shader/pointLight_Vertex_Shader.glsl", "../OpenGl/src/shader/pointLight_Fragment_Shader.glsl"));
		s_Data.PointShader.reset(new Shader("../OpenGl/src/shader/point_Vertex_Shader.glsl", "../OpenGl/src/shader/point_Fragment_Shader.glsl"));
		s_Data.ModelShader.reset(new Shader("../OpenGl/src/shader/model_Vertex_Shader.glsl", "../OpenGl/src/shader/model_Fragment_Shader.glsl"));
		s_Data.ModelNormalShader.reset(new Shader("../OpenGl/src/shader/modelNormal_Vertex_Shader.glsl", "../OpenGl/src/shader/modelNormal_Fragment_Shader.glsl"
			, "../OpenGl/src/shader/modelNormal_geometry_Shader.glsl"));
		s_Data.SceneShader.reset(new Shader("../OpenGl/src/shader/screen_Vertex_Shader.glsl", "../OpenGl/src/shader/screen_Fragment_Shader.glsl"));
		s_Data.SkyboxShader.reset(new Shader("../OpenGl/src/shader/skybox_Vertex_Shader.glsl", "../OpenGl/src/shader/skybox_Fragment_Shader.glsl"));
		s_Data.GeometryShader.reset(new Shader("../OpenGl/src/shader/geometry_Vertex_Shader.glsl", "../OpenGl/src/shader/geometry_Fragment_Shader.glsl",
			"../OpenGl/src/shader/geometry_geometry_Shader.glsl"));
		s_Data.PlanetShader.reset(new Shader("../OpenGl/src/shader/model.vs", "../OpenGl/src/shader/model.fs"));
		s_Data.RockShader.reset(new Shader("../OpenGl/src/shader/Instance.vs", "../OpenGl/src/shader/Instance.fs"));
		s_Data.GammaShader.reset(new Shader("../OpenGl/src/newShader/gamma.vs", "../OpenGl/src/newShader/gamma.fs"));
		s_Data.ShadowShader.reset(new Shader("../OpenGl/src/newShader/ShadowMap.vs", "../OpenGl/src/newShader/ShadowMap.fs"));
		s_Data.Point_ShadowMapShader.reset(new Shader("../OpenGl/src/newShader/PointShadowMap.vs", "../OpenGl/src/newShader/PointShadowMap.fs", "../OpenGl/src/newShader/PointShadowMap.gs"));
		s_Data.Point_ShadowShader.reset(new Shader("../OpenGl/src/shader/point_Shadow.vs", "../OpenGl/src/shader/point_Shadow.fs"));
		s_Data.TBNQuadShader.reset(new Shader("../OpenGl/src/newShader/TBNquad.vs", "../OpenGl/src/newShader/TBNquad.fs"));
		s_Data.HDRShader.reset(new Shader("../OpenGl/src/newShader/HDR.vs", "../OpenGl/src/newShader/HDR.fs"));
		s_Data.HDRCubeShader.reset(new Shader("../OpenGl/src/newShader/HDRCube.vs", "../OpenGl/src/newShader/HDRCube.fs"));
		s_Data.PingPongShader.reset(new Shader("../OpenGl/src/newShader/PingPong.vs", "../OpenGl/src/newShader/PingPong.fs"));

		s_Data.G_BufferShader.reset(new Shader("../OpenGl/src/newShader/g_ModelShader.vs", "../OpenGl/src/newShader/g_ModelShader.fs"));
		s_Data.deferred_lightShader.reset(new Shader("../OpenGl/src/newShader/deferred.vs", "../OpenGl/src/newShader/deferred.fs"));
		//s_Data.MRTShader.reset(new Shader("../OpenGl/src/newShader/MRT.vs", "../OpenGl/src/newShader/MRT.fs"));
		s_Data.G_BufferSSAOShader.reset(new Shader("../OpenGl/src/SSAO/G_Buffer.vs", "../OpenGl/src/SSAO/G_Buffer.fs"));
		s_Data.G_BufferCubeSSAOShader.reset(new Shader("../OpenGl/src/SSAO/G_CubeBuffer.vs", "../OpenGl/src/SSAO/G_Buffer.fs"));
		s_Data.SAOShader.reset(new Shader("../OpenGl/src/SSAO/ssao.vs", "../OpenGl/src/SSAO/ssao.fs"));
		s_Data.SAOBlurShader.reset(new Shader("../OpenGl/src/SSAO/ssao.vs", "../OpenGl/src/SSAO/blur.fs"));
		s_Data.SSAOLightShader.reset(new Shader("../OpenGl/src/SSAO/ssao.vs", "../OpenGl/src/SSAO/light.fs"));

		Renderer::SSAOKernel();

		//Texture
		s_Data.Texture1 = std::make_unique<Texture>("../OpenGl/resources/textures/container2.png");
		s_Data.Texture2 = std::make_unique<Texture>("../OpenGl/resources/textures/ChernoLogo.png");
		s_Data.Texture3 = std::make_unique<Texture>("../OpenGl/resources/textures/container2_specular.png");
		s_Data.grass_Texture = std::make_unique<Texture>("../OpenGl/resources/textures/grass.png");
		s_Data.window_Texture = std::make_unique<Texture>("../OpenGl/resources/textures/window.png");
		s_Data.metal_Texture = std::make_unique<Texture>("../OpenGl/resources/textures/metal.png", false);
		s_Data.metalGamma_Texture = std::make_unique<Texture>("../OpenGl/resources/textures/metal.png", true);

		s_Data.brickWall_Texture = std::make_unique<Texture>("../OpenGl/resources/textures/brickwall.jpg..png");
		s_Data.brickWall_Normal_Texture = std::make_unique<Texture>("../OpenGl/resources/textures/brickwall_normal.png");
		s_Data.brickWall_Depth_Texture = std::make_unique<Texture>("../OpenGl/resources/textures/brickwall.png");

		s_Data.cube_Texture = std::make_unique<Texture>();
		s_Data.cube_Texture->loadCubemap(s_Data.CubeTexturePath);
		//
		//Framebuffer

		FramebufferSpecification fbSpec1;
		fbSpec1.Attachments = { FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::RGBA16F,FramebufferTextureFormat::RGBA, FramebufferTextureFormat::Depth };
		s_Data.Multisample_FrameBuffer = std::make_unique<Framebuffer>(fbSpec1);
		s_Data.Multisample_FrameBuffer->InvalidateMRT();
		s_Data.Multisample_FrameBuffer->InitSSAO();

		//Model
		s_Data.m_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/nanosuit/nanosuit.obj");
		//s_Data.m_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/cyborg/cyborg.obj");
		//s_Data.m_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/backpack/backpack.obj");
		//s_Data.m_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/planet/planet.obj");
		//s_Data.m_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/vampire/vampire.obj");


		//init
		s_Data.m_DrawCube = std::make_unique<DrawCube>();
		s_Data.m_DrawCube->Bind();
		s_Data.m_DrawPointLight = std::make_unique<DrawPointLight>();
		s_Data.m_DrawPointLight->Bind();
		s_Data.m_DrawPoint = std::make_unique<DrawPoint>();
		s_Data.m_DrawPoint->Bind();
		s_Data.m_DrawQuad = std::make_unique<DrawQuad>();
		s_Data.m_DrawQuad->Bind();
		s_Data.m_DrawTriangles = std::make_unique<DrawTriangles>();
		s_Data.m_DrawTriangles->Bind();
		s_Data.m_DrawStencil = std::make_unique<DrawPointLight>();
		s_Data.m_DrawStencil->Bind();
		s_Data.m_DrawScreenQuad = std::make_unique<DrawScreenQuad>();
		s_Data.m_DrawScreenQuad->Bind();
		s_Data.m_DrawSkybox = std::make_unique<DrawSkybox>();
		s_Data.m_DrawSkybox->Bind();
		s_Data.m_DrawGeometry = std::make_unique<DrawGeometry>();
		s_Data.m_DrawGeometry->Bind();
		s_Data.m_DrawPlanet = std::make_unique<DrawPlanet>();
		s_Data.m_DrawPlanet->Bind();
		s_Data.m_DrawGamma = std::make_unique<DrawGamma>();
		s_Data.m_DrawGamma->Bind();

		s_Data.m_DrawTBNQuad = std::make_unique<DrawTBNQuad>();
		s_Data.m_DrawTBNQuad->Bind();

		//uniformBuffer//////////////////////////////////////////////////////////
		//uniformBuffer_BindPoint
		unsigned int uniformBlockIndex_QuadShader = glGetUniformBlockIndex(s_Data.QuadShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.QuadShader->GetShaderProgram(), uniformBlockIndex_QuadShader, 0);
		unsigned int uniformBlockIndex_TrianglesShader = glGetUniformBlockIndex(s_Data.TrianglesShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.TrianglesShader->GetShaderProgram(), uniformBlockIndex_TrianglesShader, 0);
		unsigned int uniformBlockIndex_CubeShader = glGetUniformBlockIndex(s_Data.CubeShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.CubeShader->GetShaderProgram(), uniformBlockIndex_CubeShader, 0);
		unsigned int uniformBlockIndex_PointLightShader = glGetUniformBlockIndex(s_Data.PointLightShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.PointLightShader->GetShaderProgram(), uniformBlockIndex_PointLightShader, 0);
		unsigned int uniformBlockIndex_PointShader = glGetUniformBlockIndex(s_Data.PointShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.PointShader->GetShaderProgram(), uniformBlockIndex_PointShader, 0);
		unsigned int uniformBlockIndex_ModelShader = glGetUniformBlockIndex(s_Data.ModelShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.ModelShader->GetShaderProgram(), uniformBlockIndex_ModelShader, 0);
		unsigned int uniformBlockIndex_ModelNormalShader = glGetUniformBlockIndex(s_Data.ModelNormalShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.ModelNormalShader->GetShaderProgram(), uniformBlockIndex_ModelNormalShader, 0);
		unsigned int uniformBlockIndex_GeometryShader = glGetUniformBlockIndex(s_Data.GeometryShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.GeometryShader->GetShaderProgram(), uniformBlockIndex_GeometryShader, 0);
		unsigned int uniformBlockIndex_PlanetShader = glGetUniformBlockIndex(s_Data.PlanetShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.PlanetShader->GetShaderProgram(), uniformBlockIndex_PlanetShader, 0);
		unsigned int uniformBlockIndex_RockShader = glGetUniformBlockIndex(s_Data.RockShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.RockShader->GetShaderProgram(), uniformBlockIndex_RockShader, 0);
		unsigned int uniformBlockIndex_GammaShader = glGetUniformBlockIndex(s_Data.GammaShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.GammaShader->GetShaderProgram(), uniformBlockIndex_GammaShader, 0);
		unsigned int uniformBlockIndex_Point_ShadowShader = glGetUniformBlockIndex(s_Data.Point_ShadowShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.Point_ShadowShader->GetShaderProgram(), uniformBlockIndex_Point_ShadowShader, 0);
		unsigned int uniformBlockIndex_TBNQuadShader = glGetUniformBlockIndex(s_Data.TBNQuadShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.TBNQuadShader->GetShaderProgram(), uniformBlockIndex_TBNQuadShader, 0);
		unsigned int uniformBlockIndex_HDRCubeShader = glGetUniformBlockIndex(s_Data.HDRCubeShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.HDRCubeShader->GetShaderProgram(), uniformBlockIndex_HDRCubeShader, 0);
		unsigned int uniformBlockIndex_G_BufferShader = glGetUniformBlockIndex(s_Data.G_BufferShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.G_BufferShader->GetShaderProgram(), uniformBlockIndex_G_BufferShader, 0);

		s_Data.uniformBuffer = std::make_unique<Uniform>(sizeof(glm::mat4), 0);


		/////////////////////////////////////////////////////////////////////
		srand(300);
		for (unsigned int i = 0; i < 10; i++)
		{
			GLfloat rColor = ((rand() % 100) / 100.0f); // Between 0.5 and 1.0
			GLfloat gColor = ((rand() % 100) / 100.0f); // Between 0.5 and 1.0
			GLfloat bColor = ((rand() % 100) / 100.0f);
			s_Data.lightColors.push_back(glm::vec3(rColor, gColor, bColor));
		}
		// lighting info
		// -------------
		const unsigned int NR_LIGHTS = 32;

		srand(13);
		for (unsigned int i = 0; i < NR_LIGHTS; i++)
		{
			// calculate slightly random offsets
			float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
			float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
			float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
			Temp_LightPositions.push_back(glm::vec3(xPos, yPos, zPos));
			// also calculate random color
			float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
			float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
			float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
			Temp_LightColors.push_back(glm::vec3(rColor, gColor, bColor));
		}
		//texture->setInt///////////////////////////////////////////////////////////////////
		s_Data.CubeShader->Bind();
		s_Data.CubeShader->SetInt("material.texture1", 0);
		s_Data.CubeShader->SetInt("material.texture2", 1);
		s_Data.CubeShader->SetInt("material.specular_Texture", 2);
		s_Data.CubeShader->SetInt("material.shadowMap", 3);
		//
		s_Data.QuadShader->Bind();

		s_Data.QuadShader->SetInt("texture1", 0);
		//
		s_Data.SceneShader->Bind();

		s_Data.SceneShader->SetInt("screenTexture", 0);
		//
		s_Data.SkyboxShader->Bind();

		s_Data.SkyboxShader->SetInt("skybox", 0);
		//
		//s_Data.ModelShader->Bind();
		//s_Data.ModelShader->SetInt("skybox", 2);
		s_Data.GammaShader->Bind();

		s_Data.GammaShader->SetInt("floorTexture", 0);
		//
		s_Data.Point_ShadowShader->Bind();
		s_Data.Point_ShadowShader->SetInt("material.texture1", 0);
		s_Data.Point_ShadowShader->SetInt("material.texture2", 1);
		s_Data.Point_ShadowShader->SetInt("material.specular_Texture", 2);
		s_Data.Point_ShadowShader->SetInt("material.depthMap", 3);
		//
		s_Data.TBNQuadShader->Bind();
		s_Data.TBNQuadShader->SetInt("diffuseMap", 0);
		s_Data.TBNQuadShader->SetInt("normalMap", 1);
		s_Data.TBNQuadShader->SetInt("depthMap", 2);
		//
		s_Data.HDRShader->Bind();
		s_Data.HDRShader->SetInt("scene", 0);
		s_Data.HDRShader->SetInt("bloomBlur", 1);

		s_Data.HDRCubeShader->Bind();
		s_Data.HDRCubeShader->SetInt("diffuseTexture", 0);
		//
		s_Data.PingPongShader->Bind();
		s_Data.PingPongShader->SetInt("image", 0);
		//s_Data.MRTShader->Bind();
		//s_Data.MRTShader->SetInt("diffuseTexture", 0);
		s_Data.deferred_lightShader->Bind();
		s_Data.deferred_lightShader->SetInt("gPosition", 0);
		s_Data.deferred_lightShader->SetInt("gNormal", 1);
		s_Data.deferred_lightShader->SetInt("gAlbedoSpec", 2);

		//SSAO
		s_Data.SSAOLightShader->Bind();
		s_Data.SSAOLightShader->SetInt("gPosition", 0);
		s_Data.SSAOLightShader->SetInt("gNormal", 1);
		s_Data.SSAOLightShader->SetInt("gAlbedo", 2);
		s_Data.SSAOLightShader->SetInt("ssao", 3);

		s_Data.SAOShader->Bind();
		s_Data.SAOShader->SetInt("gPosition", 0);
		s_Data.SAOShader->SetInt("gNormal", 1);
		s_Data.SAOShader->SetInt("texNoise", 2);

		s_Data.SAOBlurShader->Bind();
		s_Data.SAOBlurShader->SetInt("ssaoInput", 0);

	}
	void Renderer::EndScene()
	{

		//input
		processInput(App::Get().GetWindow().GetNativeWindow());
		//init
		Renderer::Clear();//需要放在这个位置，清除自己的帧缓冲
		//
		glm::mat4 model = glm::mat4(1.0f);
		//GreenValue
		float timeValue = glfwGetTime();
		float GreenValue = (sin(timeValue) / 2.0f) + 0.5f;//sin值变为（-1——1），/2+0.5-》0——1
		glm::vec4 result = glm::vec4(0.0f, GreenValue, 0.0f, 1.0f);


		//G_FrameBuffer///////////////////////////////////////////////////////////////////////////
		//G_FrameBuffer///////////////////////////////////////////////////////////////////////////
		//G_FrameBuffer///////////////////////////////////////////////////////////////////////////

		s_Data.Multisample_FrameBuffer->BindMRTFramebuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = App::Get().GetCamera().GetProjection();
		glm::mat4 view = App::Get().GetCamera().GetViewMatrix();

		s_Data.G_BufferSSAOShader->Bind();
		s_Data.G_BufferSSAOShader->SetMat4("projection", projection);
		s_Data.G_BufferSSAOShader->SetMat4("view", view);
		model = glm::scale(model, glm::vec3(0.3f));
		model = glm::rotate(model, glm::radians(-90.0f),glm::vec3(0.0, 1.0, 0.0)) 
			* glm::rotate(glm::mat4(1.0f),
			glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, -10.0f, -12.0));
		s_Data.G_BufferSSAOShader->SetMat4("model", model);
		s_Data.G_BufferSSAOShader->SetInt("invertedNormals", 0);
		s_Data.m_Model->Draw(s_Data.G_BufferSSAOShader);


		s_Data.G_BufferCubeSSAOShader->Bind();
		model = glm::scale(glm::mat4(1.0f), glm::vec3(8.0f,0.1f,3.0f));
		model = glm::translate(model, glm::vec3(0.0f, -40.0f, 0.0));
		s_Data.G_BufferCubeSSAOShader->SetMat4("projection", projection);
		s_Data.G_BufferCubeSSAOShader->SetMat4("view", view);
		s_Data.G_BufferCubeSSAOShader->SetMat4("model", model);
		s_Data.G_BufferSSAOShader->SetInt("invertedNormals", 0);
		s_Data.m_DrawCube->OnDraw(s_Data.G_BufferCubeSSAOShader);


		s_Data.Multisample_FrameBuffer->Unbind();

		//for (unsigned int i = 0; i < objectPositions.size(); i++)
		//{
		//	model = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f));
		//	model = glm::translate(model, objectPositions[i]);
		//	s_Data.G_BufferShader->SetMat4("model", model);
		//	s_Data.G_BufferShader->SetFloat3("camera_Position", App::Get().GetCamera().GetPosition());
		//	s_Data.m_Model->Draw(s_Data.G_BufferShader);
		//}
		//SSAO///////////////////////////////////////////////////////////////////////////
		//SSAO///////////////////////////////////////////////////////////////////////////
		//SSAO///////////////////////////////////////////////////////////////////////////
		s_Data.Multisample_FrameBuffer->BindSSAOFramebuffer();
		glClear(GL_COLOR_BUFFER_BIT);
		s_Data.SAOShader->Bind();
		
		for (unsigned int i = 0; i < 64; ++i) {
		
			s_Data.SAOShader->SetFloat3("samples[" + std::to_string(i) + "]", s_Data.ssaoKernel[i]);
		}
		s_Data.SAOShader->SetMat4("projection", projection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, s_Data.Multisample_FrameBuffer->GetMRTAttachmentRendererID(0));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, s_Data.Multisample_FrameBuffer->GetMRTAttachmentRendererID(1));
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		s_Data.m_DrawScreenQuad->OnDraw(s_Data.SAOShader);

		s_Data.Multisample_FrameBuffer->Unbind();
		//Blur///////////////////////////////////////////////////////////////////////////
		//Blur///////////////////////////////////////////////////////////////////////////
		//Blur///////////////////////////////////////////////////////////////////////////
		s_Data.Multisample_FrameBuffer->BindSSAOBlurFramebuffer();
		glClear(GL_COLOR_BUFFER_BIT);
		s_Data.SAOBlurShader->Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, s_Data.Multisample_FrameBuffer->GetSSAOColorBufferAttachmentID());
		s_Data.m_DrawScreenQuad->OnDraw(s_Data.SAOBlurShader);

		s_Data.Multisample_FrameBuffer->Unbind();
		//deferred_Light///////////////////////////////////////////////////////////////////////////
		//deferred_Light///////////////////////////////////////////////////////////////////////////
		//deferred_Light///////////////////////////////////////////////////////////////////////////
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		s_Data.SSAOLightShader->Bind();
		glm::vec3 lightPosView = glm::vec3(view * glm::vec4(2.0, 4.0, -2.0, 1.0));
		s_Data.SSAOLightShader->SetFloat3("light.Position", lightPosView);
		s_Data.SSAOLightShader->SetFloat3("light.Color", glm::vec3(0.7f,0.7f,1.0f));
		// Update attenuation parameters
		const float linear = 0.09f;
		const float quadratic = 0.032f;
		s_Data.SSAOLightShader->SetFloat("light.Linear", linear);
		s_Data.SSAOLightShader->SetFloat("light.Quadratic", quadratic);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, s_Data.Multisample_FrameBuffer->GetMRTAttachmentRendererID(0));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, s_Data.Multisample_FrameBuffer->GetMRTAttachmentRendererID(1));
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, s_Data.Multisample_FrameBuffer->GetMRTAttachmentRendererID(2));
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, s_Data.Multisample_FrameBuffer->GetSSAOColorBufferBlurAttachmentID());
		s_Data.m_DrawScreenQuad->OnDraw(s_Data.SSAOLightShader);

		//s_Data.Multisample_FrameBuffer->Unbind();
#if 0
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		s_Data.deferred_lightShader->Bind();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, s_Data.Multisample_FrameBuffer->GetMRTAttachmentRendererID(0));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, s_Data.Multisample_FrameBuffer->GetMRTAttachmentRendererID(1));
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, s_Data.Multisample_FrameBuffer->GetMRTAttachmentRendererID(2));

		//
		s_Data.deferred_lightShader->SetFloat("shininess", 32.0f);
		//
		s_Data.deferred_lightShader->SetFloat3("constVal.camera_Position", App::Get().GetCamera().GetPosition());
		s_Data.deferred_lightShader->SetFloat3("constVal.camera_Direction", App::Get().GetCamera().GetForwardDirection());
		s_Data.deferred_lightShader->SetFloat("constVal.constant", 1.0f);
		s_Data.deferred_lightShader->SetFloat("constVal.linear", 0.09f);
		s_Data.deferred_lightShader->SetFloat("constVal.quadratic", 0.032f);
		//

		// directional light
		s_Data.deferred_lightShader->SetFloat3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		s_Data.deferred_lightShader->SetFloat3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		s_Data.deferred_lightShader->SetFloat3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		s_Data.deferred_lightShader->SetFloat3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		// point light 1
		for (int i = 0; i < 10; i++) {
			s_Data.deferred_lightShader->SetFloat3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
			s_Data.deferred_lightShader->SetFloat3("pointLights[" + std::to_string(i) + "].color", s_Data.lightColors[i]);

			s_Data.deferred_lightShader->SetFloat3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			s_Data.deferred_lightShader->SetFloat3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			s_Data.deferred_lightShader->SetFloat3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
			

			// Then calculate radius of light volume/sphere
			const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
			const GLfloat linear = 0.7;
			const GLfloat quadratic = 1.8;
			const GLfloat maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].r, lightColors[i].g), lightColors[i].b);
			GLfloat radius = (-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * maxBrightness))) / (2 * quadratic);
			s_Data.deferred_lightShader->SetFloat("pointLights[" + std::to_string(i) + "].Radius", radius);
		}

		// spotLight

		s_Data.deferred_lightShader->SetFloat3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		s_Data.deferred_lightShader->SetFloat3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		s_Data.deferred_lightShader->SetFloat3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		s_Data.deferred_lightShader->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		s_Data.deferred_lightShader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


		s_Data.m_DrawScreenQuad->OnDraw(s_Data.deferred_lightShader);

#endif
		//glBlitFramebuffer////////////////////////////////////////////////////////////////
		//glBlitFramebuffer////////////////////////////////////////////////////////////////
		//glBlitFramebuffer////////////////////////////////////////////////////////////////

		glBindFramebuffer(GL_READ_FRAMEBUFFER, s_Data.Multisample_FrameBuffer->GetMRTRendererID());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, 1024, 1024, 0, 0, 1024, 1024, GL_DEPTH_BUFFER_BIT, GL_NEAREST);


		//Gamma///////////////////////////////////////////////////////////////////////////
		//Gamma///////////////////////////////////////////////////////////////////////////
		//Gamma///////////////////////////////////////////////////////////////////////////
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gammaEnabled ? s_Data.metalGamma_Texture->GetRendererID() : s_Data.metal_Texture->GetRendererID());
		s_Data.GammaShader->Bind();
		glUniform3fv(glGetUniformLocation(s_Data.GammaShader->GetShaderProgram(), "lightPositions"), 4, &lightPositions[0][0]);
		glUniform3fv(glGetUniformLocation(s_Data.GammaShader->GetShaderProgram(), "lightColors"), 4, &lightColors[0][0]);
		s_Data.GammaShader->SetFloat3("viewPos", App::Get().GetCamera().GetPosition());
		s_Data.GammaShader->SetInt("gamma", gammaEnabled);
		model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, 35.0f));
		s_Data.GammaShader->SetMat4("model", model);
		s_Data.m_DrawGamma->OnDraw(s_Data.GammaShader);

		//TBN_Quad_BrickWall///////////////////////////////////////////////////////////////////////////
		//TBN_Quad_BrickWall///////////////////////////////////////////////////////////////////////////
		//TBN_Quad_BrickWall///////////////////////////////////////////////////////////////////////////
		s_Data.TBNQuadShader->Bind();
		model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		model = glm::rotate(model, (GLfloat)glfwGetTime() * -0.1f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.1f));
		s_Data.TBNQuadShader->SetMat4("model", model);
		s_Data.TBNQuadShader->SetFloat3("lightPos", lightPos);
		s_Data.TBNQuadShader->SetFloat3("viewPos", App::Get().GetCamera().GetPosition());
		glActiveTexture(GL_TEXTURE0);
		s_Data.brickWall_Texture->Bind();
		glActiveTexture(GL_TEXTURE1);
		s_Data.brickWall_Normal_Texture->Bind();
		glActiveTexture(GL_TEXTURE2);
		s_Data.brickWall_Depth_Texture->Bind();
		s_Data.m_DrawTBNQuad->OnDraw(s_Data.TBNQuadShader);


		//geometry///////////////////////////////////////////////////////////////////////////
		//geometry///////////////////////////////////////////////////////////////////////////
		//geometry///////////////////////////////////////////////////////////////////////////
		s_Data.GeometryShader->Bind();
		model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
		s_Data.GeometryShader->SetMat4("model", model);

		s_Data.m_DrawGeometry->OnDraw(s_Data.GeometryShader);
		//instance///////////////////////////////////////////////////////////////////////////
		//instance///////////////////////////////////////////////////////////////////////////
		//instance///////////////////////////////////////////////////////////////////////////
		///Planet
		glActiveTexture(GL_TEXTURE2);
		s_Data.cube_Texture->BindCubeTexture();

		s_Data.ModelShader->Bind();

		//
		s_Data.ModelShader->SetFloat("shininess", 32.0f);
		//
		s_Data.ModelShader->SetFloat3("constVal.camera_Position", App::Get().GetCamera().GetPosition());
		s_Data.ModelShader->SetFloat3("constVal.camera_Direction", App::Get().GetCamera().GetForwardDirection());
		s_Data.ModelShader->SetFloat("constVal.constant", 1.0f);
		s_Data.ModelShader->SetFloat("constVal.linear", 0.09f);
		s_Data.ModelShader->SetFloat("constVal.quadratic", 0.032f);
		//

		// directional light
		s_Data.ModelShader->SetFloat3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		s_Data.ModelShader->SetFloat3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		s_Data.ModelShader->SetFloat3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		s_Data.ModelShader->SetFloat3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		// point light 1
		for (int i = 0; i < 10; i++) {
			s_Data.ModelShader->SetFloat3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
			s_Data.ModelShader->SetFloat3("pointLights[" + std::to_string(i) + "].color", s_Data.lightColors[i]);

			s_Data.ModelShader->SetFloat3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			s_Data.ModelShader->SetFloat3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			s_Data.ModelShader->SetFloat3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		}

		// spotLight

		s_Data.ModelShader->SetFloat3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		s_Data.ModelShader->SetFloat3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		s_Data.ModelShader->SetFloat3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		s_Data.ModelShader->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		s_Data.ModelShader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		s_Data.m_DrawPlanet->OnDrawPlanet(s_Data.ModelShader);

		///Rock
		glActiveTexture(GL_TEXTURE2);
		s_Data.cube_Texture->BindCubeTexture();

		s_Data.RockShader->Bind();

		//
		s_Data.RockShader->SetFloat("shininess", 32.0f);
		//
		s_Data.RockShader->SetFloat3("constVal.camera_Position", App::Get().GetCamera().GetPosition());
		s_Data.RockShader->SetFloat3("constVal.camera_Direction", App::Get().GetCamera().GetForwardDirection());
		s_Data.RockShader->SetFloat("constVal.constant", 1.0f);
		s_Data.RockShader->SetFloat("constVal.linear", 0.09f);
		s_Data.RockShader->SetFloat("constVal.quadratic", 0.032f);
		//

		// directional light
		s_Data.RockShader->SetFloat3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		s_Data.RockShader->SetFloat3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		s_Data.RockShader->SetFloat3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		s_Data.RockShader->SetFloat3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		// point light 1
		for (int i = 0; i < 10; i++) {
			s_Data.RockShader->SetFloat3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
			s_Data.RockShader->SetFloat3("pointLights[" + std::to_string(i) + "].color", s_Data.lightColors[i]);

			s_Data.RockShader->SetFloat3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			s_Data.RockShader->SetFloat3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			s_Data.RockShader->SetFloat3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		}

		// spotLight

		s_Data.RockShader->SetFloat3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		s_Data.RockShader->SetFloat3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		s_Data.RockShader->SetFloat3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		s_Data.RockShader->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		s_Data.RockShader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		//s_Data.m_DrawPlanet->OnDrawRock(s_Data.RockShader);
		s_Data.m_DrawPlanet->OnDrawRock(s_Data.RockShader);
		//skybox///////////////////////////////////////////////////////////////////////////
		//skybox///////////////////////////////////////////////////////////////////////////
		//skybox///////////////////////////////////////////////////////////////////////////
		{
			glActiveTexture(GL_TEXTURE0);
			s_Data.cube_Texture->BindCubeTexture();
			glDepthFunc(GL_LEQUAL);
			s_Data.SkyboxShader->Bind();

			glm::mat4 view = glm::mat4(glm::mat3(App::Get().GetCamera().GetViewMatrix()));
			glm::mat4 projection = App::Get().GetCamera().GetProjection();
			s_Data.SkyboxShader->SetMat4("view", view);
			s_Data.SkyboxShader->SetMat4("projection", projection);

			s_Data.m_DrawSkybox->OnDraw(s_Data.SkyboxShader);
			s_Data.SkyboxShader->Bind();
			glDepthFunc(GL_LESS);
		}

		//grass///////////////////////////////////////////////////////////////////////////
		//grass///////////////////////////////////////////////////////////////////////////
		//grass///////////////////////////////////////////////////////////////////////////
		glActiveTexture(GL_TEXTURE0);
		s_Data.grass_Texture->Bind();

		s_Data.QuadShader->Bind();
		model = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 0.0f));
		s_Data.QuadShader->SetFloat4("ourColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		s_Data.QuadShader->SetMat4("model", model);

		s_Data.m_DrawQuad->OnDraw(s_Data.QuadShader);

		//lightPos.z = static_cast<float>(sin(glfwGetTime() * 0.5) * 3.0);
		// 0. create depth cubemap transformation matrices
		// -----------------------------------------------
		Renderer::DrawScene();

		//Renderer_Scene
		// pointLight//////////////////////////////////////////////////////////////////////////////
		// pointLight//////////////////////////////////////////////////////////////////////////////
		// pointLight//////////////////////////////////////////////////////////////////////////////
		s_Data.PointLightShader->Bind();
		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			s_Data.PointLightShader->SetMat4("model", model);
			s_Data.PointLightShader->SetFloat3("color", lightColors[i]);
			s_Data.m_DrawPointLight->OnDraw(s_Data.PointLightShader);

		}

		// point /Line  //////////////////////////////////////////////////////////////////////////////
		// point /Line  //////////////////////////////////////////////////////////////////////////////
		// point /Line  //////////////////////////////////////////////////////////////////////////////
		s_Data.PointShader->Bind();
		model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 10.0f));
		s_Data.PointShader->SetMat4("model", model);
		s_Data.PointShader->SetFloat3("color", result);
		s_Data.m_DrawPoint->OnDraw(s_Data.PointShader);

		//modle//////////////////////////////////////////////////////////////////////////////////
		//modle//////////////////////////////////////////////////////////////////////////////////
		//modle//////////////////////////////////////////////////////////////////////////////////

		s_Data.PlanetShader->Bind();

		//
		s_Data.PlanetShader->SetFloat("shininess", 32.0f);
		//
		s_Data.PlanetShader->SetFloat3("constVal.camera_Position", App::Get().GetCamera().GetPosition());
		s_Data.PlanetShader->SetFloat3("constVal.camera_Direction", App::Get().GetCamera().GetForwardDirection());
		s_Data.PlanetShader->SetFloat("constVal.constant", 1.0f);
		s_Data.PlanetShader->SetFloat("constVal.linear", 0.09f);
		s_Data.PlanetShader->SetFloat("constVal.quadratic", 0.032f);
		//

		// directional light
		s_Data.PlanetShader->SetFloat3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		s_Data.PlanetShader->SetFloat3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		s_Data.PlanetShader->SetFloat3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		s_Data.PlanetShader->SetFloat3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		// point light 1
		for (int i = 0; i < 10; i++) {
			s_Data.PlanetShader->SetFloat3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
			s_Data.PlanetShader->SetFloat3("pointLights[" + std::to_string(i) + "].color", s_Data.lightColors[i]);

			s_Data.PlanetShader->SetFloat3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			s_Data.PlanetShader->SetFloat3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			s_Data.PlanetShader->SetFloat3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		}

		// spotLight

		s_Data.PlanetShader->SetFloat3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		s_Data.PlanetShader->SetFloat3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		s_Data.PlanetShader->SetFloat3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		s_Data.PlanetShader->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		s_Data.PlanetShader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));



		//
		model = glm::scale(glm::mat4(1.0f), glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::translate(model, glm::vec3(20.0f, -10.0f, 0.0f));
		s_Data.PlanetShader->SetMat4("model", model);

		s_Data.m_Model->Draw(s_Data.PlanetShader);


		//window///////////////////////////////////////////////////////////////////////////
		//window///////////////////////////////////////////////////////////////////////////
		//window///////////////////////////////////////////////////////////////////////////
		glEnable(GL_BLEND);
		glActiveTexture(GL_TEXTURE0);
		s_Data.window_Texture->Bind();
		s_Data.QuadShader->Bind();
		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < windows.size(); i++)
		{
			float distance = glm::length(App::Get().GetCamera().GetPosition() - windows[i]);
			sorted[distance] = windows[i];
		}
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, it->second);
			s_Data.QuadShader->SetMat4("model", model);
			s_Data.QuadShader->SetFloat4("ourColor", glm::vec4(0.3f, 1.0f, 1.0f, 1.0f));
			s_Data.m_DrawQuad->OnDraw(s_Data.QuadShader);
		}
		s_Data.QuadShader->Unbind();
		glDisable(GL_BLEND);
		//pingpong//////////////////////////////////////////////////////////////////////////////
		//pingpong//////////////////////////////////////////////////////////////////////////////
		//pingpong//////////////////////////////////////////////////////////////////////////////
		//

		//bool horizontal = true, first_iteration = true;
		//unsigned int amount = 10;
		//s_Data.PingPongShader->Bind();
		//for (unsigned int i = 0; i < amount; i++)
		//{
		//	glBindFramebuffer(GL_FRAMEBUFFER, s_Data.Multisample_FrameBuffer->GetPingPongRendererID(horizontal));
		//	s_Data.PingPongShader->SetInt("horizontal", horizontal);
		//	glBindTexture(GL_TEXTURE_2D, first_iteration ? s_Data.Multisample_FrameBuffer->GetMRTAttachmentRendererID(1) : s_Data.Multisample_FrameBuffer->GetPingPongAttachmentRendererID(!horizontal));  // bind texture of other framebuffer (or scene if first iteration)
		//	s_Data.m_DrawScreenQuad->OnDraw(s_Data.PingPongShader);
		//	horizontal = !horizontal;
		//	if (first_iteration)
		//		first_iteration = false;
		//}
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//frameBuffer//////////////////////////////////////////////////////////////////////////////
		//frameBuffer//////////////////////////////////////////////////////////////////////////////
		//frameBuffer//////////////////////////////////////////////////////////////////////////////


	}
	void Renderer::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void Renderer::Clear()
	{
		//深度测试
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		//模板测试
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		//混合
		//glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//点渲染模式，调整大小
		glEnable(GL_PROGRAM_POINT_SIZE);
		//清除颜色缓冲
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	}
	void Renderer::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::DrawPoints(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_POINTS, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
	void Renderer::DrawLines(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_LINE_STRIP, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::DrawScene()
	{
		//plane地面//////////////////////////////////////////////////////////////////////////////////
		//plane地面//////////////////////////////////////////////////////////////////////////////////
		//plane地面//////////////////////////////////////////////////////////////////////////////////

		glm::mat4 model = glm::mat4(1.0f);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		glActiveTexture(GL_TEXTURE0);
		s_Data.metal_Texture->Bind();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, 0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, s_Data.Shadow_FrameBuffer->GetDepthCubeAttachmentRendererID());

		s_Data.Point_ShadowShader->Bind();

		s_Data.Point_ShadowShader->SetFloat("material.shininess", 32.0f);
		//
		s_Data.Point_ShadowShader->SetFloat3("constVal.camera_Position", App::Get().GetCamera().GetPosition());
		s_Data.Point_ShadowShader->SetFloat3("constVal.camera_Direction", App::Get().GetCamera().GetForwardDirection());
		s_Data.Point_ShadowShader->SetFloat("constVal.constant", 1.0f);
		s_Data.Point_ShadowShader->SetFloat("constVal.linear", 0.09f);
		s_Data.Point_ShadowShader->SetFloat("constVal.quadratic", 0.032f);

		s_Data.Point_ShadowShader->SetFloat("constVal.blinn", blinn);
		s_Data.Point_ShadowShader->SetFloat("constVal.gamma", gammaEnabled);

		s_Data.Point_ShadowShader->SetFloat3("constVal.lightPos", lightPos);
		s_Data.Point_ShadowShader->SetInt("constVal.shadows", shadows);
		s_Data.Point_ShadowShader->SetFloat("constVal.far_plane", far_plane);
		//

		// directional light
		s_Data.Point_ShadowShader->SetFloat3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		s_Data.Point_ShadowShader->SetFloat3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		s_Data.Point_ShadowShader->SetFloat3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		s_Data.Point_ShadowShader->SetFloat3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		// point light 1
		for (int i = 0; i < 10; i++) {
			s_Data.Point_ShadowShader->SetFloat3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
			s_Data.Point_ShadowShader->SetFloat3("pointLights[" + std::to_string(i) + "].color", s_Data.lightColors[i]);

			s_Data.Point_ShadowShader->SetFloat3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			s_Data.Point_ShadowShader->SetFloat3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			s_Data.Point_ShadowShader->SetFloat3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		}

		// spotLight

		s_Data.Point_ShadowShader->SetFloat3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		s_Data.Point_ShadowShader->SetFloat3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		s_Data.Point_ShadowShader->SetFloat3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		s_Data.Point_ShadowShader->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		s_Data.Point_ShadowShader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		//
		model = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 1.0f, 50.0f));
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, -0.1f));
		s_Data.Point_ShadowShader->SetMat4("model", model);
		s_Data.m_DrawCube->OnDraw(s_Data.Point_ShadowShader);
		//
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		float scaleVal = 1.01f;
		s_Data.PointLightShader->Bind();
		model = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 1.0f, 50.0f) * scaleVal);
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, -0.1f));
		s_Data.PointLightShader->SetMat4("model", model);
		s_Data.PointLightShader->SetFloat3("color", s_Data.lightColors[0]);
		s_Data.m_DrawStencil->OnDraw(s_Data.PointLightShader);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);

		// cube//////////////////////////////////////////////////////////////////////////////
		// cube//////////////////////////////////////////////////////////////////////////////
		// cube//////////////////////////////////////////////////////////////////////////////
		//
		glActiveTexture(GL_TEXTURE0);
		s_Data.Texture1->Bind();
		glActiveTexture(GL_TEXTURE1);
		s_Data.Texture2->Bind();
		glActiveTexture(GL_TEXTURE2);
		s_Data.Texture3->Bind();
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, 0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, s_Data.Shadow_FrameBuffer->GetDepthCubeAttachmentRendererID());
		//
		s_Data.Point_ShadowShader->Bind();
		//		//
		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
			float angle = 20.0f * i + 1;
			model = glm::rotate(model, glm::radians(40.0f), glm::vec3(1.0f, 0.3f, 0.5f));
			s_Data.Point_ShadowShader->SetMat4("model", model);

			//Renderer::DrawIndexed(s_Data.CubeVertexArray);
			s_Data.m_DrawCube->OnDraw(s_Data.Point_ShadowShader);
		}


	}

	void Renderer::processInput(GLFWwindow* window)
	{
		//Gamma
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !gammaKeyPressed)
		{
			gammaEnabled = !gammaEnabled;//是否启用
			gammaKeyPressed = true;

		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)//松开按键执行
		{
			gammaKeyPressed = false;
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !shadowsKeyPressed)
		{
			shadows = !shadows;
			shadowsKeyPressed = true;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		{
			shadowsKeyPressed = false;
		}
	}

	void Renderer::SSAOKernel()
	{
		// generate sample kernel
		// ----------------------
		std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
		std::default_random_engine generator;
		for (unsigned int i = 0; i < 64; ++i)
		{
			glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
			sample = glm::normalize(sample);
			sample *= randomFloats(generator);
			float scale = float(i) / 64.0f;

			// scale samples s.t. they're more aligned to center of kernel
			scale = ourLerp(0.1f, 1.0f, scale * scale);
			sample *= scale;
			s_Data.ssaoKernel.push_back(sample);
		}

		// generate noise texture
		// ----------------------
		for (unsigned int i = 0; i < 16; i++)
		{
			glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
			s_Data.ssaoNoise.push_back(noise);
		}
		glGenTextures(1, &noiseTexture);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &s_Data.ssaoNoise[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}


}