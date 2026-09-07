#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include "BasicMesh.h"
#include "common.h"
#include<iostream>
#include <filesystem>
#include"util.h"

using namespace std;

#define POSITION_LOCATION  0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION    2
#define TANGENT_LOCATION   3
#define BITANGENT_LOCATION 4


BasicMesh::BasicMesh()
    : m_VAO(0)
{
    memset(m_Buffers, 0, sizeof(m_Buffers));
}


BasicMesh::~BasicMesh()
{
    Clear();
}


void BasicMesh::Clear()
{
    for (unsigned int i = 0; i < m_Textures.size(); i++) {
        SAFE_DELETE(m_Textures[i]);
    }

    if (m_Buffers[0] != 0) {
        glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
    }

    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
}


bool BasicMesh::LoadMesh(const string& Filename)
{
    Clear();

    printf("Loading mesh: '%s'\n", Filename.c_str());

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

    bool Ret = false;
    Assimp::Importer Importer;

    const aiScene* pScene = Importer.ReadFile(Filename.c_str(), ASSIMP_LOAD_FLAGS);

    if (pScene) {
        Ret = InitFromScene(pScene, Filename);
        if (Ret) {
            printf("Successfully loaded mesh '%s'\n", Filename.c_str());
        }
    }
    else {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
    }

    glBindVertexArray(0);

    return Ret;
}

bool BasicMesh::InitFromScene(const aiScene* pScene, const string& Filename)
{
    m_Meshes.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);
    m_Materials.resize(pScene->mNumMaterials);

    unsigned int NumVertices = 0;
    unsigned int NumIndices = 0;

    CountVerticesAndIndices(pScene, NumVertices, NumIndices);

    GLCheckError();

    ReserveSpace(NumVertices, NumIndices);

    GLCheckError();

    InitAllMeshes(pScene);

    GLCheckError();

    if (!InitMaterials(pScene, Filename)) {
        return false;
    }

    GLCheckError();

    PopulateBuffers();

    //GLCheck(int x);

    return GLCheckError();
}


void BasicMesh::CountVerticesAndIndices(const aiScene* pScene, unsigned int& NumVertices, unsigned int& NumIndices)
{
    for (unsigned int i = 0; i < m_Meshes.size(); i++) {
        m_Meshes[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
        m_Meshes[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
        m_Meshes[i].BaseVertex = NumVertices;
        m_Meshes[i].BaseIndex = NumIndices;

        NumVertices += pScene->mMeshes[i]->mNumVertices;
        NumIndices += m_Meshes[i].NumIndices;
    }
}


void BasicMesh::ReserveSpace(unsigned int NumVertices, unsigned int NumIndices)
{
    m_Positions.reserve(NumVertices);
    m_Normals.reserve(NumVertices);
    m_TexCoords.reserve(NumVertices);
    m_Indices.reserve(NumIndices);
}


void BasicMesh::InitAllMeshes(const aiScene* pScene)
{
    for (unsigned int i = 0; i < m_Meshes.size(); i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        InitSingleMesh(paiMesh);
    }
}


void BasicMesh::InitSingleMesh(const aiMesh* paiMesh)
{
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
        const aiVector3D& pPos = paiMesh->mVertices[i];
        const aiVector3D& pNormal = paiMesh->mNormals[i];
        const aiVector3D& pTexCoord = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][i] : Zero3D;
		const aiVector3D& pTangent = paiMesh->HasTangentsAndBitangents() ? paiMesh->mTangents[i] : Zero3D;
		const aiVector3D& pBitangent = paiMesh->HasTangentsAndBitangents() ? paiMesh->mBitangents[i] : Zero3D;

        m_Positions.push_back(glm::vec3(pPos.x, pPos.y, pPos.z));
        m_Normals.push_back(glm::vec3(pNormal.x, pNormal.y, pNormal.z));
        m_TexCoords.push_back(glm::vec2(pTexCoord.x, pTexCoord.y));
		m_Tangents.push_back(glm::vec3(pTangent.x, pTangent.y, pTangent.z));
		m_Bitangents.push_back(glm::vec3(pBitangent.x, pBitangent.y, pBitangent.z));

    }

    for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        m_Indices.push_back(Face.mIndices[0]);
        m_Indices.push_back(Face.mIndices[1]);
        m_Indices.push_back(Face.mIndices[2]);
    }
}

