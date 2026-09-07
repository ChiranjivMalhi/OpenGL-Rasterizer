
#ifndef GLRENDERER_BASIC_MESH_H
#define GLRENDERER_BASIC_MESH_H

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <vector>
#include <assimp/Importer.hpp>      
#include <assimp/scene.h>   

#include <glm/glm.hpp>
#include "texture.h"
#include "world_transform.h"
#include "material.h"

#define DEFAULT_MESH_PATH       "../Content"

class BasicMesh
{
public:
    BasicMesh();
    ~BasicMesh();

    bool LoadMesh(const std::string& Filename);

    void Render();

    void Render(unsigned int NumInstances, const glm::mat4* WVPMats, const glm::mat4* WorldMats);

    WorldTransform& GetWorldTransform() { return m_worldTransform; }
    const WorldTransform& GetWorldTransform() const { return m_worldTransform; }

    unsigned int GetMeshCount() const { return m_Meshes.size(); }
    unsigned int GetTextureCount() const { return m_Textures.size(); }

    const Material& GetMaterial();

private:
    void Clear();

    bool InitFromScene(const aiScene* pScene, const std::string& Filename);

    void CountVerticesAndIndices(const aiScene* pScene, unsigned int& NumVertices, unsigned int& NumIndices);

    void ReserveSpace(unsigned int NumVertices, unsigned int NumIndices);

    void InitAllMeshes(const aiScene* pScene);

    void InitSingleMesh(const aiMesh* paiMesh);

    bool InitMaterials(const aiScene* pScene, const std::string& Filename);

    void PopulateBuffers();


#define INVALID_MATERIAL 0xFFFFFFFF

    enum BUFFER_TYPE {
        INDEX_BUFFER = 0,
        POS_VB = 1,
        TEXCOORD_VB = 2,
        NORMAL_VB = 3,
        WVP_MAT_VB = 4,
        WORLD_MAT_VB = 5,
		TANGENT_VB = 6,
		BITANGENT_VB = 7,
        NUM_BUFFERS = 8,
    };

    WorldTransform m_worldTransform;
    GLuint m_VAO = 0;
    GLuint m_Buffers[NUM_BUFFERS] = { 0 };

    struct BasicMeshEntry {
        BasicMeshEntry()
            : NumIndices(0)
            , BaseVertex(0)
            , BaseIndex(0)
            , MaterialIndex(INVALID_MATERIAL)
        {
        }

        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
        unsigned int MaterialIndex;
    };

    std::vector<BasicMeshEntry> m_Meshes;
    std::vector<Texture*> m_Textures;
    std::vector<Material> m_Materials;

    std::vector<glm::vec3> m_Positions;
    std::vector<glm::vec3> m_Normals;
    std::vector<glm::vec2> m_TexCoords;
	std::vector<glm::vec3> m_Tangents;
	std::vector<glm::vec3> m_Bitangents;
    std::vector<unsigned int> m_Indices;
};

#endif 
