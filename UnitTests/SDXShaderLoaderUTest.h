#pragma once

#include "gtest/gtest.h"
#include "SDXTestApp.h"
#include "..\SDXEngine\SDXShaderLoader.h"

class SDXShaderLoaderUTest : public ::testing::Test
{
public:
	SDXShaderLoaderUTest();
	~SDXShaderLoaderUTest();

	void SetUp() override;
	void TearDown() override;

protected:
	SDXTestApp m_testApp;
	bool m_appInit = false;
};