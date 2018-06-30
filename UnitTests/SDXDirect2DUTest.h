#pragma once

#include <gtest\gtest.h>
#include "SDXTestApp.h"
#include "..\SDXEngine\SDXDirect2D.h"

class SDXDirect2DUTest : public ::testing::Test
{
public:
	SDXDirect2DUTest();
	~SDXDirect2DUTest();

	void SetUp() override;
	void TearDown() override;

protected:
	SDXTestApp m_testApp;
	bool m_appInit;
};

