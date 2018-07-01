#include "pch.h"
#include "SDXDirectXShaderCompilerUTest.h"
#include <d3d11.h>
#include "..\SDXEngine\SDXDirectXShaderCompiler.h"

using namespace SDXEngine;

SDXDirectXShaderCompilerUTest::SDXDirectXShaderCompilerUTest()
{
}

SDXDirectXShaderCompilerUTest::~SDXDirectXShaderCompilerUTest()
{
}

void SDXDirectXShaderCompilerUTest::SetUp()
{
}

void SDXDirectXShaderCompilerUTest::TearDown()
{
}

std::string gValidVertexShader = "..\\UTestData\\Shaders\\valid.vs";
std::string gValidPixelShader = "..\\UTestData\\Shaders\\valid.ps";
std::string gInvalidVertexShader = "..\\UTestData\\Shaders\\invalid.vs";
std::string gInvalidPixelShader = "..\\UTestData\\Shaders\\invalid.ps";
std::string gEntryPoint = "Main";
std::string gVsProfile = "vs_5_0";
std::string gPsProfile = "ps_5_0";


// --------------------------------------------------------------------------------
// Class fixture testing
// --------------------------------------------------------------------------------
TEST_F(SDXDirectXShaderCompilerUTest, Compile)
{
	SDXDirectXShaderCompiler compiler;
	
	// Invalid arg tests
	SDXErrorId error = compiler.CompileShader("", "", "", nullptr);
	EXPECT_EQ(error, SDX_ERROR_DXCOMPILER_COMPILE_INVALID_ARGS) << "Expected fail on invalid args";
	error = compiler.CompileShader(gValidVertexShader, "", "", nullptr);
	EXPECT_EQ(error, SDX_ERROR_DXCOMPILER_COMPILE_INVALID_ARGS) << "Expected fail on invalid args";
	error = compiler.CompileShader(gValidVertexShader, gEntryPoint, "", nullptr);
	EXPECT_EQ(error, SDX_ERROR_DXCOMPILER_COMPILE_INVALID_ARGS) << "Expected fail on invalid args";
	error = compiler.CompileShader(gValidVertexShader, gEntryPoint, gVsProfile, nullptr);
	EXPECT_EQ(error, SDX_ERROR_DXCOMPILER_COMPILE_INVALID_ARGS) << "Expected fail on invalid args";

	ID3DBlob* testShader = nullptr;

	// Test compiling valid vertex shader
	error = compiler.CompileShader(gValidVertexShader, gEntryPoint, gVsProfile, &testShader);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Valid vertex shader compile fail";
	if (testShader)
	{
		testShader->Release();
		testShader = nullptr;
	}

	// Test compiling valid pixel shader
	error = compiler.CompileShader(gValidPixelShader, gEntryPoint, gPsProfile, &testShader);
	EXPECT_EQ(error, SDX_ERROR_NONE) << "Valid pixel shader compile fail";
	if (testShader)
	{
		testShader->Release();
		testShader = nullptr;
	}

	// Test compiling invalid vertex shader
	error = compiler.CompileShader(gInvalidVertexShader, gEntryPoint, gVsProfile, &testShader);
	EXPECT_EQ(error, SDX_ERROR_DXCOMPILER_COMPILE_FAILED) << "Expected error on invalid vertex shader compile";
	if (testShader)
	{
		testShader->Release();
		testShader = nullptr;
	}

	// Test compiling invalid pixel shader
	error = compiler.CompileShader(gInvalidPixelShader, gEntryPoint, gPsProfile, &testShader);
	EXPECT_EQ(error, SDX_ERROR_DXCOMPILER_COMPILE_FAILED) << "Expected error on invalid vertex shader compile";
	if (testShader)
	{
		testShader->Release();
		testShader = nullptr;
	}
}