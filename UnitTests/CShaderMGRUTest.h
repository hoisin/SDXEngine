#pragma once

#include "gtest/gtest.h"
#include "SDXTestApp.h"
#include "..\SDXEngine\SDXShaderMGR.h"

class CShaderMGRUTest : public ::testing::Test
{
public:
	CShaderMGRUTest();
	~CShaderMGRUTest();

	void SetUp() override;
	void TearDown() override;

protected:
	SDXTestApp m_testApp;
	bool m_appInit = false;
};