bool BasicMesh::InitMaterials(const aiScene* pScene, const string& Filename)
{
    string::size_type SlashIndex = Filename.find_last_of("/\\");
    string Dir;

    if (SlashIndex == string::npos) {
        Dir = ".";
    }
    else if (SlashIndex == 0) {
        Dir = "/";
    }
    else {
        Dir = Filename.substr(0, SlashIndex);
    }

    printf("Model directory: '%s'\n", Dir.c_str());

    bool Ret = true;

    for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        m_Materials[i].pDiffuse = NULL;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;  

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {

                printf("Texture from MTL: '%s'\n", Path.data);

               
                if (strlen(Path.data) == 0) {
                    printf("Warning: Texture path is empty in MTL file\n");
                    continue;
                }

                std::filesystem::path TexPath(Path.C_Str());

                std::string FileName = TexPath.filename().string();

                std::filesystem::path FullPath = std::filesystem::path(Dir) / "textures" / FileName;

                printf("Attempting to load: '%s'\n", FullPath.string().c_str());

                m_Textures[i] = new Texture(GL_TEXTURE_2D, FullPath.string());
                m_Materials[i].pDiffuse = m_Textures[i];

                if (!m_Materials[i].pDiffuse->Load()) {
                    printf("Error loading texture '%s'\n", FullPath.string().c_str());
                    delete m_Textures[i];
                    m_Textures[i] = NULL;
                
                }
                else {
                    printf("Successfully loaded texture '%s'\n", FullPath.string().c_str());
                }
            }
            else {
                printf("GetTexture failed for material %d\n", i);
            }
        }

        m_Materials[i].pSpecularExponent = NULL;

        std::filesystem::path RoughnessPath =
            std::filesystem::path(Dir) / "textures" / "small_lpg_tank_rough_4k.png";

        printf("Attempting to load roughness texture: '%s'\n", RoughnessPath.string().c_str());

        m_Materials[i].pSpecularExponent =
            new Texture(GL_TEXTURE_2D, RoughnessPath.string());

        if (!m_Materials[i].pSpecularExponent->Load()) {
            printf("Error loading roughness texture '%s'\n", RoughnessPath.string().c_str());
            delete m_Materials[i].pSpecularExponent;
            m_Materials[i].pSpecularExponent = NULL;
        }
        else {
            printf("Successfully loaded roughness texture '%s'\n", RoughnessPath.string().c_str());
        }
        //Normal
        std::filesystem::path NormalPath =
            std::filesystem::path(Dir) / "textures" / "small_lpg_tank_nor_gl_4k.png";

        printf("Attempting to load Normal texture: '%s'\n", NormalPath.string().c_str());

        m_Materials[i].pNormal =
            new Texture(GL_TEXTURE_2D, NormalPath.string());

        if (!m_Materials[i].pNormal->Load()) {
            printf("Error loading Normal texture '%s'\n", NormalPath.string().c_str());
            delete m_Materials[i].pNormal;
            m_Materials[i].pNormal = NULL;
        }
        else {
            printf("Successfully loaded Normal texture '%s'\n", NormalPath.string().c_str());
        }

        aiColor3D DiffuseColor(0.0f, 0.0f, 0.0f);

        if (pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, DiffuseColor) == AI_SUCCESS) {
            printf("Loaded Diffuse color [%f %f %f]\n", DiffuseColor.r, DiffuseColor.g, DiffuseColor.b);
            m_Materials[i].DiffuseColor.r = DiffuseColor.r;
            m_Materials[i].DiffuseColor.g = DiffuseColor.g;
            m_Materials[i].DiffuseColor.b = DiffuseColor.b;
        }

        aiColor3D SpecularColor(0.0f, 0.0f, 0.0f);

        if (pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, SpecularColor) == AI_SUCCESS) {
            printf("Loaded specular color [%f %f %f]\n", SpecularColor.r, SpecularColor.g, SpecularColor.b);
            m_Materials[i].SpecularColor.r = SpecularColor.r;
            m_Materials[i].SpecularColor.g = SpecularColor.g;
            m_Materials[i].SpecularColor.b = SpecularColor.b;
        }

    }

    return Ret;
}


