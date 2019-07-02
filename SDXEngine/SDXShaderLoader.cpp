#include "stdafx.h"
#include "SDXShaderLoader.h"
#include <d3dcompiler.h>

using namespace SDXEngine;

SDXShaderLoader::SDXShaderLoader()
{
}


SDXShaderLoader::~SDXShaderLoader()
{
}

SDXErrorId SDXEngine::SDXShaderLoader::LoadVertexShader(SDXDirectX* pDX, const std::string vertexShaderFile,
	D3D11_INPUT_ELEMENT_DESC* desc, int inputElements, const std::string& entryPoint, const std::string& target,
	ID3D11VertexShader** outShader, ID3D11InputLayout** outLayout)
{
	if (pDX == nullptr)
		return SDX_ERROR_PARMETER_DIRECTX_NULLPTR;

	if (pDX->GetDevice() == nullptr)
		return SDX_ERROR_DEVICE_NOT_CREATED;

	SDXErrorId error = SDX_ERROR_NONE;
	
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	std::wstring ws(vertexShaderFile.begin(), vertexShaderFile.end());
	HRESULT result = D3DCompileFromFile(ws.c_str(),
		NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), target.c_str(), flags,
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

		return SDX_ERROR_SHADER_COMPILE_FAILED;
	}

	// Create vertex shader
	result = pDX->GetDevice()->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, outShader);
	if (FAILED(result))
	{
		shaderBlob->Release();
		return SDX_ERROR_VERTEXSHADER_CREATE_FAILED;
	}

	result = pDX->GetDevice()->CreateInputLayout(desc, inputElements, shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), outLayout);
	if (FAILED(result))
	{
		shaderBlob->Release();
		return SDX_ERROR_INPUTLAYOUT_CREATE_FAILED;
	}
	shaderBlob->Release();

	return error;
}

SDXErrorId SDXEngine::SDXShaderLoader::LoadPixelShader(SDXDirectX* pDX, const std::string pixelShaderFile,
	const std::string& entryPoint, const std::string& target, ID3D11PixelShader** outShader)
{
	if (pDX == nullptr)
		return SDX_ERROR_PARMETER_DIRECTX_NULLPTR;

	if (pDX->GetDevice() == nullptr)
		return SDX_ERROR_DEVICE_NOT_CREATED;

	SDXErrorId error = SDX_ERROR_NONE;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	std::wstring ws(pixelShaderFile.begin(), pixelShaderFile.end());
	HRESULT result = D3DCompileFromFile(ws.c_str(),
		NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), target.c_str(), flags,
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

		return SDX_ERROR_SHADER_COMPILE_FAILED;
	}

	result = pDX->GetDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, outShader);
	if (FAILED(result))
	{
		shaderBlob->Release();
		return SDX_ERROR_PIXELSHADER_CREATE_FAILED;
	}

	shaderBlob->Release();

	return error;
}
