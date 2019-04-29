#include "pch.h"
#include "SDXDirectXUTest.h"
#include "..\SDXEngine\SDXErrors.h"

using namespace SDXEngine;

SDXDirectXUTest::SDXDirectXUTest() : m_appInit(false)
{
}


SDXDirectXUTest::~SDXDirectXUTest()
{
}

void SDXDirectXUTest::SetUp()
{
	std::string testTitle = "TestWindow";
	UINT width = 800;
	UINT height = 600;
	m_appInit = m_testApp.Initialise(testTitle, width, height);
}

void SDXDirectXUTest::TearDown()
{
	// Set to run and just quit
	m_testApp.Run();
}


// --------------------------------------------------------------------------------
// Class fixture testing
// --------------------------------------------------------------------------------
TEST_F(SDXDirectXUTest, Initialise)
{
	SDXDirectX directX;
	SDXErrorId error = SDX_ERROR_NONE;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();
	
	// Invalid tests
	SDXDirectXInfo wrongSetup;
	wrongSetup = validSetup;

	wrongSetup.clientWidth = 0;
	error = directX.Initialise(wrongSetup);
	EXPECT_EQ(error, SDX_ERROR_INVALIDDIRECTX_SETUPINFO) << "Expected invalid directX setupinfo error";

	wrongSetup = validSetup;
	wrongSetup.clientHeight = 0;
	error = directX.Initialise(wrongSetup);
	EXPECT_EQ(error, SDX_ERROR_INVALIDDIRECTX_SETUPINFO) << "Expected invalid directX setupinfo error";

	wrongSetup = validSetup;
	wrongSetup.hwnd = nullptr;
	error = directX.Initialise(wrongSetup);
	EXPECT_EQ(error, SDX_ERROR_INVALIDDIRECTX_SETUPINFO) << "Expected invalid directX setupinfo error";

	wrongSetup = validSetup;
	wrongSetup.refreshRate = 0;
	error = directX.Initialise(wrongSetup);
	EXPECT_EQ(error, SDX_ERROR_INVALIDDIRECTX_SETUPINFO) << "Expected invalid directX setupinfo error";

	// Valid test
	error = directX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error from valid setup info";

	// Try Reinitialsing
	error = directX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_DEVICE_ALREADY_EXIST) << "Expected device already exist error";
}

