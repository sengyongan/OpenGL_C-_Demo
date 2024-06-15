#include"hzpch.h"

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "opengl/renderer/Shader.h"
#include"opengl/renderer/Texture.h"

using namespace std;

namespace Opengl {

    class Model
    {
    public:
        // model data 
        vector<Model_Texture> textures_loaded;//��������	// �洢Ŀǰ�Ѽ��ص����������Ż���ȷ�������ᱻ���ض�Ρ�
        vector<Mesh>    meshes; //��������
        string directory; //Ŀ¼
        bool gammaCorrection;//٤��У��

        // constructor, expects a filepath to a 3D model.���캯������Ҫһ�� 3D ģ�͵��ļ�·����
        Model(string const& path, bool gamma = false) : gammaCorrection(gamma)//gammaĬ�Ϸ�
        {
            loadModel(path);
        }

        // draws the model, and thus all its meshes//����ģ�ͼ�����������
        void Draw(const std::shared_ptr<Shader>& shader)
        {
            for (unsigned int i = 0; i < meshes.size(); i++)
                meshes[i].Draw(shader);
        }

    private:
        // ���ļ��м���֧�� ASSIMP ��չ����ģ�ͣ��������ɵ�����洢�����������С�
        void loadModel(string const& path)
        {
            // read file via ASSIMPͨ�� ASSIMP ��ȡ�ļ�
            Assimp::Importer importer;//���ڶ�ȡ3Dģ���ļ�//���ڴ������ǻ�������ƽ�����ߡ���תUV����ͼ������߿ռ䣩//����һ��ָ�򳡾���ָ��scene��
            const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

            // check for errors��� ���� / AI_SCENE_FLAGS_INCOMPLETE��ǣ������Ƿ����������ڵ� �Ƿ�Ϊ��
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
            {
                cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
                return;
            }
            // retrieve the directory path of the filepath//�ҵ����һ��'/'�ַ���λ�ã�Ȼ��ʹ��substr()�������ַ����Ŀ�ͷ��ȡ����λ��
            directory = path.substr(0, path.find_last_of('/'));

            // process ASSIMPs root node recursively�ݹ鴦�� ASSIMP �ĸ��ڵ�
            processNode(scene->mRootNode, scene);
        }

        // �Եݹ鷽ʽ����ڵ㡣����ڵ��ϵ�ÿ�����񣬲������ӽڵ㣨����У����ظ���һ���̡�
        void processNode(aiNode* node, const aiScene* scene)
        {
            // process each mesh located at the current node
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                // the node object only contains indices to index the actual objects in the scene. 
                // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(processMesh(mesh, scene));
            }
            // after weve processed all of the meshes (if any) we then recursively process each of the children nodes
            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene);
            }

        }
        //��������
        Mesh processMesh(aiMesh* mesh, const aiScene* scene)
        {
            // data to fill3������Ϊ�˴���mesh����
            vector<Vertex> vertices;
            vector<unsigned int> indices;
            vector<Model_Texture> textures;

            // walk through each of the meshs vertices����ÿ�����񶥵�
            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;//mesh���еĶ�������
                glm::vec3 vector; // ����������һ��ռλ����������Ϊ assimp ʹ���Լ��������࣬����ֱ��ת��Ϊ glm �� vec3 �࣬���������Ƚ����ݴ��䵽���ռλ�� glm::vec3��
                // positionsλ��
                vector.x = mesh->mVertices[i].x;//�������ȡ��ǰ�����λ������
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.Position = vector;//���붥��
                // normals����
                if (mesh->HasNormals())
                {
                    vector.x = mesh->mNormals[i].x;//�������ȡ��ǰ����ķ�������
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    vertex.Normal = vector;
                }
                // texture coordinates�������꣨�������ߣ������ߣ�
                if (mesh->mTextureCoords[0]) // �����Ƿ�����������ꣿ
                {
                    glm::vec2 vec;
                    // һ�����������԰��� 8 ����ͬ���������ꡣ��ˣ����Ǽ��費�� 
                    // ʹ��һ����������ж�����������ģ�ͣ������������ȡ��һ�飨0����
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.TexCoords = vec;
                    // tangent����
                    vector.x = mesh->mTangents[i].x;
                    vector.y = mesh->mTangents[i].y;
                    vector.z = mesh->mTangents[i].z;
                    vertex.Tangent = vector;
                    // bitangent������
                    vector.x = mesh->mBitangents[i].x;
                    vector.y = mesh->mBitangents[i].y;
                    vector.z = mesh->mBitangents[i].z;
                    vertex.Bitangent = vector;
                }
                else
                    vertex.TexCoords = glm::vec2(0.0f, 0.0f);

                vertices.push_back(vertex);//��ӵ���������
            }
            // now wak through each of the meshs faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                // ���������������������洢������������
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }
            // process materials
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            // ���Ǽٶ���ɫ���в��������Ƶ�Լ���׳ɡ�ÿ������������Ӧ����Ϊ
            // ÿ������������Ӧ����Ϊ "texture_diffuseN"������ N ��һ���� 1 �� MAX_SAMPLER_NUMBER ��˳��š�
            // ��������Ҳ����ˣ����±���ʾ��
            // diffuse: texture_diffuseN
            // specular��texture_specularN
            // normal: texture_normalN

            // 1. diffuse maps
            vector<Model_Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 2. specular maps
            vector<Model_Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            // 3. normal maps
            std::vector<Model_Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
            // 4. height maps
            std::vector<Model_Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
            textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

            // ���ظ�����ȡ���������ݴ������������
            return Mesh(vertices, indices, textures);//���뵽mesh������
        }

        // ���������͵����в������������δ���أ��������Щ����  
        // �������Ϣ����Ϊ����ṹ���ء�
        vector<Model_Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)//���ʣ��������ͣ���������
        {
            vector<Model_Texture> textures;
            for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)//ѭ�����ʵ�ÿ������
            {
                aiString str;
                mat->GetTexture(type, i, &str);//��ȡ��������str��ֵΪ·��
                // ���֮ǰ�Ƿ���ع���������ǣ��������һ�ε����� ��������������
                bool skip = false;
                for (unsigned int j = 0; j < textures_loaded.size(); j++)//��ǰ�����е��������Ѿ���ӹ�������
                {
                    if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)//�Ƚϣ�==0��ʾ���
                    {
                        textures.push_back(textures_loaded[j]);
                        skip = true; // ����Ѿ������˾�����ͬ�ļ�·�������������������һ�����Ż�
                        break;
                    }
                }
                if (!skip)//û�м��ع�����
                {
                    Model_Texture texture;
                    texture.id = Texture::TextureFromFile(str.C_Str(), this->directory);//������������id
                    texture.type = typeName;//��������
                    texture.path = str.C_Str();//����·��str.C_Str()
                    textures.push_back(texture);
                    textures_loaded.push_back(texture);  // ����洢Ϊ����ģ�͵�������أ���ȷ�����ǲ��᲻��Ҫ�ؼ����ظ�������
                }
            }
            return textures;
        }
    };


}