#include "pch.h"
#include "SDXRendererUTest.h"
#include "..\SDXEngine\SDXErrors.h"

using namespace SDXEngine;

SDXRendererUTest::SDXRendererUTest() : m_width(0), m_height(0)
{
}


SDXRendererUTest::~SDXRendererUTest()
{
}

void SDXRendererUTest::SetUp()
{
	std::string testTitle = "TestWindow";
	m_width = 800;
	m_height = 600;
	m_appInit = m_testApp.Initialise(testTitle, m_width, m_height);
}

void SDXRendererUTest::TearDown()
{
	// Set to run and just quit
	m_testApp.Run();
}


// --------------------------------------------------------------------------------
// Class fixture testing
// --------------------------------------------------------------------------------
TEST_F(SDXRendererUTest, Initialise)
{
	SDXDirectXInfo info;
	SDXRenderer renderer;

	// Blank info test
	SDXErrorId error = renderer.Initialise(info);
	EXPECT_EQ(error, SDXErrorId::SDX_ERROR_RENDERER_CLIENTAREA_NOT_SET) << "Unexpected error for initialise without client area set";

	info.clientWidth = m_width;
	info.clientHeight = m_height;

	// Test with setting only client area
	error = renderer.Initialise(info);
	EXPECT_EQ(error, SDXErrorId::SDX_ERROR_RENDERER_OUTPUTWINDOW_NOT_SET) << "Unexpected error for initialise without output window set";

	info.hwnd = m_testApp.GetHwndTest();

	// Valid test
	error = renderer.Initialise(info);
	EXPECT_EQ(error, SDXErrorId::SDX_ERROR_NONE);
}
