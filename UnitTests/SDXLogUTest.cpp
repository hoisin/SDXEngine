#include "pch.h"
#include "SDXLogUTest.h"
#include "..\SDXEngine\SDXLog.h"

using namespace SDXEngine;


SDXLogUTest::SDXLogUTest()
{
	m_logFileName = "UTestLog.log";
}

SDXLogUTest::~SDXLogUTest()
{
}

void SDXLogUTest::SetUp()
{
}

void SDXLogUTest::TearDown()
{
}

// --------------------------------------------------------------------------------
// Class fixture testing
// --------------------------------------------------------------------------------
TEST_F(SDXLogUTest, SetUp)
{
	SDXLog* log = SDXLOG->GetInstance();

	// Invalid file
	SDXErrorId error = log->Setup("");
	EXPECT_EQ(error, SDXErrorId::SDX_ERROR_LOG_OPEN_FAILED) << "Expected log file open failure with invalid file name";

	// Valid Setup run test
	error = log->Setup(m_logFileName);
	EXPECT_EQ(error, SDXErrorId::SDX_ERROR_NONE) << "Failed to setup log";

	// Setup again with same file
	error = log->Setup(m_logFileName);
	EXPECT_EQ(error, SDXErrorId::SDX_ERROR_LOG_FILE_ALREADY_OPEN) << "Unexpected error when attempting to open already opened file";

	// Try to setup with different file with one already opened
	error = log->Setup("newFile.log");
	EXPECT_EQ(error, SDXErrorId::SDX_ERROR_LOG_FILE_ALREADY_OPEN) << "Unexpected error when attempting to open another file when already have one opened";

	log->Release();

	// Clean up test log
	remove(m_logFileName.c_str());
}


TEST_F(SDXLogUTest, WriteLog)
{
	SDXLog* log = SDXLOG->GetInstance();

	// Attempt to write log without setup
	SDXErrorId error = log->WriteLog("Test");
	EXPECT_EQ(error, SDXErrorId::SDX_ERROR_LOG_NO_FILE_OPEN) << "Expecting no file open error when setup not ran";

	// Create log test
	error = log->Setup(m_logFileName);
	EXPECT_EQ(error, SDXErrorId::SDX_ERROR_NONE) << "Failed to setup log";

	// Valid write test
	log->SetUseTimeStamp(true);
	error = log->WriteLog("Test");
	EXPECT_EQ(error, SDXErrorId::SDX_ERROR_NONE) << "Unexpected error on valid write log";

	log->Release();

	// Clean up test log
	remove(m_logFileName.c_str());
}