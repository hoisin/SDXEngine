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
TEST_F(SDXDirectXUTest, CreateDevice)
{
	SDXDirectX directX;
	SDXErrorId error = SDX_ERROR_NONE;

	// Create test
	error = directX.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when creating device";

	// Create again
	error = directX.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_DEVICE_ALREADY_EXIST) << "Unexpected error when attempting to create a device when already one existing";

	// Shut down and attempt to re-create
	directX.ShutDown();
	error = directX.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when re-creating device after shutdown";

	directX.ShutDown();
}

TEST_F(SDXDirectXUTest, CreateSwapChain)
{
	// Setup directX device
	SDXDirectX testDirectX;
	SDXErrorId error = testDirectX.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when creating device";

	// Create without setting the output window
	error = testDirectX.CreateSwapChain();
	EXPECT_EQ(error, SDX_ERROR_SWAPCHAIN_NO_SET_OUTPUTWINDOW) << "Unexpected error when creating swapchain without setting output window";

	UINT clientWidth = 800;
	UINT clientHeight = 600;
	UINT refreshRate = 60;
	bool bWindow = true;
	bool useMsaa = false;

	// Create after setup
	testDirectX.SetOutputWindow(m_testApp.GetHwndTest());
	testDirectX.SetClientArea(clientWidth, clientHeight);
	testDirectX.SetRefreshRate(refreshRate);
	testDirectX.SetWindowed(bWindow);
	testDirectX.SetUse4XMSAA(useMsaa);
	error = testDirectX.CreateSwapChain();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when attempting to create swapchain";

	// Create again
	error = testDirectX.CreateSwapChain();
	EXPECT_EQ(error, SDX_ERROR_SWAPCHAIN_ALREADY_EXIST) << "Unexpected error when attempting to create swapchain when one already exist";

	// Shutdown and recreate swapchain
	testDirectX.ShutDown();
	error = testDirectX.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when re-creating device after shutdown";
	testDirectX.SetOutputWindow(m_testApp.GetHwndTest());
	testDirectX.SetClientArea(clientWidth, clientHeight);
	testDirectX.SetRefreshRate(refreshRate);
	testDirectX.SetWindowed(bWindow);
	testDirectX.SetUse4XMSAA(useMsaa);
	error = testDirectX.CreateSwapChain();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when attempting to create swapchain";

	// Free resources
	testDirectX.ShutDown();

	// Attempt to create without creating device
	SDXDirectX noDevice;
	error = noDevice.CreateSwapChain();
	EXPECT_EQ(error, SDX_ERROR_DEVICE_NOT_CREATED) << "Unexpected error when attempting to create swapchain without device";
}


TEST_F(SDXDirectXUTest, CheckMSAAQuality)
{
	// Setup directX device
	SDXDirectX testDirectX;
	SDXErrorId error = testDirectX.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when creating device";

	// Test MSAA quality check
	UINT quality = 0;
	error = testDirectX.Check4XMSAAQuality(quality);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error querying 4X MSAA quality";
	EXPECT_NE(quality, 0) << "4X MSAA should not be 0";

	// Free resources
	testDirectX.ShutDown();

	// Attempt to query without device
	SDXDirectX noDevice;
	quality = 0;
	error = noDevice.Check4XMSAAQuality(quality);
	EXPECT_EQ(error, SDX_ERROR_DEVICE_NOT_CREATED) << "Unexpected error querying 4x MSAA with no device created";
	EXPECT_EQ(quality, 0) << "Quality should be 0 when checking 4X MSAA without creating a device";
}

