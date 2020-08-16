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
	EXPECT_EQ(testData.GetMaterialID(), "") << "Unexpected material ID on default";
	EXPECT_EQ(testData.GetMinVertexPos().x, 0) << "Unexpected min position X on default";
	EXPECT_EQ(testData.GetMinVertexPos().y, 0) << "Unexpected min position y on default";
	EXPECT_EQ(testData.GetMinVertexPos().z, 0) << "Unexpected min position z on default";
	EXPECT_EQ(testData.GetMaxVertexPos().x, 0) << "Unexpected max position x on default";
	EXPECT_EQ(testData.GetMaxVertexPos().y, 0) << "Unexpected max position y on default";
	EXPECT_EQ(testData.GetMaxVertexPos().z, 0) << "Unexpected max position z on default";
	
}

TEST_F(SDXMeshDataUTest, VertexCreateTest)
{
	SDXMeshData testData;
	UINT vertexCount = 100;
	SDXVertexType testType = SDXVERTEX_TYPE_PC;

	// Invalid vertex type test
	EXPECT_EQ(testData.CreateVertexArray(SDXVERTEX_TYPE_UNKNOWN, vertexCount), SDXErrorId::SDX_ERROR_MESHDATA_UNKNOWN_VERTEXTYPE) << "Unexpected error on creating with unknown vertex type";
	EXPECT_EQ(testData.GetVertexData(), nullptr) << "Should be no data on invalid vertex type create";
	EXPECT_EQ(testData.GetVertexCount(), 0) << "Count should be 0 on invalid vertex type create";
	EXPECT_EQ(testData.GetVertexType(), SDXVERTEX_TYPE_UNKNOWN) << "Type should be unknown on invalid vertex create";

	// 0 Vertex Count test
	EXPECT_EQ(testData.CreateVertexArray(testType, 0), SDXErrorId::SDX_ERROR_MESHDATA_CREATE_VERTEXARRAY_COUNT_ZERO) << "Unexpected error on vertex array create with 0 vertex count";
	EXPECT_EQ(testData.GetVertexData(), nullptr) << "Should be not data on 0 vertex count create";
	EXPECT_EQ(testData.GetVertexCount(), 0) << "Count should be 0 on 0 vertex count create";
	EXPECT_EQ(testData.GetVertexType(), SDXVERTEX_TYPE_UNKNOWN) << "Type should be unknown on 0 vertex create";

	// Valid create
	EXPECT_EQ(testData.CreateVertexArray(testType, vertexCount), SDXErrorId::SDX_ERROR_NONE) << "Unexpected error on valid create";
	EXPECT_NE(testData.GetVertexData(), nullptr) << "Should contain data on valid create";
	EXPECT_EQ(testData.GetVertexCount(), vertexCount) << "Unexpected vertex count after valid create";
	EXPECT_EQ(testData.GetVertexType(), testType) << "Unexpected vertex type after valid create";

	// Attempt to create with existing vertex data
	EXPECT_EQ(testData.CreateVertexArray(testType, vertexCount), SDXErrorId::SDX_ERROR_MESHDATA_VERTEXARRAY_ALREADY_CREATED) << "Unexpected on creating vertex array with existing data";
	EXPECT_NE(testData.GetVertexData(), nullptr) << "Should still contain data";
	EXPECT_EQ(testData.GetVertexCount(), vertexCount) << "Should still be the same count";
	EXPECT_EQ(testData.GetVertexType(), testType) << "Should still be the same vertex type";
}

