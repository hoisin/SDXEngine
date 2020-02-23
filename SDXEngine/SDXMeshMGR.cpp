#include "stdafx.h"
#include "SDXMeshMGR.h"

using namespace SDXEngine;

SDXEngine::SDXMeshMGR::SDXMeshMGR()
{
}

SDXEngine::SDXMeshMGR::~SDXMeshMGR()
{
}

SDXMesh* SDXEngine::SDXMeshMGR::CreateMesh(const std::string& id)
{
	SDXMesh* pReturnMesh = nullptr;

	auto find = m_meshMap.find(id);
	// If no existing,  create
	if (find == m_meshMap.end())
	{
		m_vMeshes.push_back(SDXMesh());
		m_meshMap.insert(std::pair<std::string, int>(id, static_cast<int>(m_vMeshes.size() - 1)));
		pReturnMesh = &m_vMeshes.back();
	}

	return pReturnMesh;
}

SDXMesh* SDXEngine::SDXMeshMGR::GetMesh(const std::string& id)
{
	SDXMesh* pReturnMesh = nullptr;

	auto find = m_meshMap.find(id);
	if (find != m_meshMap.end())
	{
		pReturnMesh = &m_vMeshes[find->second];
	}

	return pReturnMesh;
}

int SDXEngine::SDXMeshMGR::GetMeshCount()
{
	return static_cast<int>(m_vMeshes.size());
}

void SDXEngine::SDXMeshMGR::RemoveMesh(const std::string& id)
{
	auto find = m_meshMap.find(id);
	if (find != m_meshMap.end())
	{
		auto lastItem = std::find_if(m_meshMap.begin(), m_meshMap.end(),
			[this](std::pair<std::string, int> p) { return (p.second == static_cast<int>(m_vMeshes.size() - 1)) ? true : false; });
		lastItem->second = find->second;
		std::swap(m_vMeshes[find->second], m_vMeshes.back());
		m_meshMap.erase(find);
		m_vMeshes.pop_back();
	}
}