TEST_F(SDXDirectXUTest, CreateTargetRenderView)
{
	// Setup directX device
	SDXDirectX testDirectX;
	SDXErrorId error = testDirectX.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when creating device";

	UINT clientWidth = 800;
	UINT clientHeight = 600;
	UINT refreshRate = 60;
	bool bWindow = true;
	bool useMsaa = false;

	// Setup swap chain
	testDirectX.SetOutputWindow(m_testApp.GetHwndTest());
	testDirectX.SetClientArea(clientWidth, clientHeight);
	testDirectX.SetRefreshRate(refreshRate);
	testDirectX.SetWindowed(bWindow);
	testDirectX.SetUse4XMSAA(useMsaa);
	error = testDirectX.CreateSwapChain();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when attempting to create swapchain";

	// Create render target view
	error = testDirectX.CreateRenderTargetView();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error creating render target view";

	// Attempt to create again
	error = testDirectX.CreateRenderTargetView();
	EXPECT_EQ(error, SDX_ERROR_RENDERTARGETVIEW_ALREADY_EXIST) << "Unexpected error when attempting to create render target view when one already exist";
	
	// Attempt to re-create after shutdown
	testDirectX.ShutDown();
	error = testDirectX.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when creating device";

	testDirectX.SetOutputWindow(m_testApp.GetHwndTest());
	testDirectX.SetClientArea(clientWidth, clientHeight);
	testDirectX.SetRefreshRate(refreshRate);
	testDirectX.SetWindowed(bWindow);
	testDirectX.SetUse4XMSAA(useMsaa);
	error = testDirectX.CreateSwapChain();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when attempting to create swapchain";

	error = testDirectX.CreateRenderTargetView();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error creating render target view";

	// Free resource
	testDirectX.ShutDown();

	// Attempt to create without device
	SDXDirectX noDevice;
	error = noDevice.CreateRenderTargetView();
	EXPECT_EQ(error, SDX_ERROR_SWAPCHAIN_NOT_CREATED) << "Unexpected error when attempting to create render target view without device/swap chain";
}

TEST_F(SDXDirectXUTest, CreateDepthStencilBufferView)
{
	// Setup directX device
	SDXDirectX testDirectX;
	SDXErrorId error = testDirectX.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when creating device";

	// Valid create test
	error = testDirectX.CreateDepthStencilBufferView();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when creating depth/stencil buffer/view";

	// Attempt to create again
	error = testDirectX.CreateDepthStencilBufferView();
	EXPECT_EQ(error, SDX_ERROR_DEPTHSTENCIL_ALREADY_EXIST) << "Unexpected error when attetmping to create depth/stencil buffer/view with one already created";

	// Free resource
	testDirectX.ShutDown();

	// Attempt create with no device
	SDXDirectX noDevice;
	error = noDevice.CreateDepthStencilBufferView();
	EXPECT_EQ(error, SDX_ERROR_DEVICE_NOT_CREATED) << "Unexpected error when attempting to create depth/stencil buffer/view without device";
}

TEST_F(SDXDirectXUTest, BindOutputMerger)
{
	// Setup directX device
	SDXDirectX testDirectX;

	// Attempt to bind OM without device
	SDXErrorId error = testDirectX.BindOutputMerger();
	EXPECT_EQ(error, SDX_ERROR_DEVICE_NOT_CREATED) << "Unexpected error when running bind OM without device";

	// Create device
	error = testDirectX.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when creating device";

	// Don't need to test after device create.
	// Bind OM not swap chain dependent

	UINT clientWidth = 800;
	UINT clientHeight = 600;
	UINT refreshRate = 60;
	bool bWindow = true;
	bool useMsaa = false;

	// Setup swap chain
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
}


TEST_F(SDXDirectXUTest, SwapChainPresent)
{
	SDXDirectX testDirectX;

	// Attempt to present without swap chain
	SDXErrorId error = testDirectX.SwapChainPresent();
	EXPECT_EQ(error, SDX_ERROR_SWAPCHAIN_NOT_CREATED) << "Unexpected error on swap chain present without swap chain";

	UINT clientWidth = 800;
	UINT clientHeight = 600;
	UINT refreshRate = 60;
	bool bWindow = true;
	bool useMsaa = false;

	// Initialise device and context
	error = testDirectX.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when creating device";

	// Create after setup
	testDirectX.SetOutputWindow(m_testApp.GetHwndTest());
	testDirectX.SetClientArea(clientWidth, clientHeight);
	testDirectX.SetRefreshRate(refreshRate);
	testDirectX.SetWindowed(bWindow);
	testDirectX.SetUse4XMSAA(useMsaa);
	error = testDirectX.CreateSwapChain();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when attempting to create swapchain";

	// Test swapchain present
	error = testDirectX.SwapChainPresent();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Valid swap chain present failed";
}


TEST_F(SDXDirectXUTest, GetDevice)
{
	SDXDirectX testDirectX;
	// Test without creating 
	ID3D11Device* device = testDirectX.GetDevice();
	EXPECT_EQ(device, nullptr) << "Expected to return nullptr when device not created";

	// Test after created
	testDirectX.CreateDevice();
	device = testDirectX.GetDevice();
	EXPECT_NE(device, nullptr) << "Device ptr should not be nullptr after create";

	// test on shutdown
	testDirectX.ShutDown();
	device = testDirectX.GetDevice();
	EXPECT_EQ(device, nullptr) << "Expected nullptr after shutdown";
}


