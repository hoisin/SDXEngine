#include "pch.h"
#include "SDXRasterStateUTest.h"

#include "..\SDXEngine\SDXRasterState.h"

using namespace SDXEngine;


SDXRasterStateUTest::SDXRasterStateUTest()
{
}


SDXRasterStateUTest::~SDXRasterStateUTest()
{
}

void SDXRasterStateUTest::SetUp()
{
}

void SDXRasterStateUTest::TearDown()
{
}


//  Tests
TEST_F(SDXRasterStateUTest, GetSetTests)
{
	SDXRasterState testState;

	// Check defaults
	bool defWireFrame = false;
	bool defFrontCounterClockwise = false;
	bool defDepthClip = true;
	bool defScissor = false;
	bool defMultiSample = false;
	bool defAntitalisedLine = false;
	SDXCullMode defCullMode = SDX_CULL_BACK;
	float defDepthBiasClamp = 0.f;
	float defSlopeScaledDepthBias = 0.f;

	EXPECT_EQ(testState.IsWireFrame(), defWireFrame) << "Unexpected default";
	EXPECT_EQ(testState.IsFrontCounterClockwise(), defFrontCounterClockwise) << "Unexpected default";
	EXPECT_EQ(testState.IsDepthClip(), defDepthClip) << "Unexpected default";
	EXPECT_EQ(testState.IsScissor(), defScissor) << "Unexpected default";
	EXPECT_EQ(testState.IsMultiSample(), defMultiSample) << "Unxpected default";
	EXPECT_EQ(testState.IsAntialisedLine(), defAntitalisedLine) << "Unexpected default";
	EXPECT_EQ(testState.GetCullMode(), defCullMode) << "Unexpected default";
	EXPECT_EQ(testState.GetDepthBiasClamp(), defDepthBiasClamp) << "Unexpected default";
	EXPECT_EQ(testState.GetSlopeScaledDepthBias(), defSlopeScaledDepthBias) << "Unexpected default";

	// Test Set/Get
	bool wireFrame = true;
	bool frontCounterClockwise = true;
	bool depthClip = false;
	bool scissor = true;
	bool multiSample = true;
	bool antialisedLine = true;
	SDXCullMode cullMode = SDX_CULL_NONE;
	float depthBiasClamp = 1.0f;
	float slopeScaledDepthBias = 1.0f;

	testState.SetWireFrame(wireFrame);
	testState.SetFrontCounterClockwise(frontCounterClockwise);
	testState.SetDepthClip(depthClip); 
	testState.SetScissor(scissor);
	testState.SetMultiSample(multiSample);
	testState.SetAntialisedLine(antialisedLine);
	testState.SetCullMode(cullMode);
	testState.SetDepthBiasClamp(depthBiasClamp);
	testState.SetSlopeScaledDepthBias(slopeScaledDepthBias);

	EXPECT_EQ(testState.IsWireFrame(), wireFrame) << "Unexpected get after set";
	EXPECT_EQ(testState.IsFrontCounterClockwise(), frontCounterClockwise) << "Unexpected get after set";
	EXPECT_EQ(testState.IsDepthClip(), depthClip) << "Unexpected get after set";
	EXPECT_EQ(testState.IsScissor(), scissor) << "Unexpected get after set";
	EXPECT_EQ(testState.IsMultiSample(), multiSample) << "Unexpected get after set";
	EXPECT_EQ(testState.IsAntialisedLine(), antialisedLine) << "Unexpected get after set";
	EXPECT_EQ(testState.GetCullMode(), cullMode) << "Unexpected get after set";
	EXPECT_EQ(testState.GetDepthBiasClamp(), depthBiasClamp) << "Unexpected get after set";
	EXPECT_EQ(testState.GetSlopeScaledDepthBias(), slopeScaledDepthBias) << "Unexpected get after set";
}