TEST_F(SDXMeshDataUTest, SetGetTests)
{
	SDXMeshData testData;
	std::string materialID = "testID";
	XMFLOAT3 minPos(-1, -1, -1);
	XMFLOAT3 maxPos(1, 1, 1);

	// Defaults already test in ConstructorTest

	// Test set/get
	testData.SetMaterialID(materialID);
	testData.SetMinVertexPos(minPos);
	testData.SetMaxVertexPos(maxPos);

	EXPECT_EQ(testData.GetMaterialID(), materialID) << "Unexpected material ID returned";
	EXPECT_EQ(testData.GetMinVertexPos().x, minPos.x) << "Unexpected min pos x returned";
	EXPECT_EQ(testData.GetMinVertexPos().y, minPos.y) << "Unexpected min pos y returned";
	EXPECT_EQ(testData.GetMinVertexPos().z, minPos.z) << "Unexpected min pos z returned";
	EXPECT_EQ(testData.GetMaxVertexPos().x, maxPos.x) << "Unexpected max pos x returned";
	EXPECT_EQ(testData.GetMaxVertexPos().y, maxPos.y) << "Unexpected max pos y returned";
	EXPECT_EQ(testData.GetMaxVertexPos().z, maxPos.z) << "Unexpected max pos z returned";
}

TEST_F(SDXMeshDataUTest, IndexCreateTest)
{
	SDXMeshData testData;
	UINT indexCount = 100;

	// 0 index 
	EXPECT_EQ(testData.CreateIndexArray(0), SDXErrorId::SDX_ERROR_MESHDATA_CREATE_INDEXARRAY_COUNT_ZERO) << "Unexpected error for 0 index count creation";
	EXPECT_EQ(testData.GetIndexData(), nullptr) << "Should not be any data on create fail";
	EXPECT_EQ(testData.GetIndexCount(), 0) << "Index count should be 0 on fail create";

	// Valid create
	EXPECT_EQ(testData.CreateIndexArray(indexCount), SDXErrorId::SDX_ERROR_NONE) << "Unexpected error on valid create";
	EXPECT_NE(testData.GetIndexData(), nullptr) << "Should be data after valid create";
	EXPECT_EQ(testData.GetIndexCount(), indexCount) << "Index count does not match create";

	// Create on existing data
	EXPECT_EQ(testData.CreateIndexArray(indexCount), SDXErrorId::SDX_ERROR_MESHDATA_INDEXARRAY_ALREADY_CREATED) << "Unexpected error on create with existing index data";
	EXPECT_NE(testData.GetIndexData(), nullptr) << "Should still contain the data from initial create";
	EXPECT_EQ(testData.GetIndexCount(), indexCount) << "Index count should still be the same on initial create";
}

TEST_F(SDXMeshDataUTest, DestroyTest)
{
	SDXMeshData testData;
	UINT vertexCount = 100;
	UINT indexCount = 200;
	SDXVertexType type = SDXVERTEX_TYPE_PC;

	EXPECT_EQ(testData.CreateVertexArray(type, vertexCount), SDXErrorId::SDX_ERROR_NONE) << "Unexpected error on vertex array create";
	EXPECT_EQ(testData.CreateIndexArray(indexCount), SDXErrorId::SDX_ERROR_NONE) << "Unexpected error on index array create";

	// Test destroy
	testData.Destroy();
	EXPECT_EQ(testData.GetVertexData(), nullptr) << "Should be no vertex data after destroy";
	EXPECT_EQ(testData.GetVertexCount(), 0) << "Vertex count should be 0 after destroy";
	EXPECT_EQ(testData.GetIndexData(), nullptr) << "Should be no index data after destroy";
	EXPECT_EQ(testData.GetIndexCount(), 0) << "Index count should be 0 after destroy";
	EXPECT_EQ(testData.GetVertexType(), SDXVERTEX_TYPE_UNKNOWN) << "Vertex type should be unknown after destroy";
	EXPECT_EQ(testData.GetMaterialID(), "") << "Material id should be empty";
	EXPECT_EQ(testData.GetMinVertexPos().x, 0) << "Unexpected min position x after destroy";
	EXPECT_EQ(testData.GetMinVertexPos().y, 0) << "Unexpected min position y after destroy";
	EXPECT_EQ(testData.GetMinVertexPos().z, 0) << "Unexpected min position z after destroy";
	EXPECT_EQ(testData.GetMaxVertexPos().x, 0) << "Unexpected max position x after destroy";
	EXPECT_EQ(testData.GetMaxVertexPos().y, 0) << "Unexpected max position y after destroy";
	EXPECT_EQ(testData.GetMaxVertexPos().z, 0) << "Unexpected max position z after destroy";
}