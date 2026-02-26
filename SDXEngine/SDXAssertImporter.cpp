#include "stdafx.h"
#include "SDXAssetImporter.h"
#include <format>


using namespace SDXEngine;

SDXAssetImporter::SDXAssetImporter() : m_assimpImporter(Assimp::Importer())
{
}

SDXAssetImporter::~SDXAssetImporter()
{
}

SDXErrorId SDXAssetImporter::Initialise()
{
	return SDXErrorId::SDX_ERROR_NONE;
}

SDXErrorId SDXAssetImporter::LoadScene(
	const std::string& fileDir)
{
	// TODO: default shader for now
	const std::string shaderId = "basic_shader";

	const aiScene* scene = m_assimpImporter.ReadFile(fileDir,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		// TODO
		return SDXErrorId::SDX_ERROR_NONE;
	}

	ProcessNode(scene->mRootNode, scene);

	if (scene != nullptr)
	{
		m_assimpImporter.FreeScene();
	}

	return SDXErrorId::SDX_ERROR_NONE;
}

void SDXAssetImporter::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* pMesh = scene->mMeshes[node->mMeshes[i]];

		// Get the mesh name
		aiString meshName = pMesh->mName;

		SDXMesh* pCreatedMesh;
		ASSETMGR->CreateMesh(std::format("{}_{}", meshName.C_Str(), i), &pCreatedMesh);
		SDXMeshData* meshData = ProcessMesh(pMesh, scene);
		pCreatedMesh->AddSubMesh(meshData, ASSETMGR->GetDirectX());

		delete meshData;
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

SDXMeshData* SDXAssetImporter::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	SDXMeshData* dataMesh = new SDXMeshData();
	if (mesh->HasNormals())
	{
		dataMesh->CreateVertexArray(SDXVertexType::SDXVERTEX_TYPE_PNT, mesh->mNumVertices);
		SDXVertexPNT* pVertex = (SDXVertexPNT*)dataMesh->GetVertexData();

		for (int vert = 0; vert < mesh->mNumVertices; vert++)
		{
			pVertex->position = XMFLOAT3(
				mesh->mVertices[vert].x,
				mesh->mVertices[vert].y,
				mesh->mVertices[vert].z);

			pVertex->normal = XMFLOAT3(
				mesh->mNormals[vert].x,
				mesh->mNormals[vert].y,
				mesh->mNormals[vert].z);

			// Assimp can support upto 8 texture coords per vertex, only concerned about the first
			if (mesh->mTextureCoords[0])
			{
				pVertex->textureCoord = XMFLOAT2(
					mesh->mTextureCoords[0][vert].x,
					mesh->mTextureCoords[0][vert].y);
			}

			// Move to next vertex
			pVertex++;
		}
	}

	std::vector<int> indices = std::vector<int>();
	// For total faces
	for (int face = 0; face < mesh->mNumFaces; face++)
	{
		// For each indices in the face
		auto faceData = mesh->mFaces[face];
		for (int idx = 0; idx < faceData.mNumIndices; idx++)
		{
			indices.push_back(faceData.mIndices[idx]);
		}
	}

	dataMesh->CreateIndexArray(indices.size());
	auto pIdx = dataMesh->GetIndexData();

	for (int k = 0; k < indices.size(); k++)
	{
		*pIdx = indices[k];

		// Move to next index
		pIdx++;
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		LoadMaterials(material, aiTextureType_DIFFUSE, "texture_diffuse");
		LoadMaterials(material, aiTextureType_SPECULAR, "texture_specular");
	}

	return dataMesh;
}

SDXErrorId SDXAssetImporter::LoadMaterials(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string filePath = std::string(str.C_Str());
		std::string directory = std::format("../Assets/Main{}", filePath.substr(1, std::string::npos));
		SDXErrorId error = ASSETMGR->LoadTexture(directory, directory);
		if (error != SDXErrorId::SDX_ERROR_NONE && error != SDXErrorId::SDX_ERROR_TEXTUREMGR_ID_ALREADY_EXIST)
		{
			return error;
		}
	}

	return SDXErrorId::SDX_ERROR_NONE;
}