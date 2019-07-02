#pragma once

#include "gtest/gtest.h"
#include "SDXTestApp.h"
#include "..\SDXEngine\SDXShaderMGR.h"

class SDXShaderMGRUTest : public ::testing::Test
{
public:
	SDXShaderMGRUTest();
	~SDXShaderMGRUTest();

	void SetUp() override;
	void TearDown() override;

protected:
	SDXTestApp m_testApp;
	bool m_appInit = false;
};