void BasicMesh::PopulateBuffers()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_Positions[0]) * m_Positions.size(), &m_Positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_TexCoords[0]) * m_TexCoords.size(), &m_TexCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_Normals[0]) * m_Normals.size(), &m_Normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TANGENT_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_Tangents[0]) * m_Tangents.size(), &m_Tangents[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TANGENT_LOCATION);
    glVertexAttribPointer(TANGENT_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BITANGENT_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Bitangents[0]) * m_Bitangents.size(), &m_Bitangents[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(BITANGENT_LOCATION);
	glVertexAttribPointer(BITANGENT_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices[0]) * m_Indices.size(), &m_Indices[0], GL_STATIC_DRAW);
}


void BasicMesh::Render()
{
    
    glBindVertexArray(m_VAO);

    for (unsigned int i = 0; i < m_Meshes.size(); i++) {
        unsigned int MaterialIndex = m_Meshes[i].MaterialIndex;

        assert(MaterialIndex < m_Textures.size());

        if (m_Materials[MaterialIndex].pDiffuse) {
            m_Materials[MaterialIndex].pDiffuse->Bind(COLOR_TEXTURE_UNIT);
        }

        if (m_Materials[MaterialIndex].pSpecularExponent) {
            m_Materials[MaterialIndex].pSpecularExponent->Bind(SPECULAR_EXPONENT_UNIT);
        }

        if (m_Materials[MaterialIndex].pNormal) {
			m_Materials[MaterialIndex].pNormal->Bind(NORMAL_TEXTURE_UNIT);
        }

        glDrawElementsBaseVertex(GL_TRIANGLES,
            m_Meshes[i].NumIndices,
            GL_UNSIGNED_INT,
            (void*)(sizeof(unsigned int) * m_Meshes[i].BaseIndex),
            m_Meshes[i].BaseVertex);
    }

   
    glBindVertexArray(0);
}


void BasicMesh::Render(unsigned int NumInstances, const glm::mat4* WVPMats, const glm::mat4* WorldMats)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WVP_MAT_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * NumInstances, WVPMats, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WORLD_MAT_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * NumInstances, WorldMats, GL_DYNAMIC_DRAW);

    glBindVertexArray(m_VAO);

    for (unsigned int i = 0; i < m_Meshes.size(); i++) {
        const unsigned int MaterialIndex = m_Meshes[i].MaterialIndex;

        assert(MaterialIndex < m_Textures.size());

        if (m_Textures[MaterialIndex]) {
            m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
        }

        glDrawElementsInstancedBaseVertex(GL_TRIANGLES,
            m_Meshes[i].NumIndices,
            GL_UNSIGNED_INT,
            (void*)(sizeof(unsigned int) * m_Meshes[i].BaseIndex),
            NumInstances,
            m_Meshes[i].BaseVertex);
    }

    
    glBindVertexArray(0);
}

const Material& BasicMesh::GetMaterial()
{
   // for (unsigned int i = 0; i < m_Materials.size(); i++) {
   //     if (m_Materials[i].AmbientColor != glm::vec3(0.0f, 0.0f, 0.0f)) {
			//std::cout << m_Materials[i].AmbientColor.x << " " << m_Materials[i].AmbientColor.y << " " << m_Materials[i].AmbientColor.z << std::endl;
   //         return m_Materials[i];
   //     }
   // }

    assert(!m_Materials.empty());
    return m_Materials[0];

    //assert(0);
}