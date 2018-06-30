#include "stdafx.h"
#include <d3dcompiler.h>
#include "SDXDirectXShaderCompiler.h"

using namespace SDXEngine;

SDXDirectXShaderCompiler::SDXDirectXShaderCompiler()
{
}


SDXDirectXShaderCompiler::~SDXDirectXShaderCompiler()
{
}

SDXErrorId SDXEngine::SDXDirectXShaderCompiler::CompileShader(const std::string & shaderFile, const std::string & entryPoint, const std::string & profile, ID3DBlob ** _outShader)
{
	if (shaderFile.empty() || entryPoint.empty() || profile.empty() || !_outShader)
		return SDX_ERROR_DXCOMPILER_COMPILE_INVALID_ARGS;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT result = D3DCompileFromFile(StringToWideString(shaderFile).c_str(),
		NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), profile.c_str(), flags,
		0, &shaderBlob, &errorBlob);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (shaderBlob)
			shaderBlob->Release();

		return SDX_ERROR_DXCOMPILER_COMPILE_FAILED;
	}

	*_outShader = shaderBlob;

	return SDX_ERROR_NONE;
}
