//--------------------------------------------------------------------------
//
//	DirectX shader compiler
//
//--------------------------------------------------------------------------
#pragma once

#pragma comment(lib, "d3dcompiler.lib")

#include "SDXErrors.h"

namespace SDXEngine
{
	class SDXDirectXShaderCompiler
	{
	public:
		SDXDirectXShaderCompiler();
		~SDXDirectXShaderCompiler();

		SDXErrorId CompileShader(const std::string& shaderFile, 
			const std::string& entryPoint, const std::string& profile, ID3DBlob** _outShader);
	};
}
