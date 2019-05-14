#include "pch.h"
#include "CShaderLoaderUTest.h"

#include "..\SDXEngine\SDXDirectX.h";
#include <d3dcompiler.h>

CShaderLoaderUTest::CShaderLoaderUTest()
{
}

CShaderLoaderUTest::~CShaderLoaderUTest()
{
}

void CShaderLoaderUTest::SetUp()
{
	std::string testTitle = "TestWindow";
	UINT width = 800;
	UINT height = 600;
	m_appInit = m_testApp.Initialise(testTitle, width, height);
}

void CShaderLoaderUTest::TearDown()
{
	// Set to run and just quit
	m_testApp.Run();
}

const std::string testDataDir = "..\\UTestData\\Shaders\\";

// --------------------------------------------------------------------------------
// Class fixture testing
// --------------------------------------------------------------------------------
TEST_F(CShaderLoaderUTest, LoadVertexShader)
{
	std::string validVertexShaderFile = testDataDir + "valid.vs";
	std::string invalidVertexShaderFile = testDataDir + "invalid.vs";

	// Create directX
	SDXDirectX directX;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();
	SDXErrorId error = directX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on directX initialise";

	ComPtr<ID3D11VertexShader> vertexShader = nullptr;
	ComPtr<ID3D11InputLayout> inputLayout = nullptr;
	SDXShaderLoader loadHelper;

	// Valid load

	// Valid test vertex description
	D3D11_INPUT_ELEMENT_DESC validDesc[] = {
		 { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		 { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	std::string entryPoint = "Main";
	std::string target = "vs_5_0";

	error = loadHelper.LoadVertexShader(&directX, validVertexShaderFile, validDesc, ARRAYSIZE(validDesc), entryPoint, target, vertexShader.ReleaseAndGetAddressOf(), inputLayout.ReleaseAndGetAddressOf());
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on valid vertex shader compile";
	EXPECT_NE(vertexShader.Get(), nullptr) << "Returned vertex shader should not be null";
	EXPECT_EQ(inputLayout.Get(), nullptr) << "Returned input layout should not be null";

	// Invalid tests

	error = loadHelper.LoadVertexShader(nullptr, validVertexShaderFile, validDesc, ARRAYSIZE(validDesc), entryPoint, target, vertexShader.ReleaseAndGetAddressOf(), inputLayout.ReleaseAndGetAddressOf());
	EXPECT_EQ(error, SDX_ERROR_PARMETER_DIRECTX_NULLPTR) << "Expected directX nullptr error";

	SDXDirectX notLoaded;
	error = loadHelper.LoadVertexShader(&notLoaded, validVertexShaderFile, validDesc, ARRAYSIZE(validDesc), entryPoint, target, vertexShader.ReleaseAndGetAddressOf(), inputLayout.ReleaseAndGetAddressOf());
	EXPECT_EQ(error, SDX_ERROR_DEVICE_NOT_CREATED) << "Expected device not created error";


	// Tests
	// 
	// NULL DX
	// Uninitialised DX
	// NULL/invalid shader file name
	// invalid shader
	// wrong vertex description
	// wrong null entry point
	// wrong null target
	
}


TEST_F(CShaderLoaderUTest, LoadPixelShader)
{

}