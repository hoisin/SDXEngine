/*
	Helper class handling loading of shaders from file
*/
#pragma once

#include "SDXDirectX.h"

namespace SDXEngine
{
	class SDXShaderLoader
	{
	public:
		SDXShaderLoader();
		~SDXShaderLoader();

		SDXErrorId LoadVertexShader(SDXDirectX* pDX, const std::string vertexShaderFile, 
			D3D11_INPUT_ELEMENT_DESC* desc, int inputElements, const std::string& entryPoint, const std::string& target,
			ID3D11VertexShader** outShader, ID3D11InputLayout** outLayout);
		 
		SDXErrorId LoadPixelShader(SDXDirectX* pDX, const std::string pixelShaderFile, 
			const std::string& entryPoint, const std::string& target, ID3D11PixelShader** outShader);
	};
};