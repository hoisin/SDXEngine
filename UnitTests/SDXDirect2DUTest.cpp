#include "pch.h"
#include "SDXDirect2DUTest.h"
#include "..\SDXEngine\SDXErrors.h"


SDXDirect2DUTest::SDXDirect2DUTest()
{
}


SDXDirect2DUTest::~SDXDirect2DUTest()
{
}

void SDXDirect2DUTest::SetUp()
{
	std::string testTitle = "TestWindow";
	UINT width = 800;
	UINT height = 600;
	m_appInit = m_testApp.Initialise(testTitle, width, height);
}

void SDXDirect2DUTest::TearDown()
{
	// Set to run and just quit
	m_testApp.Run();
}

// --------------------------------------------------------------------------------
// Class fixture testing
// --------------------------------------------------------------------------------
TEST_F(SDXDirect2DUTest, SetupSDXDirectX)
{
	// Test set nullptr
	SDXDirect2D direct2D;
	SDXErrorId error = direct2D.SetSDXDirectX(nullptr);
	EXPECT_EQ(error, SDX_ERROR_DIRECT2D_NULLPTR_SET) << "Expected error on null set";

	// Valid set 
	SDXDirectX directX;
	error = direct2D.SetSDXDirectX(&directX);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on valid set";
}


TEST_F(SDXDirect2DUTest, CreateDevice)
{
	// Test without SDXDirectX
	SDXDirect2D direct2D;
	SDXErrorId error = direct2D.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_DIRECT2D_NOT_SET_DIRECTX) << "Expected SDXDirectX not set error";

	// Test without creating directX device
	SDXDirectX directX;
	direct2D.SetSDXDirectX(&directX);
	error = direct2D.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_DEVICE_NOT_CREATED) << "Expected directX device not created";

	// Test valid create
	directX.CreateDevice();
	error = direct2D.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error";

	// Test when already created
	error = direct2D.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_DIRECT2D_DEVICE_ALREADY_CREATED) << "Expected Direct2D device already created";
}


TEST_F(SDXDirect2DUTest, CreateBitmapRenderTarget)
{
	SDXDirect2D direct2D;

	// Test with no set directX
	SDXErrorId error = direct2D.CreateBitmapRenderTarget();
	EXPECT_EQ(error, SDX_ERROR_DIRECT2D_NOT_SET_DIRECTX) << "Unexpected error for no SDX directX set";

	SDXDirectX testDirectX;
	error = direct2D.SetSDXDirectX(&testDirectX);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error setting directX";

	// Test with no direct2D device
	error = direct2D.CreateBitmapRenderTarget();
	EXPECT_EQ(error, SDX_ERROR_DIRECT2D_DEVICE_NOT_CREATED) << "Unexpected error when no direct2D context";

	// Setup SDXDirectX and direct2D
	error = testDirectX.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on creating SDXDirectX";
	error = direct2D.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on creating direct2d";

	// Test with no swap chain
	error = direct2D.CreateBitmapRenderTarget();
	EXPECT_EQ(error, SDX_ERROR_SWAPCHAIN_NOT_CREATED) << "Unexpected error";

	// Create swap chain
	UINT clientWidth = 800;
	UINT clientHeight = 600;
	UINT refreshRate = 60;
	bool bWindow = true;
	bool useMsaa = false;
	testDirectX.SetOutputWindow(m_testApp.GetHwndTest());
	testDirectX.SetClientArea(clientWidth, clientHeight);
	testDirectX.SetRefreshRate(refreshRate);
	testDirectX.SetWindowed(bWindow);
	testDirectX.SetUse4XMSAA(useMsaa);
	error = testDirectX.CreateSwapChain();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when attempting to create swapchain";

	// Test No error
	error = direct2D.CreateBitmapRenderTarget();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on valid create";

	// Test already created?
	error = direct2D.CreateBitmapRenderTarget();
	EXPECT_EQ(error, SDX_ERROR_DIRECT2D_BITMAPRENDERTARGET_ALREADY_CREATED) << "Unexpected error";
}


TEST_F(SDXDirect2DUTest, InitialiseTextFormats)
{
	SDXDirect2D direct2D;

	// Initialise without direct 2D device/context
	SDXErrorId error = direct2D.InitialiseTextFormats();
	EXPECT_EQ(error, SDX_ERROR_DIRECT2D_DEVICE_NOT_CREATED) << "Unexpected error";

	// Setup SDXDirectX
	SDXDirectX testDirectX;
	error = testDirectX.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on creating SDXDirectX";
	// Setup SDXDirect2D
	direct2D.SetSDXDirectX(&testDirectX);
	error = direct2D.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on creating direct2d";

	// Valid initialise
	error = direct2D.InitialiseTextFormats();
	EXPECT_EQ(error, SDX_ERROR_NONE);
}


TEST_F(SDXDirect2DUTest, RenderText)
{
	SDXDirect2D direct2D;
	UINT pX = 0;
	UINT pY = 0;
	std::string text = "TEST TEXT";

	// Test without creating device 
	SDXErrorId error = direct2D.RenderText(pX, pY, text);
	EXPECT_EQ(error, SDX_ERROR_DIRECT2D_DEVICE_NOT_CREATED) << "Unexpected error";

	// Setup DirectX/2D
	SDXDirectX testDirectX;
	error = testDirectX.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Exepected error on valid SDXDirectX device create";

	// create the swap chain
	UINT clientWidth = 800;
	UINT clientHeight = 600;
	UINT refreshRate = 60;
	bool bWindow = true;
	bool useMsaa = false;
	testDirectX.SetOutputWindow(m_testApp.GetHwndTest());
	testDirectX.SetClientArea(clientWidth, clientHeight);
	testDirectX.SetRefreshRate(refreshRate);
	testDirectX.SetWindowed(bWindow);
	testDirectX.SetUse4XMSAA(useMsaa);
	error = testDirectX.CreateSwapChain();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when attempting to create swapchain";
	
	// Attempt to bind OM without creating render target
	error = testDirectX.BindOutputMerger();
	EXPECT_EQ(error, SDX_ERROR_RENDERTARGETVIEW_NOT_CREATED) << "Unexpected error when running bind OM without render target view";

	// Create render target view
	error = testDirectX.CreateRenderTargetView();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error creating render target view";

	// Attempt to bind OM without creating depth/stencil view
	error = testDirectX.BindOutputMerger();
	EXPECT_EQ(error, SDX_ERROR_DEPTHSTENCIL_NOT_CREATED) << "Unexpected error when running bind OM without depth/stencil view";

	// Create depth/stencil
	error = testDirectX.CreateDepthStencilBufferView();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when creating depth/stencil buffer/view";

	// Valid test call for bind OM
	error = testDirectX.BindOutputMerger();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when running valid bind OM";

	// Set SDXDirectX to SDXDirect2D and create device
	direct2D.SetSDXDirectX(&testDirectX);
	direct2D.CreateDevice();
	direct2D.CreateBitmapRenderTarget();

	// Test without text format initialise
	error = direct2D.RenderText(pX, pY, text);
	EXPECT_EQ(error, SDX_ERROR_DIRECT2D_TEXTFORMAT_NOT_SETUP) << "Unexpected error";

	// Valid test
	error = direct2D.InitialiseTextFormats();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error";

	error = direct2D.RenderText(pX, pY, text);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error";
} 