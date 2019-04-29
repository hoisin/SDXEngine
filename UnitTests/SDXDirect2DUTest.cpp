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
	// Test nullptr init
	SDXDirect2D direct2D;
	SDXErrorId error = direct2D.Initialise(nullptr);
	EXPECT_EQ(error, SDX_ERROR_DIRECT2D_NULLPTR_SET) << "Expected error on null initialise";

	// Valid set 
	SDXDirectX directX;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();
	error = directX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on initialise";

	error = direct2D.Initialise(&directX);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on valid set";

	direct2D.ShutDown();
	directX.ShutDown();
}

TEST_F(SDXDirect2DUTest, RenderText)
{
	SDXDirect2D direct2D;
	UINT pX = 0;
	UINT pY = 0;
	std::string text = "TEST TEXT";

	// Test without creating device 
	SDXErrorId error = direct2D.RenderText(pX, pY, text);
	EXPECT_EQ(error, SDX_ERROR_DIRECT2D_NOT_INITIALISED) << "Unexpected error";

	error = direct2D.SetRenderText(text);
	EXPECT_EQ(error, SDX_ERROR_DIRECT2D_NOT_INITIALISED) << "Unexpected error";

	error = direct2D.RenderText(pX, pY);
	EXPECT_EQ(error, SDX_ERROR_DIRECT2D_NOT_INITIALISED) << "Unexpected error";

	// Setup DirectX/2D
	SDXDirectX testDirectX;
	SDXDirectXInfo validSetup;
	validSetup.clientWidth = 800;
	validSetup.clientHeight = 600;
	validSetup.hwnd = m_testApp.GetHwndTest();

	error = testDirectX.Initialise(validSetup);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexepected error on valid SDXDirectX device create";

	// Set SDXDirectX to SDXDirect2D and create device
	error = direct2D.Initialise(&testDirectX);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error on direct2D initialise";

	// Valid setup
	error = direct2D.RenderText(pX, pY, text);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error";

	error = direct2D.RenderText(pX, pY, text);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Unexpected error";

	direct2D.ShutDown();
	testDirectX.ShutDown();
} 