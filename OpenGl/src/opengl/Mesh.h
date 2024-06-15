//
#include"hzpch.h"
#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

using namespace std;

namespace Opengl {
#define MAX_BONE_INFLUENCE 4

    struct Vertex {//��������
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent����
        glm::vec3 Tangent;
        // bitangent������
        glm::vec3 Bitangent;
        //bone indexes which will influence this vertex��������
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        //weights from each boneȨ��
        float m_Weights[MAX_BONE_INFLUENCE];
    };

    struct Model_Texture {//��������
        unsigned int id;//ID�����ͺ�·��
        string type;
        string path;
    };

    class Mesh {
    public:
        // mesh Data
        vector<Vertex>       vertices;//�洢�������ݵ�����
        vector<unsigned int> indices;//������������
        vector<Model_Texture>      textures;//�洢�������ݵ�����
        unsigned int VAO;//��������

        // constructor
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Model_Texture> textures)//���캯��
        {
            this->vertices = vertices;//this���øó�Ա�����Ķ���Mesh��ʵ������Mesh��Ĺ��캯����thisָMesh��ʵ��
            this->indices = indices;//ʹ��thisָ�룬���Է������е�������Ա�����ͳ�Ա����
            this->textures = textures;

            // now that we have all the required data, set the vertex buffers and its attribute pointers.
            setupMesh();
        }

        // render the mesh�������񣨰�vao������glsl�����ƣ�����while�У�
        void Draw(Shader& shader)
        {
            //��ͬ�������͵ĸ����ĳ�ʼ
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            unsigned int normalNr = 1;
            unsigned int heightNr = 1;
            for (unsigned int i = 0; i < textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
                // retrieve texture number (the N in diffuse_textureN)��ȡ�����ţ�uniform sampler2D texture_diffuse1;�е� N��
                //��ȡglsl����������ĸ���������
                string number;
                string name = textures[i].type;

                if (name == "texture_diffuse")//��ǰѭ�������������п����������е�һ��
                    number = std::to_string(diffuseNr++);
                else if (name == "texture_specular")
                    number = std::to_string(specularNr++); // transfer unsigned int to string���ر�������֮����������ٵ������Ӷ�Ϊ��ȷ����ţ�
                else if (name == "texture_normal")
                    number = std::to_string(normalNr++); // transfer unsigned int to string
                else if (name == "texture_height")
                    number = std::to_string(heightNr++); // transfer unsigned int to string

                //���ڽ�����������Ϊ��ȷ������λ��λ��ֵ��
                glUniform1i(glGetUniformLocation(shader.GetShaderProgram(), (name + number).c_str()), i);//name�����

                // and finally bind the texture
                glBindTexture(GL_TEXTURE_2D, textures[i].id);//����ǰҪbind����
            }

            // draw mesh��������
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // һ��������ɣ��������������û�Ĭ��ֵ���Ǻõ������������
            glActiveTexture(GL_TEXTURE0);
        }

    private:
        // render data 
        unsigned int VBO, EBO;

        // initializes all the buffer objects/arrays
        void setupMesh()//��������gen����ȣ��󶨶������ݺ��������ݣ����ò��֣��󶨵�VAO�У�����init��
        {
            // create buffers/arrays
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);
            // load data into vertex buffers
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            // A great thing about structs is that their memory layout is sequential for all its items.
            // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
            // again translates to 3/2 floats which translates to a byte array.
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

            // set the vertex attribute pointers
            // vertex Positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            // vertex normals
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));//һ��������ܲ������ڵ�ǰ�����е�ƫ����
            // vertex texture coords
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
            // vertex tangent
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
            // vertex bitangent
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));//normalized��������ΪGL_TRUE�����ݻᱻӳ�䵽[-1,1]����
            // ids
            glEnableVertexAttribArray(5);
            glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));//�������������͵����ݣ����Ҳ��Ὣ����ת��Ϊ��������������normalized����
            // weights
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

            glBindVertexArray(0);
        }
    };

}