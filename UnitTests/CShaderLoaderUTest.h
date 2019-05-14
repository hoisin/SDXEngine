#pragma once

#include "gtest/gtest.h"
#include "SDXTestApp.h"
#include "..\SDXEngine\SDXShaderLoader.h"

class CShaderLoaderUTest : public ::testing::Test
{
public:
	CShaderLoaderUTest();
	~CShaderLoaderUTest();

	void SetUp() override;
	void TearDown() override;

protected:
	SDXTestApp m_testApp;
	bool m_appInit = false;
};