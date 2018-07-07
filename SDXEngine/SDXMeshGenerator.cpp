#include "stdafx.h"
#include "SDXMeshGenerator.h"

using namespace SDXEngine;

SDXMeshGenerator::SDXMeshGenerator()
{
}

SDXMeshGenerator::~SDXMeshGenerator()
{
}

SDXMeshData * SDXEngine::SDXMeshGenerator::GeneratePlane(UINT size, SDXVertexType type, UINT subdivision,
	const XMFLOAT3& color)
{
	SDXErrorId error = SDX_ERROR_NONE;
	SDXMeshData* mesh = new SDXMeshData;

	float x = 0;
	float y = 0;
	float z = 0;

	// Find total vertices
	int verticesOnEdge = 2 + (subdivision - 1);
	int totalVertices = verticesOnEdge * verticesOnEdge;

	// Find total indices
	int totalIndices = 6 * (subdivision * subdivision);

	error = mesh->CreateVertexArray(type, totalVertices);
	if (error != SDX_ERROR_NONE)
	{
		delete mesh;
		mesh = nullptr;
		return nullptr;
	}

	error = mesh->CreateIndexArray(totalIndices);
	if (error != SDX_ERROR_NONE)
	{
		delete mesh;
		mesh = nullptr;
		return nullptr;
	}

	switch (type)
	{
	case SDXVERTEX_TYPE_PC:
		break;

	default:
		delete mesh;
		mesh = nullptr;
		break;
	}

	return mesh;
}

SDXMeshData * SDXEngine::SDXMeshGenerator::GenerateQuad()
{
	return nullptr;
}

SDXMeshData * SDXEngine::SDXMeshGenerator::GenerateCube(UINT size, SDXVertexType type, UINT subdivision,
	const XMFLOAT3& color)
{
	//SDXMeshData* pMesh = new SDXMeshData;

	//int vertexRow = (subdivision - 1) + 2;
	//int verticesInFace = (vertexRow * vertexRow);
	//int vertexCount = verticesInFace * 6;
	//int indexCount = (subdivision * subdivision * 6) * 6;

	//switch (type)
	//{
	//case SDXVERTEX_TYPE_PC:
	//	pMesh->CreateVertexArray(type, vertexCount);
	//	pMesh->CreateIndexArray(indexCount);

	//	float stepSize = size / subdivision;
	//	SDXVertexPC* vertices = static_cast<SDXVertexPC*>(pMesh->GetVertexData());
	//	int counter = 0;
	//	int rowCount = 1;

	//	// Front 
	//	float x = -(size / 2);
	//	float y = (size / 2);
	//	float z = -(size / 2);

	//	for (; counter < verticesInFace; counter++)
	//	{
	//		vertices[counter].position.x = x;
	//		vertices[counter].position.y = y;
	//		vertices[counter].position.z = z;
	//		vertices[counter].color.x = color.x;
	//		vertices[counter].color.y = color.y;
	//		vertices[counter].color.z = color.z;

	//		x += stepSize;
	//		if (rowCount >= vertexRow)
	//		{
	//			x = -(size / 2);
	//			y -= stepSize;
	//			rowCount = 1;
	//		}
	//		else
	//			rowCount++;
	//	}


	//	// Right
	//	// Back
	//	// Left
	//	// Top
	//	// Bottom
	//	break;

	//default:
	//	delete pMesh;
	//	pMesh = nullptr;
	//	break;
	//}

	//return pMesh;

	return nullptr;
}

SDXMeshData * SDXEngine::SDXMeshGenerator::GenerateSphere()
{
	return nullptr;
}
