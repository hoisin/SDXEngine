#include "pch.h"
#include "SDXShaderLoaderUTest.h"

#include "..\SDXEngine\SDXDirectX.h"
#include <d3dcompiler.h>

SDXShaderLoaderUTest::SDXShaderLoaderUTest()
{
}

SDXShaderLoaderUTest::~SDXShaderLoaderUTest()
{
}

void SDXShaderLoaderUTest::SetUp()
{
	std::string testTitle = "TestWindow";
	UINT width = 800;
	UINT height = 600;
	m_appInit = m_testApp.Initialise(testTitle, width, height);
}

void SDXShaderLoaderUTest::TearDown()
{
	// Set to run and just quit
	m_testApp.Run();
}

const std::string testDataDir = "..\\UTestData\\Shaders\\";

// --------------------------------------------------------------------------------
// Class fixture testing
// --------------------------------------------------------------------------------
TEST_F(SDXShaderLoaderUTest, LoadVertexShader)
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
	EXPECT_NE(inputLayout.Get(), nullptr) << "Returned input layout should not be null";

	// Invalid tests

	// Null directX
	error = loadHelper.LoadVertexShader(nullptr, validVertexShaderFile, validDesc, ARRAYSIZE(validDesc), entryPoint, target, vertexShader.ReleaseAndGetAddressOf(), inputLayout.ReleaseAndGetAddressOf());
	EXPECT_EQ(error, SDX_ERROR_PARMETER_DIRECTX_NULLPTR) << "Expected directX nullptr error";

	// Uninitialised directX
	SDXDirectX notLoaded;
	error = loadHelper.LoadVertexShader(&notLoaded, validVertexShaderFile, validDesc, ARRAYSIZE(validDesc), entryPoint, target, vertexShader.ReleaseAndGetAddressOf(), inputLayout.ReleaseAndGetAddressOf());
	EXPECT_EQ(error, SDX_ERROR_DEVICE_NOT_CREATED) << "Expected device not created error";

	// Empty/wrong file name
	std::string invalidFile = "";
	error = loadHelper.LoadVertexShader(&directX, invalidFile, validDesc, ARRAYSIZE(validDesc), entryPoint, target, vertexShader.ReleaseAndGetAddressOf(), inputLayout.ReleaseAndGetAddressOf());
	EXPECT_NE(error, SDX_ERROR_NONE) << "Expected error on invalid file name";

	// Null shader desc
	error = loadHelper.LoadVertexShader(&directX, validVertexShaderFile, nullptr, ARRAYSIZE(validDesc), entryPoint, target, vertexShader.ReleaseAndGetAddressOf(), inputLayout.ReleaseAndGetAddressOf());
	EXPECT_NE(error, SDX_ERROR_NONE) << "Expected error on null vertex description";

	// Null/invalid entry point
	error = loadHelper.LoadVertexShader(&directX, validVertexShaderFile, validDesc, ARRAYSIZE(validDesc), "", target, vertexShader.ReleaseAndGetAddressOf(), inputLayout.ReleaseAndGetAddressOf());
	EXPECT_NE(error, SDX_ERROR_NONE) << "Expected error on invalid entry point";

	// Null/invalid target
	error = loadHelper.LoadVertexShader(&directX, validVertexShaderFile, validDesc, ARRAYSIZE(validDesc), entryPoint, "", vertexShader.ReleaseAndGetAddressOf(), inputLayout.ReleaseAndGetAddressOf());
	EXPECT_NE(error, SDX_ERROR_NONE) << "Expected error on invalid target";

	// Load invalid file
	error = loadHelper.LoadVertexShader(&directX, invalidVertexShaderFile, validDesc, ARRAYSIZE(validDesc), entryPoint, target, vertexShader.ReleaseAndGetAddressOf(), inputLayout.ReleaseAndGetAddressOf());
	EXPECT_NE(error, SDX_ERROR_NONE) << "Expected error on errors in shader file";
}


TEST_F(SDXShaderLoaderUTest, LoadPixelShader)
{
	std::string validPixelShaderFile = testDataDir + "valid.ps";
	std::string invalidPixelShaderFile = testDataDir + "invalid.ps";

	// Create directX
	SDXDirectX directX;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();
	SDXErrorId error = directX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on directX initialise";

	ComPtr<ID3D11PixelShader> pixelShader;
	SDXShaderLoader loadHelper;

	std::string entryPoint = "Main";
	std::string target = "ps_5_0";

	// Valid test
	error = loadHelper.LoadPixelShader(&directX, validPixelShaderFile, entryPoint, target, pixelShader.ReleaseAndGetAddressOf());
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on valid pixel shader load";

	// Invalid tests

	// Null directX
	error = loadHelper.LoadPixelShader(nullptr, validPixelShaderFile, entryPoint, target, pixelShader.ReleaseAndGetAddressOf());
	EXPECT_EQ(error, SDX_ERROR_PARMETER_DIRECTX_NULLPTR) << "Expected directX nullptr error";

	// DirectX not loaded
	SDXDirectX notLoaded;
	error = loadHelper.LoadPixelShader(&notLoaded, validPixelShaderFile, entryPoint, target, pixelShader.ReleaseAndGetAddressOf());
	EXPECT_EQ(error, SDX_ERROR_DEVICE_NOT_CREATED) << "Expected device not created error";

	// Empty/wrong file name
	std::string invalidFile = "";
	error = loadHelper.LoadPixelShader(&directX, invalidFile, entryPoint, target, pixelShader.ReleaseAndGetAddressOf());
	EXPECT_NE(error, SDX_ERROR_NONE) << "Expected error on invalid file name";

	// Invalid entry point
	error = loadHelper.LoadPixelShader(&directX, validPixelShaderFile, "", target, pixelShader.ReleaseAndGetAddressOf());
	EXPECT_NE(error, SDX_ERROR_NONE) << "EXpected error on invalid entry point";

	// Invalid target 
	error = loadHelper.LoadPixelShader(&directX, validPixelShaderFile, entryPoint, "", pixelShader.ReleaseAndGetAddressOf());
	EXPECT_NE(error, SDX_ERROR_NONE) << "Expected error on invalid target";

	// Invalid pixel shader file
	error = loadHelper.LoadPixelShader(&directX, invalidPixelShaderFile, entryPoint, target, pixelShader.ReleaseAndGetAddressOf());
	EXPECT_NE(error, SDX_ERROR_NONE) << "Expected error on errors in pixel shader";
}