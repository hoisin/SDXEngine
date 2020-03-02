//--------------------------------------------------------------------------
//
// Graphics includes
//
//--------------------------------------------------------------------------

#pragma once

#include <DirectXMath.h>

namespace SDXEngine
{
	using namespace DirectX;

	enum SDXVertexType
	{
		SDXVERTEX_TYPE_UNKNOWN,
		SDXVERTEX_TYPE_PC,				// Position, color
		SDXVERTEX_TYPE_PNC,				// Position, normal, color
		SDXVERTEX_TYPE_PNT,				// Position, normal, texture coordinate
		SDXVERTEX_TYPE_PNTT,			// Position, normal, tangent, texture coordinate
		SDXVERTEX_TYPE_TOTAL
	};

	struct SDXVertex
	{
	};

	// Position and color
	struct SDXVertexPC : public SDXVertex
	{
		XMFLOAT3 position;
		XMFLOAT3 color;

		SDXVertexPC(const XMFLOAT3& pos, const XMFLOAT3& col) :
			position(pos), color(col)
		{
		}

		SDXVertexPC() : position(XMFLOAT3(0,0,0)), color(XMFLOAT3(0,0,0))
		{
		}
	};

	// Position, normal and color
	struct SDXVertexPNC : public SDXVertex
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT3 color;

		SDXVertexPNC(const XMFLOAT3& pos, const XMFLOAT3& norm,
			const XMFLOAT3& col) :
			position(pos), normal(norm), color(col)
		{
		}

		SDXVertexPNC() : position(XMFLOAT3(0, 0, 0)), normal(XMFLOAT3(0, 0, 0)), color(XMFLOAT3(0, 0, 0))
		{
		}
	};

	// Position, normal texture
	struct SDXVertexPNT : public SDXVertex
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 textureCoord;

		SDXVertexPNT(const XMFLOAT3& pos, const XMFLOAT3& norm,
			const XMFLOAT2& texCoord) :
			position(pos), normal(norm), textureCoord(texCoord)
		{
		}

		SDXVertexPNT() : position(XMFLOAT3(0,0,0)), normal(XMFLOAT3(0,0,0)), textureCoord(XMFLOAT2(0,0))
		{
		}
	};

	inline int GetSizeOfVertexType(SDXVertexType type)
	{
		int size = 0;

		switch (type)
		{
		case SDXVERTEX_TYPE_PC:
			size = sizeof(SDXVertexPC);
			break;

		case SDXVERTEX_TYPE_PNC:
			size = sizeof(SDXVertexPNC);
			break;

		case SDXVERTEX_TYPE_PNT:
			size = sizeof(SDXVertexPNT);
			break;

		default:

			break;
		}

		return size;
	}

	struct SMaterial
	{
		std::string diffuseTextureID = "";
		std::string shaderID = "";
		
		// Could include rasterstate but not required yet.
		// Probably shift rasterstate to a MGR and refer to as ID when needed.

		// To add more params when required such as...
		//
		// Material light params:
		// - specular val, col
		// - diffuse col?
		// - bump/normal texture
		// - displacement texture
		// - shadowmap?
		// - cubemap?
		// - ambient??

		SMaterial()
		{
		}

		void operator=(const SMaterial& other)
		{
			diffuseTextureID = other.diffuseTextureID;
			shaderID = other.shaderID;
		}
	};

	// CBuffer definitions
	// Testing for vertex shader uniform
	typedef struct _constantBufferStruct {
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	} ConstantBufferStruct;
}       