#pragma once

#include <gtest\gtest.h>
#include "SDXTestApp.h"
#include "..\SDXEngine\SDXVertexBuffer.h"

class SDXVertexBufferUTest : public ::testing::Test
{
public:
	SDXVertexBufferUTest();
	~SDXVertexBufferUTest();

protected:
	SDXTestApp m_testApp;
	bool m_appInit;
};

