//--------------------------------------------------------------------------
//
// Vertex and index buffer manager
//
//	Currently unused
//
//--------------------------------------------------------------------------
#pragma once

#include <vector>
#include <map>
#include <string>
#include "SDXVertexBuffer.h"
#include "SDXIndexBuffer.h"

namespace SDXEngine
{
	class SDXBufferMGR
	{
	public:
		SDXBufferMGR();
		~SDXBufferMGR();

	private:
		std::vector<SDXVertexBuffer> m_vertexBuffers;
		std::vector<SDXIndexBuffer> m_indexBuffers;
		std::map<std::string, int> m_vertexMap;
		std::map<std::string, int> m_indexMap;
	};
}