#include "pch.h"
#include "CShaderMGRUTest.h"

#include "..\SDXEngine\SDXDirectX.h"

CShaderMGRUTest::CShaderMGRUTest()
{
}

CShaderMGRUTest::~CShaderMGRUTest()
{
}

void CShaderMGRUTest::SetUp()
{
	std::string testTitle = "TestWindow";
	UINT width = 800;
	UINT height = 600;
	m_appInit = m_testApp.Initialise(testTitle, width, height);
}

void CShaderMGRUTest::TearDown()
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
TEST_F(CShaderMGRUTest, Initialise)
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

TEST_F(CShaderMGRUTest, LoadShader)
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
		ARRAYSIZE(validDesc), "validShader");
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on valid shader load";

	// Invalid load using null file name strings
	// Just testing not equal to no error.
	// Actual error code tests checked via SDXShaderLoaderUTest
	error = mgr.LoadShader("", "", nullptr, 0, "");
	EXPECT_NE(error, SDX_ERROR_NONE) << "Expected an error on invalid load";
}

TEST_F(CShaderMGRUTest, BindConstant)
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

TEST_F(CShaderMGRUTest, GetTests)
{
	// Create directX
	SDXDirectX directX;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();
	SDXErrorId error = directX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on directX initialise";
}