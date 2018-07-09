#include "stdafx.h"
#include "SDXMeshGenerator.h"

using namespace SDXEngine;

SDXMeshGenerator::SDXMeshGenerator()
{
}

SDXMeshGenerator::~SDXMeshGenerator()
{
}

SDXMeshData * SDXEngine::SDXMeshGenerator::GenerateTriangle(float size, SDXVertexType type, const XMFLOAT3 & color)
{
	if (size <= 0)
		return nullptr;

	SDXMeshData* pMesh = new SDXMeshData;

	XMFLOAT3 p1(-(size / 2), -(size / 2), 0);
	XMFLOAT3 p2(0, (size / 2), 0);
	XMFLOAT3 p3((size / 2), -(size / 2), 0);

	switch (type)
	{
	case SDXVERTEX_TYPE_PC:
	{
		if (pMesh->CreateVertexArray(type, 3) != SDX_ERROR_NONE ||
			pMesh->CreateIndexArray(3) != SDX_ERROR_NONE)
		{
			delete pMesh;
			pMesh = nullptr;
			return nullptr;
		}

		SDXVertexPC* pVertices = static_cast<SDXVertexPC*>(pMesh->GetVertexData());
		unsigned int* pIndices = static_cast<unsigned int*>(pMesh->GetIndexData());

		pVertices[0].position = p1;
		pVertices[1].position = p2;
		pVertices[2].position = p3;

		pVertices[0].color = color;
		pVertices[1].color = color;
		pVertices[2].color = color;

		pIndices[0] = 0;
		pIndices[1] = 1;
		pIndices[2] = 2;
		break;
	}
	default:
		delete pMesh;
		pMesh = nullptr;
		break;
	}

	return pMesh;
}

SDXMeshData * SDXEngine::SDXMeshGenerator::GeneratePlane(float size, SDXVertexType type, UINT subdivision,
	const XMFLOAT3& color)
{
	if (type == SDXVERTEX_TYPE_UNKNOWN)
		return nullptr;

	SDXMeshData* mesh = new SDXMeshData;

	// Find total vertices
	int verticesOnEdge = 2 + (subdivision - 1);
	int totalVertices = verticesOnEdge * verticesOnEdge;

	// Find total indices
	int totalIndices = 6 * (subdivision * subdivision);

	if (mesh->CreateVertexArray(type, totalVertices) != SDX_ERROR_NONE ||
		mesh->CreateIndexArray(totalIndices) != SDX_ERROR_NONE)
	{
		delete mesh;
		mesh = nullptr;
		return nullptr;
	}

	float pX = -(size / 2.f);
	float pY = 0;
	float pZ = (size / 2.f);
	float stepSize = (float)(size / verticesOnEdge);
	int vertexCounter = 0;
	int indexCounter = 0;

	switch (type)
	{
	case SDXVERTEX_TYPE_PC:
	{
		SDXVertexPC * pVertices = static_cast<SDXVertexPC*>(mesh->GetVertexData());
		unsigned int* pIndices = mesh->GetIndexData();
		// Generate vertex data & index data
		for (int z = 0; z < verticesOnEdge; z++)
		{
			for (int x = 0; x < verticesOnEdge; x++)
			{
				pVertices[vertexCounter].position.x = pX;
				pVertices[vertexCounter].position.y = pY;
				pVertices[vertexCounter].position.z = pZ;
				pVertices[vertexCounter].color = color;

				if (x < (verticesOnEdge - 1) && z < (verticesOnEdge - 1))
				{
					pIndices[indexCounter] = vertexCounter;
					pIndices[indexCounter + 1] = vertexCounter + 1;
					pIndices[indexCounter + 2] = vertexCounter + verticesOnEdge + 1;

					pIndices[indexCounter + 3] = vertexCounter;
					pIndices[indexCounter + 4] = vertexCounter + verticesOnEdge + 1;
					pIndices[indexCounter + 5] = vertexCounter + verticesOnEdge;
					indexCounter += 6;
				}

				pX += stepSize;
				vertexCounter++;
			}
			pZ -= stepSize;
			pX = -(size / 2.f);
		}

		break;
	}
	default:
		delete mesh;
		mesh = nullptr;
		break;
	}

	return mesh;
}

