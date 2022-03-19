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
	case SDXVertexType::SDXVERTEX_TYPE_PC:
	{
		if (pMesh->CreateVertexArray(type, 3) != SDXErrorId::SDX_ERROR_NONE ||
			pMesh->CreateIndexArray(3) != SDXErrorId::SDX_ERROR_NONE)
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

	case SDXVertexType::SDXVERTEX_TYPE_PNC:
	{
		if (pMesh->CreateVertexArray(type, 3) != SDXErrorId::SDX_ERROR_NONE ||
			pMesh->CreateIndexArray(3) != SDXErrorId::SDX_ERROR_NONE)
		{
			delete pMesh;
			pMesh = nullptr;
			return pMesh;
		}

		SDXVertexPNC* pVertices = static_cast<SDXVertexPNC*>(pMesh->GetVertexData());
		unsigned int* pIndices = static_cast<unsigned int*>(pMesh->GetIndexData());

		pVertices[0].position = p1;
		pVertices[1].position = p2;
		pVertices[2].position = p3;

		pVertices[0].normal = XMFLOAT3(0, 0, -1);
		pVertices[1].normal = XMFLOAT3(0, 0, -1);
		pVertices[2].normal = XMFLOAT3(0, 0, -1);

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
	const XMFLOAT3& color, float texScaleX, float texScaleY)
{
	if (type == SDXVertexType::SDXVERTEX_TYPE_UNKNOWN)
		return nullptr;

	SDXMeshData* mesh = new SDXMeshData;

	// Find total vertices
	int verticesOnEdge = 2 + (subdivision - 1);
	int totalVertices = verticesOnEdge * verticesOnEdge;

	// Find total indices
	int totalIndices = 6 * (subdivision * subdivision);

	if (mesh->CreateVertexArray(type, totalVertices) != SDXErrorId::SDX_ERROR_NONE ||
		mesh->CreateIndexArray(totalIndices) != SDXErrorId::SDX_ERROR_NONE)
	{
		delete mesh;
		mesh = nullptr;
		return nullptr;
	}

	float pX = -(size / 2.f);
	float pY = 0;
	float pZ = (size / 2.f);
	float stepSize = (float)(size / subdivision);
	int vertexCounter = 0;
	int indexCounter = 0;

	switch (type)
	{
	case SDXVertexType::SDXVERTEX_TYPE_PC:
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
	case SDXVertexType::SDXVERTEX_TYPE_PNC:
	{
		SDXVertexPNC * pVertices = static_cast<SDXVertexPNC*>(mesh->GetVertexData());
		unsigned int* pIndices = mesh->GetIndexData();
		// Generate vertex data & index data
		for (int z = 0; z < verticesOnEdge; z++)
		{
			for (int x = 0; x < verticesOnEdge; x++)
			{
				pVertices[vertexCounter].position.x = pX;
				pVertices[vertexCounter].position.y = pY;
				pVertices[vertexCounter].position.z = pZ;
				pVertices[vertexCounter].normal.x = 0;
				pVertices[vertexCounter].normal.y = 1;
				pVertices[vertexCounter].normal.z = 0;
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
	case SDXVertexType::SDXVERTEX_TYPE_PNT:
	{
		SDXVertexPNT* pVertices = static_cast<SDXVertexPNT*>(mesh->GetVertexData());
		unsigned int* pIndices = mesh->GetIndexData();
		// Generate vertex data & index data
		for (int z = 0; z < verticesOnEdge; z++)
		{
			for (int x = 0; x < verticesOnEdge; x++)
			{
				pVertices[vertexCounter].position.x = pX;
				pVertices[vertexCounter].position.y = pY;
				pVertices[vertexCounter].position.z = pZ;
				pVertices[vertexCounter].normal.x = 0;
				pVertices[vertexCounter].normal.y = 1;
				pVertices[vertexCounter].normal.z = 0;
				pVertices[vertexCounter].textureCoord.x = ((float)x / (float)(verticesOnEdge - 1)) * texScaleX;
				pVertices[vertexCounter].textureCoord.y = (1.f - ((float)(verticesOnEdge - 1) - (float)z) / (float)(verticesOnEdge - 1)) * texScaleY;

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
	const XMFLOAT3& color, float texScaleX, float texScaleY)
{
	if (type == SDXVertexType::SDXVERTEX_TYPE_UNKNOWN)
		return nullptr;

	SDXMeshData* mesh = new SDXMeshData;

	// Find total vertices
	int verticesOnEdge = 2 + (subdivision - 1);
	int totalVertices = verticesOnEdge * verticesOnEdge;

	// Find total indices
	int totalIndices = 6 * (subdivision * subdivision);

	if (mesh->CreateVertexArray(type, totalVertices) != SDXErrorId::SDX_ERROR_NONE ||
		mesh->CreateIndexArray(totalIndices) != SDXErrorId::SDX_ERROR_NONE)
	{
		delete mesh;
		mesh = nullptr;
		return nullptr;
	}

	float pX = -(size / 2.f);
	float pY = (size / 2.f);
	float pZ = 0;
	float stepSize = (float)(size / subdivision);
	int vertexCounter = 0;
	int indexCounter = 0;

	switch (type)
	{
	case SDXVertexType::SDXVERTEX_TYPE_PC:
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
	case SDXVertexType::SDXVERTEX_TYPE_PNC:
	{
		SDXVertexPNC * pVertices = static_cast<SDXVertexPNC*>(mesh->GetVertexData());
		unsigned int* pIndices = mesh->GetIndexData();
		// Generate vertex data & index data
		for (int y = 0; y < verticesOnEdge; y++)
		{
			for (int x = 0; x < verticesOnEdge; x++)
			{
				pVertices[vertexCounter].position.x = pX;
				pVertices[vertexCounter].position.y = pY;
				pVertices[vertexCounter].position.z = pZ;
				pVertices[vertexCounter].normal.x = 0;
				pVertices[vertexCounter].normal.y = 0;
				pVertices[vertexCounter].normal.z = -1;
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
	case SDXVertexType::SDXVERTEX_TYPE_PNT:
	{
		SDXVertexPNT* pVertices = static_cast<SDXVertexPNT*>(mesh->GetVertexData());
		unsigned int* pIndices = mesh->GetIndexData();

		float tU = 0.f;
		float tV = 1.f;
		float incrementsU = 1.f / static_cast<float>(verticesOnEdge - 1);
		float incrementsV = 1.f / static_cast<float>(verticesOnEdge - 1);
		// Generate vertex data & index data
		for (int y = 0; y < verticesOnEdge; y++)
		{
			for (int x = 0; x < verticesOnEdge; x++)
			{
				pVertices[vertexCounter].position.x = pX;
				pVertices[vertexCounter].position.y = pY;
				pVertices[vertexCounter].position.z = pZ;
				pVertices[vertexCounter].normal.x = 0;
				pVertices[vertexCounter].normal.y = 0;
				pVertices[vertexCounter].normal.z = -1;
				pVertices[vertexCounter].textureCoord.x = ((float)x / (float)(verticesOnEdge - 1)) * texScaleX;
				pVertices[vertexCounter].textureCoord.y = (1.f - ((float)(verticesOnEdge - 1) - (float)y) / (float)(verticesOnEdge - 1)) * texScaleY;

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

SDXMeshData* SDXEngine::SDXMeshGenerator::GenerateQuad(float width, float height, SDXVertexType type, UINT subDivisionW, UINT subDivisionH, const XMFLOAT3& color,
	float texScaleX, float texScaleY)
{
	if (type == SDXVertexType::SDXVERTEX_TYPE_UNKNOWN)
		return nullptr;

	SDXMeshData* mesh = new SDXMeshData;

	// Find total vertices
	int verticesOnEdgeW = 2 + (subDivisionW - 1);
	int verticesOnEdgeH = 2 + (subDivisionH - 1);
	int totalVertices = verticesOnEdgeW * verticesOnEdgeH;

	// Find total indices
	int totalIndices = 6 * (subDivisionW * subDivisionH);

	if (mesh->CreateVertexArray(type, totalVertices) != SDXErrorId::SDX_ERROR_NONE ||
		mesh->CreateIndexArray(totalIndices) != SDXErrorId::SDX_ERROR_NONE)
	{
		delete mesh;
		mesh = nullptr;
		return nullptr;
	}

	float pX = -(width / 2.f);
	float pY = (height / 2.f);
	float pZ = 0;
	float stepSizeX = (float)(width / subDivisionW);
	float stepSizeY = (float)(height / subDivisionH);
	int vertexCounter = 0;
	int indexCounter = 0;

	switch (type)
	{
	case SDXVertexType::SDXVERTEX_TYPE_PC:
	{
		SDXVertexPC* pVertices = static_cast<SDXVertexPC*>(mesh->GetVertexData());
		unsigned int* pIndices = mesh->GetIndexData();
		// Generate vertex data & index data
		for (int y = 0; y < verticesOnEdgeH; y++)
		{
			for (int x = 0; x < verticesOnEdgeW; x++)
			{
				pVertices[vertexCounter].position.x = pX;
				pVertices[vertexCounter].position.y = pY;
				pVertices[vertexCounter].position.z = pZ;
				pVertices[vertexCounter].color = color;

				if (x < (verticesOnEdgeW - 1) && y < (verticesOnEdgeH - 1))
				{
					pIndices[indexCounter] = vertexCounter;
					pIndices[indexCounter + 1] = vertexCounter + 1;
					pIndices[indexCounter + 2] = vertexCounter + verticesOnEdgeW + 1;

					pIndices[indexCounter + 3] = vertexCounter;
					pIndices[indexCounter + 4] = vertexCounter + verticesOnEdgeW + 1;
					pIndices[indexCounter + 5] = vertexCounter + verticesOnEdgeW;
					indexCounter += 6;

				}

				pX += stepSizeX;
				vertexCounter++;
			}
			pY -= stepSizeY;
			pX = -(width / 2.f);
		}
		break;
	}
	case SDXVertexType::SDXVERTEX_TYPE_PNC:
	{
		SDXVertexPNC* pVertices = static_cast<SDXVertexPNC*>(mesh->GetVertexData());
		unsigned int* pIndices = mesh->GetIndexData();
		// Generate vertex data & index data
		for (int y = 0; y < verticesOnEdgeH; y++)
		{
			for (int x = 0; x < verticesOnEdgeW; x++)
			{
				pVertices[vertexCounter].position.x = pX;
				pVertices[vertexCounter].position.y = pY;
				pVertices[vertexCounter].position.z = pZ;
				pVertices[vertexCounter].normal.x = 0;
				pVertices[vertexCounter].normal.y = 0;
				pVertices[vertexCounter].normal.z = -1;
				pVertices[vertexCounter].color = color;

				if (x < (verticesOnEdgeW - 1) && y < (verticesOnEdgeH - 1))
				{
					pIndices[indexCounter] = vertexCounter;
					pIndices[indexCounter + 1] = vertexCounter + 1;
					pIndices[indexCounter + 2] = vertexCounter + verticesOnEdgeW + 1;

					pIndices[indexCounter + 3] = vertexCounter;
					pIndices[indexCounter + 4] = vertexCounter + verticesOnEdgeW + 1;
					pIndices[indexCounter + 5] = vertexCounter + verticesOnEdgeW;
					indexCounter += 6;

				}

				pX += stepSizeX;
				vertexCounter++;
			}
			pY -= stepSizeY;
			pX = -(width / 2.f);
		}
		break;
	}
	case SDXVertexType::SDXVERTEX_TYPE_PNT:
	{
		SDXVertexPNT* pVertices = static_cast<SDXVertexPNT*>(mesh->GetVertexData());
		unsigned int* pIndices = mesh->GetIndexData();

		float tU = 0.f;
		float tV = 1.f;
		float incrementsU = 1.f / static_cast<float>(verticesOnEdgeW - 1);
		float incrementsV = 1.f / static_cast<float>(verticesOnEdgeH - 1);
		// Generate vertex data & index data
		for (int y = 0; y < verticesOnEdgeH; y++)
		{
			for (int x = 0; x < verticesOnEdgeW; x++)
			{
				pVertices[vertexCounter].position.x = pX;
				pVertices[vertexCounter].position.y = pY;
				pVertices[vertexCounter].position.z = pZ;
				pVertices[vertexCounter].normal.x = 0;
				pVertices[vertexCounter].normal.y = 0;
				pVertices[vertexCounter].normal.z = -1;
				pVertices[vertexCounter].textureCoord.x = ((float)x / (float)(verticesOnEdgeW - 1)) * texScaleX;
				pVertices[vertexCounter].textureCoord.y = (1.f - ((float)(verticesOnEdgeH - 1) - (float)y) / (float)(verticesOnEdgeH - 1)) * texScaleY;

				if (x < (verticesOnEdgeW - 1) && y < (verticesOnEdgeH - 1))
				{
					pIndices[indexCounter] = vertexCounter;
					pIndices[indexCounter + 1] = vertexCounter + 1;
					pIndices[indexCounter + 2] = vertexCounter + verticesOnEdgeW + 1;

					pIndices[indexCounter + 3] = vertexCounter;
					pIndices[indexCounter + 4] = vertexCounter + verticesOnEdgeW + 1;
					pIndices[indexCounter + 5] = vertexCounter + verticesOnEdgeW;
					indexCounter += 6;

				}

				pX += stepSizeX;
				vertexCounter++;
			}
			pY -= stepSizeY;
			pX = -(width / 2.f);
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
	if (type == SDXVertexType::SDXVERTEX_TYPE_UNKNOWN)
		return nullptr;

	SDXMeshData* mesh = new SDXMeshData;

	// Find total vertices
	int verticesOnEdge = 2 + (subdivision - 1);
	int totalFaceVertices = verticesOnEdge * verticesOnEdge;
	int totalVertices = totalFaceVertices * 6;

	// Find total indices
	int totalIndices = 6 * 6 * (subdivision * subdivision);

	if (mesh->CreateVertexArray(type, totalVertices) != SDXErrorId::SDX_ERROR_NONE ||
		mesh->CreateIndexArray(totalIndices) != SDXErrorId::SDX_ERROR_NONE)
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
	case SDXVertexType::SDXVERTEX_TYPE_PC:
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
	case SDXVertexType::SDXVERTEX_TYPE_PNC:
	{
		SDXVertexPNC * pVertices = static_cast<SDXVertexPNC*>(mesh->GetVertexData());
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
				pVertices[vertexCounter].normal.x = 0;
				pVertices[vertexCounter].normal.y = 0;
				pVertices[vertexCounter].normal.z = -1;
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
				pVertices[vertexCounter].normal.x = 1;
				pVertices[vertexCounter].normal.y = 0;
				pVertices[vertexCounter].normal.z = 0;
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
				pVertices[vertexCounter].normal.x = 0;
				pVertices[vertexCounter].normal.y = 0;
				pVertices[vertexCounter].normal.z = 1;
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
				pVertices[vertexCounter].normal.x = -1;
				pVertices[vertexCounter].normal.y = 0;
				pVertices[vertexCounter].normal.z = 0;
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
				pVertices[vertexCounter].normal.x = 0;
				pVertices[vertexCounter].normal.y = 1;
				pVertices[vertexCounter].normal.z = 0;
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
				pVertices[vertexCounter].normal.x = 0;
				pVertices[vertexCounter].normal.y = -1;
				pVertices[vertexCounter].normal.z = 0;
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
