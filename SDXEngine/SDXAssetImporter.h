#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#pragma comment(lib, "assimp-vc143-mt.lib")

#include <list>

#include "SDXErrors.h"
#include "SDXAssetMGR.h"

namespace SDXEngine
{
	class SDXAssetImporter
	{
	public:
		SDXAssetImporter();
		~SDXAssetImporter();

		SDXErrorId Initialise();

		SDXErrorId LoadScene(
			const std::string& fileDir);

	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		SDXMeshData* ProcessMesh(aiMesh* mesh, const aiScene* scene);
		SDXErrorId LoadMaterials(aiMaterial* mat, aiTextureType type, std::string typeName);

	private:
		Assimp::Importer m_assimpImporter;
	};
}