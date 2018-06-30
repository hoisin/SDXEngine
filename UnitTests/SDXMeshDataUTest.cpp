#include "pch.h"
#include "SDXMeshDataUTest.h"
#include "..\SDXEngine\SDXMeshData.h"

using namespace SDXEngine;

SDXMeshDataUTest::SDXMeshDataUTest()
{
}


SDXMeshDataUTest::~SDXMeshDataUTest()
{
}

void SDXMeshDataUTest::SetUp()
{
}

void SDXMeshDataUTest::TearDown()
{
}

// --------------------------------------------------------------------------------
// Class fixture testing
// --------------------------------------------------------------------------------
TEST_F(SDXMeshDataUTest, ConstructorTest)
{
	SDXMeshData testData;

	// Check defaults
	EXPECT_EQ(testData.GetVertexData(), nullptr) << "Should not have any vertex data";
	EXPECT_EQ(testData.GetIndexData(), nullptr) << "Should not have any index data";
	EXPECT_EQ(testData.GetVertexCount(), 0) << "Unexpected count for default";
	EXPECT_EQ(testData.GetIndexCount(), 0) << "Unexpected count for default";
	EXPECT_EQ(testData.GetVertexType(), SDXVERTEX_TYPE_UNKNOWN) << "Unexpected vertex type on default";
}

TEST_F(SDXMeshDataUTest, VertexCreateTest)
{
	SDXMeshData testData;
	UINT vertexCount = 100;
	SDXVertexType testType = SDXVERTEX_TYPE_PC;

	// Invalid vertex type test
	EXPECT_EQ(testData.CreateVertexArray(SDXVERTEX_TYPE_UNKNOWN, vertexCount), SDX_ERROR_MESHDATA_UNKNOWN_VERTEXTYPE) << "Unexpected error on creating with unknown vertex type";
	EXPECT_EQ(testData.GetVertexData(), nullptr) << "Should be no data on invalid vertex type create";
	EXPECT_EQ(testData.GetVertexCount(), 0) << "Count should be 0 on invalid vertex type create";
	EXPECT_EQ(testData.GetVertexType(), SDXVERTEX_TYPE_UNKNOWN) << "Type should be unknown on invalid vertex create";

	// 0 Vertex Count test
	EXPECT_EQ(testData.CreateVertexArray(testType, 0), SDX_ERROR_MESHDATA_CREATE_VERTEXARRAY_COUNT_ZERO) << "Unexpected error on vertex array create with 0 vertex count";
	EXPECT_EQ(testData.GetVertexData(), nullptr) << "Should be not data on 0 vertex count create";
	EXPECT_EQ(testData.GetVertexCount(), 0) << "Count should be 0 on 0 vertex count create";
	EXPECT_EQ(testData.GetVertexType(), SDXVERTEX_TYPE_UNKNOWN) << "Type should be unknown on 0 vertex create";

	// Valid create
	EXPECT_EQ(testData.CreateVertexArray(testType, vertexCount), SDX_ERROR_NONE) << "Unexpected error on valid create";
	EXPECT_NE(testData.GetVertexData(), nullptr) << "Should contain data on valid create";
	EXPECT_EQ(testData.GetVertexCount(), vertexCount) << "Unexpected vertex count after valid create";
	EXPECT_EQ(testData.GetVertexType(), testType) << "Unexpected vertex type after valid create";

	// Attempt to create with existing vertex data
	EXPECT_EQ(testData.CreateVertexArray(testType, vertexCount), SDX_ERROR_MESHDATA_VERTEXARRAY_ALREADY_CREATED) << "Unexpected on creating vertex array with existing data";
	EXPECT_NE(testData.GetVertexData(), nullptr) << "Should still contain data";
	EXPECT_EQ(testData.GetVertexCount(), vertexCount) << "Should still be the same count";
	EXPECT_EQ(testData.GetVertexType(), testType) << "Should still be the same vertex type";
}