#pragma once

#include "SDXDirectX.h"
#include "SDXErrors.h"
#include "SDXShaderLoader.h"
 
namespace SDXEngine
{
	struct SShader
	{
		ComPtr<ID3D11VertexShader> vertexShader = nullptr;
		ComPtr<ID3D11PixelShader> pixelShader = nullptr;
		ComPtr<ID3D11InputLayout> inputLayout = nullptr;
		std::string id = "";
	};

	struct SCBuffer
	{
		ComPtr<ID3D11Buffer> cBuffer = nullptr;
		std::string id = "";
	};

	class SDXShaderMGR
	{
	public:
		SDXShaderMGR();
		~SDXShaderMGR();

		SDXErrorId Initialise(SDXDirectX* pDX);

		SDXErrorId LoadShader();

		SDXErrorId BindConstant();

		SShader* GetShader(UINT handle);
		SShader* GetShader(const std::string* id);

		ComPtr<ID3D11Buffer> GetCBuffer(UINT hangle);
		ComPtr<ID3D11Buffer> GetCBuffer(const std::string& id);

	private:
		SDXErrorId LoadVertexShader();

		SDXErrorId LoadPixelShader();

	private:
		SDXShaderLoader m_loadHelper;
	};
};