#include"DrawSphere.h"

namespace Opengl {

    const unsigned int X_SEGMENTS = 64;//segments
    const unsigned int Y_SEGMENTS = 64;
    const float PI = 3.14159265359f;

    struct SphereData {
        std::shared_ptr<VertexArray> SphereVertexArray;
        unsigned int indexCount;

        //�������ݣ�λ�ã��������꣬���ߣ�
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;
        //
        std::vector<float> data;
        //
        unsigned int sphereVAO = 0;
    };
    static SphereData s_SphereData;



	void DrawSphere::Bind() const
	{
        //
        //4096�����㣬�����������壬���վ��Ȱ�Ȧ��Ȧ�����ϵ��»��ƣ�������о����߶���ɢ�Ļ�����
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)//γ����64������
        {
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)//ÿ�����Ȱ�Ȧ��64������
            {
                float xSegment = (float)x / (float)X_SEGMENTS;//�ָ���y��ĽǶ�
                float ySegment = (float)y / (float)Y_SEGMENTS;//�ָ���x��z�ĽǶȣ�

                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);//������
                float yPos = std::cos(ySegment * PI);//
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);//

                s_SphereData.positions.push_back(glm::vec3(xPos, yPos, zPos));
                s_SphereData.uv.push_back(glm::vec2(xSegment, ySegment));
                s_SphereData.normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }
        //��������ǻ�����ƽ���4�����㣬ʵ����Ҫ6�����㣬
        for (int i = 0; i < Y_SEGMENTS; i++)//��0������͵�65��������룬���ڵ�ά�ȣ�����3����
        {
            for (int j = 0; j < X_SEGMENTS; j++)
            {

                 s_SphereData.indices.push_back(i * (X_SEGMENTS + 1) + j);
                 s_SphereData.indices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
                 s_SphereData.indices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
                 
                 s_SphereData.indices.push_back(i * (X_SEGMENTS + 1) + j);
                 s_SphereData.indices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
                 s_SphereData.indices.push_back(i * (X_SEGMENTS + 1) + j + 1);
            }
        }

        s_SphereData.indexCount = static_cast<unsigned int>(s_SphereData.indices.size());
        //data��ÿ��������������ݺ�Ϊһ��-������ÿ�����������

        for (unsigned int i = 0; i < s_SphereData.positions.size(); ++i)
        {
             s_SphereData.data.push_back( s_SphereData.positions[i].x);
             s_SphereData.data.push_back( s_SphereData.positions[i].y);
             s_SphereData.data.push_back( s_SphereData.positions[i].z);
            if (s_SphereData.normals.size() > 0)
            {
                 s_SphereData.data.push_back( s_SphereData.normals[i].x);
                 s_SphereData.data.push_back( s_SphereData.normals[i].y);
                 s_SphereData.data.push_back( s_SphereData.normals[i].z);
            }
            if (s_SphereData.uv.size() > 0)
            {
                 s_SphereData.data.push_back( s_SphereData.uv[i].x);
                 s_SphereData.data.push_back( s_SphereData.uv[i].y);
            }
        }

        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        
        glGenVertexArrays(1, &s_SphereData.sphereVAO);
        glBindVertexArray(s_SphereData.sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, s_SphereData.data.size() * sizeof(float), &s_SphereData.data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_SphereData.indices.size() * sizeof(unsigned int), &s_SphereData.indices[0], GL_STATIC_DRAW);
        unsigned int stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

	}
	void DrawSphere::OnDraw(const std::shared_ptr<Shader>& shader) const
	{
        glBindVertexArray(s_SphereData.sphereVAO);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // ����Ϊ�߿�ģʽ

        //glDrawElements(GL_LINES, s_SphereData.indexCount, GL_UNSIGNED_INT, 0); //
        glDrawElements(GL_TRIANGLE_STRIP, s_SphereData.indexCount, GL_UNSIGNED_INT, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // �ָ������ģʽ
	}
}
