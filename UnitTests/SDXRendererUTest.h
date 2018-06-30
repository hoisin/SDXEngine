#pragma once

#include <gtest/gtest.h>
#include "SDXTestApp.h"
#include "..\SDXEngine\SDXRenderer.h"

class SDXRendererUTest : public ::testing::Test
{
public:
	SDXRendererUTest();
	~SDXRendererUTest();

	void SetUp() override;
	void TearDown() override;

protected:
	UINT m_width;
	UINT m_height;
	SDXTestApp m_testApp;
	bool m_appInit;
};

 