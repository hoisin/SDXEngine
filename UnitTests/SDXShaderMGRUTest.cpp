#include "pch.h"
#include "SDXShaderMGRUTest.h"

#include "..\SDXEngine\SDXDirectX.h"

SDXShaderMGRUTest::SDXShaderMGRUTest()
{
}

SDXShaderMGRUTest::~SDXShaderMGRUTest()
{
}

void SDXShaderMGRUTest::SetUp()
{
	std::string testTitle = "TestWindow";
	UINT width = 800;
	UINT height = 600;
	m_appInit = m_testApp.Initialise(testTitle, width, height);
}

void SDXShaderMGRUTest::TearDown()
{
	// Set to run and just quit
	m_testApp.Run();
}


// Globals
const std::string testDataDir = "..\\UTestData\\Shaders\\";

std::string validVertexShader = testDataDir + "valid.vs";
std::string validPixelShader = testDataDir + "valid.ps";

// Valid test vertex description
D3D11_INPUT_ELEMENT_DESC validDesc[] = {
	 { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
	 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	 { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT,
	 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

std::string entryPoint = "Main";
std::string vsTarget = "vs_5_0";
std::string psTarget = "ps_5_0";


typedef struct _testBuffer
{
	XMFLOAT4 data = XMFLOAT4(0, 0, 0, 0);
} TestBuffer;

TestBuffer g_testBuffer;

// --------------------------------------------------------------------------------
// Class fixture testing
// --------------------------------------------------------------------------------
TEST_F(SDXShaderMGRUTest, Initialise)
{
	// Create directX
	SDXDirectX directX;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();
	SDXErrorId error = directX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on directX initialise";

	SDXShaderMGR mgr;

	// Invalid initialise
	error = mgr.Initialise(nullptr);
	EXPECT_EQ(error, SDX_ERROR_PARMETER_DIRECTX_NULLPTR) << "Expected directX null ptr error";

	// Valid 
	error = mgr.Initialise(&directX);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on valid initialise";
}

TEST_F(SDXShaderMGRUTest, LoadShader)
{
	// Create directX
	SDXDirectX directX;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();
	SDXErrorId error = directX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on directX initialise";

	SDXShaderMGR mgr;

	// Try loading without initialising
	error = mgr.LoadShader(validVertexShader, validPixelShader, validDesc,
		ARRAYSIZE(validDesc), "Test");
	EXPECT_EQ(error, SDX_ERROR_SHADERMGR_DIRECTX_NOTSET) << "Expected directX not set error";

	// Init shader manager
	error = mgr.Initialise(&directX);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Failed to initialise shader manager";

	std::string validShader = "validShader";
	error = mgr.LoadShader(validVertexShader, validPixelShader, validDesc,
		ARRAYSIZE(validDesc), validShader);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on valid shader load";

	// Invalid load using null file name strings
	// Just testing not equal to no error.
	// Actual error code tests checked via SDXShaderLoaderUTest
	error = mgr.LoadShader("", "", nullptr, 0, "");
	EXPECT_NE(error, SDX_ERROR_NONE) << "Expected an error on invalid load";
}

TEST_F(SDXShaderMGRUTest, BindConstant)
{
	// Create directX
	SDXDirectX directX;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();
	SDXErrorId error = directX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on directX initialise";

	// Try binding without initialise
	std::string bufferID = "TestBuffer";
	SDXShaderMGR mgr;
	CD3D11_BUFFER_DESC bufferDesc(
		sizeof(TestBuffer),
		D3D11_BIND_CONSTANT_BUFFER
	);
	error = mgr.BindConstant(bufferID, &bufferDesc);
	EXPECT_EQ(error, SDX_ERROR_SHADERMGR_DIRECTX_NOTSET) << "Expected directX not set error";

	error = mgr.Initialise(&directX);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on initialise";

	// NULL string
	error = mgr.BindConstant("", &bufferDesc);
	EXPECT_EQ(error, SDX_ERROR_SHADERMGR_NULL_ID) << "Expected null ID error";

	error = mgr.BindConstant(bufferID, nullptr);
	EXPECT_EQ(error, SDX_ERROR_SHADERMGR_BIND_CONSTANT_FAILED) << "Expected bind failure";

	// Valid check
	error = mgr.BindConstant(bufferID, &bufferDesc);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on valid bind";

	// Existing ID
	error = mgr.BindConstant(bufferID, &bufferDesc);
	EXPECT_EQ(error, SDX_ERROR_SHADERMGR_BIND_CONSTANT_ID_ALREADY_EXIST) << "Expected bind constant ID already exist error";
}

TEST_F(SDXShaderMGRUTest, GetTests)
{
	// Create directX
	SDXDirectX directX;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();
	SDXErrorId error = directX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on directX initialise";

	SDXShaderMGR mgr;

	// Init shader manager
	error = mgr.Initialise(&directX);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Failed to initialise shader manager";

	std::string validShader = "validShader";
	std::string validShader_2 = "validShader2";

	// Test non-existing shader get
	SShader* pResult = mgr.GetShader(0);
	EXPECT_EQ(pResult, nullptr) << "Expected nullptr on get shader with no shaders";

	pResult = mgr.GetShader(validShader);
	EXPECT_EQ(pResult, nullptr) << "Expected nullptr on get shader with no shader";

	// Load test shaders in
	error = mgr.LoadShader(validVertexShader, validPixelShader, validDesc,
		ARRAYSIZE(validDesc), validShader);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on valid shader load";

	error = mgr.LoadShader(validVertexShader, validPixelShader, validDesc,
		ARRAYSIZE(validDesc), validShader_2);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on valid shader load";

	// Valid get shader tests
	pResult = mgr.GetShader(0);
	EXPECT_NE(pResult, nullptr) << "Result should not be nullptr";

	pResult = mgr.GetShader(1);
	EXPECT_NE(pResult, nullptr) << "Result should not be nullptr";

	pResult = mgr.GetShader(validShader);
	EXPECT_NE(pResult, nullptr) << "Result should not be nullptr";
	
	pResult = mgr.GetShader(validShader_2);
	EXPECT_NE(pResult, nullptr) << "Result should not be nullptr";

	// Test bind constant test data
	std::string bufferID_1 = "TestBuffer1";
	std::string bufferID_2 = "TestBuffer2";

	CD3D11_BUFFER_DESC bufferDesc1(
		sizeof(TestBuffer),
		D3D11_BIND_CONSTANT_BUFFER
	);
	CD3D11_BUFFER_DESC bufferDesc2(
		sizeof(TestBuffer),
		D3D11_BIND_CONSTANT_BUFFER
	);

	// Invalid tests
	ComPtr<ID3D11Buffer> pReturn;
	pReturn = mgr.GetCBuffer(0);
	EXPECT_EQ(pReturn.Get(), nullptr) << "Expected nullptr on no constant buffers";

	pReturn = mgr.GetCBuffer(bufferID_1);
	EXPECT_EQ(pReturn.Get(), nullptr) << "Expected nullptr on no constant buffers";

	// Bind test constant buffers
	error = mgr.BindConstant(bufferID_1, &bufferDesc1);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on valid bind";

	error = mgr.BindConstant(bufferID_2, &bufferDesc2);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on valid bind";

	// Valid tests
	pReturn = mgr.GetCBuffer(0);
	EXPECT_NE(pReturn.Get(), nullptr) << "Unexpected nullptr on valid return";

	pReturn = mgr.GetCBuffer(1);;
	EXPECT_NE(pReturn.Get(), nullptr) << "Unexpected nullptr on valid return";

	pReturn = mgr.GetCBuffer(bufferID_1);
	EXPECT_NE(pReturn.Get(), nullptr) << "Unexpected nullptr on valid return";

	pReturn = mgr.GetCBuffer(bufferID_2);
	EXPECT_NE(pReturn.Get(), nullptr) << "Unexpected nullptr on valid return";
}