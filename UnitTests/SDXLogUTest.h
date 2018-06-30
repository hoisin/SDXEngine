#pragma once

#include <gtest/gtest.h>

class SDXLogUTest : public ::testing::Test
{
public:
	SDXLogUTest();
	~SDXLogUTest();

	void SetUp() override;
	void TearDown()  override;

protected:
	std::string m_logFileName;
};

