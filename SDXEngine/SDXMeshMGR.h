#pragma once

#include <string>
#include <map>
#include <vector>

#include "SDXGfx.h"
#include "SDXMesh.h"
#include "SDXMeshData.h"
#include "SDXErrors.h"

namespace SDXEngine
{
	class SDXMeshMGR
	{
	public:
		SDXMeshMGR();
		~SDXMeshMGR();

		SDXMesh* CreateMesh(const std::string& id);
		SDXMesh* GetMesh(const std::string& id);

		int GetMeshCount();

		void RemoveMesh(const std::string& id);

	private:
		std::map<std::string, int> m_meshMap;
		std::vector<SDXMesh> m_vMeshes;
	};
}