TEST_F(SDXDirectXUTest, GetContext)
{
	SDXDirectX testDirectX;
	// Test without creating
	ID3D11DeviceContext* context = testDirectX.GetContext();
	EXPECT_EQ(context, nullptr) << "Expected to return nullptr when context not created";

	// Test after created
	testDirectX.CreateDevice();
	context = testDirectX.GetContext();
	EXPECT_NE(context, nullptr) << "Context ptr should not be nullptr after create";

	// Test on shutdown
	testDirectX.ShutDown();
	context = testDirectX.GetContext();
	EXPECT_EQ(context, nullptr) << "Expected nullptr after shutdown";
}


TEST_F(SDXDirectXUTest, GetAspectRatio)
{
	SDXDirectX testDirectX;

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
	// Set directX
	testDirectX.SetClientArea(testWidth, testHeight);
	testRatio = testDirectX.GetAspectRatio();
	EXPECT_EQ(testRatio, expectedAspectRatio);

}


TEST_F(SDXDirectXUTest, GetRenderTargetView)
{
	// nullptr tests
	SDXDirectX testDirectX;
	ID3D11RenderTargetView* target = testDirectX.GetRenderTargetView();
	EXPECT_EQ(target, nullptr) << "Expected null when not created render target view";

	SDXErrorId error = testDirectX.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when creating device";

	UINT clientWidth = 800;
	UINT clientHeight = 600;
	UINT refreshRate = 60;
	bool bWindow = true;
	bool useMsaa = false;

	// Setup swap chain
	testDirectX.SetOutputWindow(m_testApp.GetHwndTest());
	testDirectX.SetClientArea(clientWidth, clientHeight);
	testDirectX.SetRefreshRate(refreshRate);
	testDirectX.SetWindowed(bWindow);
	testDirectX.SetUse4XMSAA(useMsaa);
	error = testDirectX.CreateSwapChain();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when attempting to create swapchain";

	// Create render target view
	error = testDirectX.CreateRenderTargetView();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error creating render target view";

	// Test valid get
	target = testDirectX.GetRenderTargetView();
	EXPECT_NE(target, nullptr) << "Failed on valid get";
}


TEST_F(SDXDirectXUTest, GetDepthStencilView)
{
	// nullptr tests
	SDXDirectX testDirectX;
	ID3D11DepthStencilView* depthStencil = testDirectX.GetDepthStencilView();
	EXPECT_EQ(depthStencil, nullptr) << "Expected nullptr when not created depth stencil view";

	SDXErrorId error = testDirectX.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when creating device";

	// Create depth stencil view
	error = testDirectX.CreateDepthStencilBufferView();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when creating depth/stencil buffer/view";

	// Valid get test
	depthStencil = testDirectX.GetDepthStencilView();
	EXPECT_NE(depthStencil, nullptr) << "Failed on valid get test";
}


TEST_F(SDXDirectXUTest, GetSwapChain)
{
	// null check
	SDXDirectX testDirectX;
	IDXGISwapChain* swapChain = testDirectX.GetSwapChain();
	EXPECT_EQ(swapChain, nullptr) << "Expected nullptr when not created swap chain";

	SDXErrorId  error = testDirectX.CreateDevice();
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error when creating device";

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

	// Test get
	swapChain = testDirectX.GetSwapChain();
	EXPECT_NE(swapChain, nullptr) << "Unexpected nullptr on swap chain get";
}


TEST_F(SDXDirectXUTest, GetClientAreaDimensions)
{
	SDXDirectX testDirectX;

	// Default tests
	EXPECT_EQ(testDirectX.GetClientAreaWidth(), 800) << "Unexpected client area width";
	EXPECT_EQ(testDirectX.GetClientAreaHeight(), 600) << "Unexpected client area height";

	// Test case
	UINT width = 1024;
	UINT height = 764;
	testDirectX.SetClientArea(width, height);
	EXPECT_EQ(width, testDirectX.GetClientAreaWidth()) << "Unexpected returned client area width after set";
	EXPECT_EQ(height, testDirectX.GetClientAreaHeight()) << "Unexpected returned client area height after set";
}