SDXMeshData * SDXEngine::SDXMeshGenerator::GenerateQuad(float size, SDXVertexType type, UINT subdivision,
	const XMFLOAT3& color)
{
	if (type == SDXVERTEX_TYPE_UNKNOWN)
		return nullptr;

	SDXMeshData* mesh = new SDXMeshData;

	// Find total vertices
	int verticesOnEdge = 2 + (subdivision - 1);
	int totalVertices = verticesOnEdge * verticesOnEdge;

	// Find total indices
	int totalIndices = 6 * (subdivision * subdivision);

	if (mesh->CreateVertexArray(type, totalVertices) != SDX_ERROR_NONE ||
		mesh->CreateIndexArray(totalIndices) != SDX_ERROR_NONE)
	{
		delete mesh;
		mesh = nullptr;
		return nullptr;
	}

	float pX = -(size / 2.f);
	float pY = (size / 2.f);
	float pZ = 0;
	float stepSize = (float)(size / verticesOnEdge);
	int vertexCounter = 0;
	int indexCounter = 0;

	switch (type)
	{
	case SDXVERTEX_TYPE_PC:
	{
		SDXVertexPC * pVertices = static_cast<SDXVertexPC*>(mesh->GetVertexData());
		unsigned int* pIndices = mesh->GetIndexData();
		// Generate vertex data & index data
		for (int y = 0; y < verticesOnEdge; y++)
		{
			for (int x = 0; x < verticesOnEdge; x++)
			{
				pVertices[vertexCounter].position.x = pX;
				pVertices[vertexCounter].position.y = pY;
				pVertices[vertexCounter].position.z = pZ;
				pVertices[vertexCounter].color = color;

				if (x < (verticesOnEdge - 1) && y < (verticesOnEdge - 1))
				{
					pIndices[indexCounter] = vertexCounter;
					pIndices[indexCounter + 1] = vertexCounter + 1;
					pIndices[indexCounter + 2] = vertexCounter + verticesOnEdge + 1;

					pIndices[indexCounter + 3] = vertexCounter;
					pIndices[indexCounter + 4] = vertexCounter + verticesOnEdge + 1;
					pIndices[indexCounter + 5] = vertexCounter + verticesOnEdge;
					indexCounter += 6;

				}

				pX += stepSize;
				vertexCounter++;
			}
			pY -= stepSize;
			pX = -(size / 2.f);
		}

		break;
	}
	default:
		delete mesh;
		mesh = nullptr;
		break;
	}

	return mesh;
}

