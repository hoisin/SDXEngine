#pragma once

#include <gtest/gtest.h>

class CTimerUTest : public ::testing::Test 
{
public:
	CTimerUTest();
	~CTimerUTest();

	void SetUp() override;
	void TearDown() override;

protected:
	
};