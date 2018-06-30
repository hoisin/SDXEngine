#pragma once

#include <gtest\gtest.h>
#include "SDXTestApp.h"
#include "..\SDXEngine\SDXDirectX.h"

class SDXDirectXUTest : public ::testing::Test
{
public:
	SDXDirectXUTest();
	~SDXDirectXUTest();

	void SetUp() override;
	void TearDown() override;
	
protected:
	SDXTestApp m_testApp;
	bool m_appInit;
};