SDXMeshData * SDXEngine::SDXMeshGenerator::GenerateCube(float size, SDXVertexType type, UINT subdivision,
	const XMFLOAT3& color)
{
	if (type == SDXVERTEX_TYPE_UNKNOWN)
		return nullptr;

	SDXMeshData* mesh = new SDXMeshData;

	// Find total vertices
	int verticesOnEdge = 2 + (subdivision - 1);
	int totalFaceVertices = verticesOnEdge * verticesOnEdge;
	int totalVertices = totalFaceVertices * 6;

	// Find total indices
	int totalIndices = 6 * 6 * (subdivision * subdivision);

	if (mesh->CreateVertexArray(type, totalVertices) != SDX_ERROR_NONE ||
		mesh->CreateIndexArray(totalIndices) != SDX_ERROR_NONE)
	{
		delete mesh;
		mesh = nullptr;
		return nullptr;
	}

	float pX = 0;
	float pY = 0;
	float pZ = 0;
	float stepSize = (float)(size / subdivision);
	int vertexCounter = 0;
	int indexCounter = 0;

	switch (type)
	{
	case SDXVERTEX_TYPE_PC:
	{
		SDXVertexPC * pVertices = static_cast<SDXVertexPC*>(mesh->GetVertexData());
		unsigned int* pIndices = mesh->GetIndexData();
		
		// Front face 
		pX = -(size / 2.f);
		pY = (size / 2.f);
		pZ = -(size / 2.f);
		for (int y = 0; y < verticesOnEdge; y++)
		{
			for (int x = 0; x < verticesOnEdge; x++)
			{
				pVertices[vertexCounter].position.x = pX;
				pVertices[vertexCounter].position.y = pY;
				pVertices[vertexCounter].position.z = pZ;
				pVertices[vertexCounter].color = color;

				if (x < (verticesOnEdge - 1) && y < (verticesOnEdge - 1))
				{
					pIndices[indexCounter] = vertexCounter;
					pIndices[indexCounter + 1] = vertexCounter + 1;
					pIndices[indexCounter + 2] = vertexCounter + verticesOnEdge + 1;

					pIndices[indexCounter + 3] = vertexCounter;
					pIndices[indexCounter + 4] = vertexCounter + verticesOnEdge + 1;
					pIndices[indexCounter + 5] = vertexCounter + verticesOnEdge;
					indexCounter += 6;

				}

				pX += stepSize;
				vertexCounter++;
			}
			pY -= stepSize;
			pX = -(size / 2.f);
		}

		// Right face
		pX = (size / 2.f);
		pY = (size / 2.f);
		pZ = -(size / 2.f);
		for (int y = 0; y < verticesOnEdge; y++)
		{
			for (int z = 0; z < verticesOnEdge; z++)
			{
				pVertices[vertexCounter].position.x = pX;
				pVertices[vertexCounter].position.y = pY;
				pVertices[vertexCounter].position.z = pZ;
				pVertices[vertexCounter].color = color;

				if (z < (verticesOnEdge - 1) && y < (verticesOnEdge - 1))
				{
					pIndices[indexCounter] = vertexCounter;
					pIndices[indexCounter + 1] = vertexCounter + 1;
					pIndices[indexCounter + 2] = vertexCounter + verticesOnEdge + 1;

					pIndices[indexCounter + 3] = vertexCounter;
					pIndices[indexCounter + 4] = vertexCounter + verticesOnEdge + 1;
					pIndices[indexCounter + 5] = vertexCounter + verticesOnEdge;
					indexCounter += 6;

				}

				pZ += stepSize;
				vertexCounter++;
			}
			pY -= stepSize;
			pZ = -(size / 2.f);
		}

		// Back face 
		pX = (size / 2.f);
		pY = (size / 2.f);
		pZ = (size / 2.f);
		for (int y = 0; y < verticesOnEdge; y++)
		{
			for (int x = 0; x < verticesOnEdge; x++)
			{
				pVertices[vertexCounter].position.x = pX;
				pVertices[vertexCounter].position.y = pY;
				pVertices[vertexCounter].position.z = pZ;
				pVertices[vertexCounter].color = color;

				if (x < (verticesOnEdge - 1) && y < (verticesOnEdge - 1))
				{
					pIndices[indexCounter] = vertexCounter;
					pIndices[indexCounter + 1] = vertexCounter + 1;
					pIndices[indexCounter + 2] = vertexCounter + verticesOnEdge + 1;

					pIndices[indexCounter + 3] = vertexCounter;
					pIndices[indexCounter + 4] = vertexCounter + verticesOnEdge + 1;
					pIndices[indexCounter + 5] = vertexCounter + verticesOnEdge;
					indexCounter += 6;

				}

				pX -= stepSize;
				vertexCounter++;
			}
			pY -= stepSize;
			pX = (size / 2.f);
		}

		// Left face
		pX = -(size / 2.f);
		pY = (size / 2.f);
		pZ = (size / 2.f);
		for (int y = 0; y < verticesOnEdge; y++)
		{
			for (int z = 0; z < verticesOnEdge; z++)
			{
				pVertices[vertexCounter].position.x = pX;
				pVertices[vertexCounter].position.y = pY;
				pVertices[vertexCounter].position.z = pZ;
				pVertices[vertexCounter].color = color;

				if (z < (verticesOnEdge - 1) && y < (verticesOnEdge - 1))
				{
					pIndices[indexCounter] = vertexCounter;
					pIndices[indexCounter + 1] = vertexCounter + 1;
					pIndices[indexCounter + 2] = vertexCounter + verticesOnEdge + 1;

					pIndices[indexCounter + 3] = vertexCounter;
					pIndices[indexCounter + 4] = vertexCounter + verticesOnEdge + 1;
					pIndices[indexCounter + 5] = vertexCounter + verticesOnEdge;
					indexCounter += 6;

				}

				pZ -= stepSize;
				vertexCounter++;
			}
			pY -= stepSize;
			pZ = (size / 2.f);
		}

		// Top face
		pX = -(size / 2.f);
		pY = (size / 2.f);
		pZ = (size / 2.f);
		for (int z = 0; z < verticesOnEdge; z++)
		{
			for (int x = 0; x < verticesOnEdge; x++)
			{
				pVertices[vertexCounter].position.x = pX;
				pVertices[vertexCounter].position.y = pY;
				pVertices[vertexCounter].position.z = pZ;
				pVertices[vertexCounter].color = color;

				if (x < (verticesOnEdge - 1) && z < (verticesOnEdge - 1))
				{
					pIndices[indexCounter] = vertexCounter;
					pIndices[indexCounter + 1] = vertexCounter + 1;
					pIndices[indexCounter + 2] = vertexCounter + verticesOnEdge + 1;

					pIndices[indexCounter + 3] = vertexCounter;
					pIndices[indexCounter + 4] = vertexCounter + verticesOnEdge + 1;
					pIndices[indexCounter + 5] = vertexCounter + verticesOnEdge;
					indexCounter += 6;

				}

				pX += stepSize;
				vertexCounter++;
			}
			pZ -= stepSize;
			pX = -(size / 2.f);
		}

		// Bottom face
		pX = -(size / 2.f);
		pY = -(size / 2.f);
		pZ = -(size / 2.f);
		for (int z = 0; z < verticesOnEdge; z++)
		{
			for (int x = 0; x < verticesOnEdge; x++)
			{
				pVertices[vertexCounter].position.x = pX;
				pVertices[vertexCounter].position.y = pY;
				pVertices[vertexCounter].position.z = pZ;
				pVertices[vertexCounter].color = color;

				if (x < (verticesOnEdge - 1) && z < (verticesOnEdge - 1))
				{
					pIndices[indexCounter] = vertexCounter;
					pIndices[indexCounter + 1] = vertexCounter + 1;
					pIndices[indexCounter + 2] = vertexCounter + verticesOnEdge + 1;

					pIndices[indexCounter + 3] = vertexCounter;
					pIndices[indexCounter + 4] = vertexCounter + verticesOnEdge + 1;
					pIndices[indexCounter + 5] = vertexCounter + verticesOnEdge;
					indexCounter += 6;

				}

				pX += stepSize;
				vertexCounter++;
			}
			pZ += stepSize;
			pX = -(size / 2.f);
		}
		break;
	}
	default:
		delete mesh;
		mesh = nullptr;
		break;
	}

	return mesh;
}

SDXMeshData * SDXEngine::SDXMeshGenerator::GenerateSphere()
{
	return nullptr;
}