TEST_F(SDXDirectXUTest, ReInitWindowDependentResources)
{
	SDXDirectX directX;
	SDXErrorId error = SDX_ERROR_NONE;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();

	// Invalid tests

	// Reinit without first running intialise
	error = directX.ReInitWindowDependentResources(validSetup);
	EXPECT_EQ(error, SDX_ERROR_DEVICE_NOT_CREATED) << "Expected device not created error";

	// Initialise
	error = directX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error";

	// Invalid input
	SDXDirectXInfo wrongSetup;
	wrongSetup = validSetup;

	wrongSetup.clientWidth = 0;
	error = directX.ReInitWindowDependentResources(wrongSetup);
	EXPECT_EQ(error, SDX_ERROR_INVALIDDIRECTX_SETUPINFO) << "Expected invalid directX setupinfo error";

	wrongSetup = validSetup;
	wrongSetup.clientHeight = 0;
	error = directX.ReInitWindowDependentResources(wrongSetup);
	EXPECT_EQ(error, SDX_ERROR_INVALIDDIRECTX_SETUPINFO) << "Expected invalid directX setupinfo error";

	wrongSetup = validSetup;
	wrongSetup.hwnd = nullptr;
	error = directX.ReInitWindowDependentResources(wrongSetup);
	EXPECT_EQ(error, SDX_ERROR_INVALIDDIRECTX_SETUPINFO) << "Expected invalid directX setupinfo error";

	wrongSetup = validSetup;
	wrongSetup.refreshRate = 0;
	error = directX.ReInitWindowDependentResources(wrongSetup);
	EXPECT_EQ(error, SDX_ERROR_INVALIDDIRECTX_SETUPINFO) << "Expected invalid directX setupinfo error";

	// Valid test
	error = directX.ReInitWindowDependentResources(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error from valid setup info";

	// Run again
	error = directX.ReInitWindowDependentResources(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error from running Reinit again";
}

TEST_F(SDXDirectXUTest, SwapChainPresent)
{
	SDXDirectX testDirectX;
	SDXErrorId error = SDX_ERROR_NONE;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();
	validSetup.windowed = true;
	validSetup.refreshRate = 60;

	// Test running without initialising 
	error = testDirectX.SwapChainPresent();
	EXPECT_EQ(error, SDX_ERROR_SWAPCHAIN_NOT_CREATED) << "Expected swap chain not created error";

	// Initialise device and context
	error = testDirectX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when initialising DirectX";

	// Test swapchain present
	error = testDirectX.SwapChainPresent();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Valid swap chain present failed";

	// Run Reinit
	error = testDirectX.ReInitWindowDependentResources(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error running Reinit";

	// Test swapchain again
	error = testDirectX.SwapChainPresent();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error";
}


TEST_F(SDXDirectXUTest, GetDevice)
{
	SDXDirectX testDirectX;
	SDXErrorId error = SDX_ERROR_NONE;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();
	validSetup.windowed = true;
	validSetup.refreshRate = 60;

	// Test without creating 
	ID3D11Device* device = testDirectX.GetDevice().Get();
	EXPECT_EQ(device, nullptr) << "Expected to return nullptr when device not created";

	// Test after created
	error = testDirectX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on initialise DirectX";

	device = testDirectX.GetDevice().Get();
	EXPECT_NE(device, nullptr) << "Device ptr should not be nullptr after create";
}


TEST_F(SDXDirectXUTest, GetContext)
{
	SDXDirectX testDirectX;
	SDXErrorId error = SDX_ERROR_NONE;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();
	validSetup.windowed = true;
	validSetup.refreshRate = 60;

	// Test without creating
	ID3D11DeviceContext* context = testDirectX.GetContext().Get();
	EXPECT_EQ(context, nullptr) << "Expected to return nullptr when context not created";

	// Test after created
	error = testDirectX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on initialise DirectX";

	context = testDirectX.GetContext().Get();
	EXPECT_NE(context, nullptr) << "Context ptr should not be nullptr after create";
}


TEST_F(SDXDirectXUTest, GetAspectRatio)
{
	SDXDirectX testDirectX;
	SDXErrorId error = SDX_ERROR_NONE;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();
	validSetup.windowed = true;
	validSetup.refreshRate = 60;

	// Testing without setting anything
	//
	// Expected default width is 800 and height 600
	int testWidth = 800;		// Default in class c'tor
	int testHeight = 600;		// Default in class c'tor

	float expectedAspectRatio = (static_cast<float>(testWidth) / static_cast<float>(testHeight));
	float testRatio = testDirectX.GetAspectRatio();
	EXPECT_EQ(testRatio, expectedAspectRatio) << "Unexpected default aspect ratio";

	// Testing with set
	testWidth = 1600;
	testHeight = 1200;
	expectedAspectRatio = (static_cast<float>(testWidth) / static_cast<float>(testHeight));

	validSetup.clientWidth = testWidth;
	validSetup.clientHeight = testHeight;

	// Setup directX
	error = testDirectX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on DirectX initialise";

	// Check aspect ratio
	testRatio = testDirectX.GetAspectRatio();
	EXPECT_EQ(testRatio, expectedAspectRatio) << "Unexpected aspect ratio";
}


TEST_F(SDXDirectXUTest, GetRenderTargetView)
{
	SDXDirectX testDirectX;
	SDXErrorId error = SDX_ERROR_NONE;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();
	validSetup.windowed = true;
	validSetup.refreshRate = 60;

	// nullptr tests
	ID3D11RenderTargetView* target = testDirectX.GetRenderTargetView().Get();
	EXPECT_EQ(target, nullptr) << "Expected null when not created render target view";

	error = testDirectX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on DirectX initialise";

	// Test valid get
	target = testDirectX.GetRenderTargetView().Get();
	EXPECT_NE(target, nullptr) << "Failed on valid get";

	// Test with Re-init
	error = testDirectX.ReInitWindowDependentResources(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on reinit";

	target = testDirectX.GetRenderTargetView().Get();
	EXPECT_NE(target, nullptr) << "Failed on valid get";
}


TEST_F(SDXDirectXUTest, GetDepthStencilView)
{
	// nullptr tests
	SDXDirectX testDirectX;
	SDXErrorId error = SDX_ERROR_NONE;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();
	validSetup.windowed = true;
	validSetup.refreshRate = 60;

	ID3D11DepthStencilView* depthStencil = testDirectX.GetDepthStencilView().Get();
	EXPECT_EQ(depthStencil, nullptr) << "Expected nullptr when not created depth stencil view";

	error = testDirectX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on DirectX initialise";

	// Valid get test
	depthStencil = testDirectX.GetDepthStencilView().Get();
	EXPECT_NE(depthStencil, nullptr) << "Failed on valid get test";

	// Test with Re-init
	error = testDirectX.ReInitWindowDependentResources(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on reinit";

	depthStencil = testDirectX.GetDepthStencilView().Get();
	EXPECT_NE(depthStencil, nullptr) << "Failed on valid get test";
}


TEST_F(SDXDirectXUTest, GetSwapChain)
{
	// null check
	SDXDirectX testDirectX;
	SDXErrorId error = SDX_ERROR_NONE;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();
	validSetup.windowed = true;
	validSetup.refreshRate = 60;

	IDXGISwapChain* swapChain = testDirectX.GetSwapChain().Get();
	EXPECT_EQ(swapChain, nullptr) << "Expected nullptr when not created swap chain";

	error = testDirectX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on DirectX initialise";

	// Test get
	swapChain = testDirectX.GetSwapChain().Get();
	EXPECT_NE(swapChain, nullptr) << "Unexpected nullptr on swap chain get";

	// Test with Re-init
	error = testDirectX.ReInitWindowDependentResources(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on reinit";

	swapChain = testDirectX.GetSwapChain().Get();
	EXPECT_NE(swapChain, nullptr) << "Unexpected nullptr on swap chain get";
}


TEST_F(SDXDirectXUTest, GetClientAreaDimensions)
{
	SDXDirectX testDirectX;
	SDXErrorId error = SDX_ERROR_NONE;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();
	validSetup.windowed = true;
	validSetup.refreshRate = 60;

	// Default tests
	EXPECT_EQ(testDirectX.GetClientAreaWidth(), 800) << "Unexpected client area width";
	EXPECT_EQ(testDirectX.GetClientAreaHeight(), 600) << "Unexpected client area height";

	// Test case
	UINT width = 1024;
	UINT height = 764;
	validSetup.clientWidth = width;
	validSetup.clientHeight = height;

	error = testDirectX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on DirectX initialise";

	EXPECT_EQ(width, testDirectX.GetClientAreaWidth()) << "Unexpected returned client area width after set";
	EXPECT_EQ(height, testDirectX.GetClientAreaHeight()) << "Unexpected returned client area height after set";
}


TEST_F(SDXDirectXUTest, SetRasterTest)
{
	SDXDirectX testDirectX;
	SDXErrorId error = SDX_ERROR_NONE;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();
	validSetup.windowed = true;
	validSetup.refreshRate = 60;

	SDXRasterState state;

	// Test without device
	error = testDirectX.SetRasterState(state);
	EXPECT_EQ(error, SDX_ERROR_DEVICE_NOT_CREATED) << "Unexpected error on raster state set without device";

	// Valid call
	error = testDirectX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on DirectX initialise";

	error = testDirectX.SetRasterState(state);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on valid raster state set";
}


TEST_F(SDXDirectXUTest, SetDepthStencilState)
{
	SDXDirectX testDirectX;
	SDXErrorId error = SDX_ERROR_NONE;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();
	validSetup.windowed = true;
	validSetup.refreshRate = 60;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Try set without initialising
	error = testDirectX.SetDepthStencilState(depthStencilDesc);
	EXPECT_EQ(error, SDX_ERROR_DEVICE_NOT_CREATED) << "Expected device not created error";

	// Valid test
	error = testDirectX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on DirectX initialise";

	error = testDirectX.SetDepthStencilState(depthStencilDesc);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Failed to set valid depth stencil state";
}


TEST_F(SDXDirectXUTest, ShutDown)
{
	SDXDirectX testDirectX;
	SDXErrorId error = SDX_ERROR_NONE;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();
	validSetup.windowed = true;
	validSetup.refreshRate = 60;

	error = testDirectX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on DirectX initialise";

	testDirectX.ShutDown();

	error = testDirectX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on DirectX initialise";
}