#include "pch.h"
#include "CTimerUTest.h"
#include "..\SDXEngine\CTimer.h"

#include <Windows.h>

CTimerUTest::CTimerUTest()
{
}

CTimerUTest::~CTimerUTest()
{
}

void CTimerUTest::SetUp()
{
}

void CTimerUTest::TearDown()
{
}


// --------------------------------------------------------------------------------
// Class fixture testing
// --------------------------------------------------------------------------------
TEST_F(CTimerUTest, Defaults)
{
	CTimer testTimer;
	// Default value tests
	EXPECT_EQ(testTimer.Time(), 0) << "Current duration should be 0";
	EXPECT_EQ(testTimer.DeltaTimeMicro(), 0) << "Delta microseconds should be 0";
	EXPECT_EQ(testTimer.DeltaTimeMilli(), 0) << "Delta milliseconds should be 0";
	EXPECT_EQ(testTimer.DeltaTimeSecs(), 0) << "Delta seconds should be 0";
}


TEST_F(CTimerUTest, TestStartTick)
{
	CTimer testTimer;

	double expectedDurationSecs = 1.0;
	double expectedDurationMilli = 1000;
	double expectedDurationMicro = 1000000;
	double errorMarginMilli = 2;
	
	// Test just running tick
	testTimer.Tick();

	// Should be 0
	EXPECT_EQ(testTimer.Time(), 0) << "Current duration should be 0";
	EXPECT_EQ(testTimer.DeltaTimeMicro(), 0) << "Delta microseconds should be 0";
	EXPECT_EQ(testTimer.DeltaTimeMilli(), 0) << "Delta milliseconds should be 0";
	EXPECT_EQ(testTimer.DeltaTimeSecs(), 0) << "Delta seconds should be 0";

	testTimer.Start();

	Sleep(static_cast<DWORD>(expectedDurationMilli));

	testTimer.Tick();

	testTimer.Stop();

	// Valid test with Start, Tick and Stop
	// Test timer tick with expected duration
	double resultTime = testTimer.Time();
	double resultDeltaMicro = testTimer.DeltaTimeMicro();
	double resultDeltaMilli = testTimer.DeltaTimeMilli();
	double resultDeltaSecs = testTimer.DeltaTimeSecs();

	// Current duration test with error margin factored in
	EXPECT_EQ((resultTime >= (expectedDurationSecs - (errorMarginMilli * 0.001)))
		&& (resultTime <= (expectedDurationSecs + (errorMarginMilli * 0.001))), true) << "Unexpected time duration";
	
	EXPECT_EQ((resultDeltaMicro >= (expectedDurationMicro - (errorMarginMilli * 1000)))
		&& (resultDeltaMicro <= (expectedDurationMicro + (errorMarginMilli * 1000))), true) << "Unexpected delta time micro";

	EXPECT_EQ((resultDeltaMilli >= (resultDeltaMilli - (errorMarginMilli)))
		&& (resultDeltaMilli <= (resultDeltaMilli + (errorMarginMilli))), true) << "Unexpected delta time milli";

	EXPECT_EQ((resultDeltaSecs >= (expectedDurationSecs - (errorMarginMilli * 0.001)))
		&& (resultDeltaSecs <= (expectedDurationSecs + (errorMarginMilli * 0.001))), true) << "Unexpected delta time seconds";

	// Sleep
	Sleep(static_cast<DWORD>(expectedDurationMilli));

	// Test time after stop without running tick
	resultTime = testTimer.Time();
	resultDeltaMicro = testTimer.DeltaTimeMicro();
	resultDeltaMilli = testTimer.DeltaTimeMilli();
	resultDeltaSecs = testTimer.DeltaTimeSecs();

	// Duration should be the same as timer stopped
	EXPECT_EQ((resultTime >= (expectedDurationSecs - (errorMarginMilli * 0.001)))
		&& (resultTime <= (expectedDurationSecs + (errorMarginMilli * 0.001))), true) << "Unexpected time duration";

	EXPECT_EQ((resultDeltaMicro >= (expectedDurationMicro - (errorMarginMilli * 1000)))
		&& (resultDeltaMicro <= (expectedDurationMicro + (errorMarginMilli * 1000))), true) << "Unexpected delta time micro";

	EXPECT_EQ((resultDeltaMilli >= (resultDeltaMilli - (errorMarginMilli)))
		&& (resultDeltaMilli <= (resultDeltaMilli + (errorMarginMilli))), true) << "Unexpected delta time milli";

	EXPECT_EQ((resultDeltaSecs >= (expectedDurationSecs - (errorMarginMilli * 0.001)))
		&& (resultDeltaSecs <= (expectedDurationSecs + (errorMarginMilli * 0.001))), true) << "Unexpected delta time seconds";

	// Run tick
	testTimer.Tick();

	// Test timer after stop with tick
	resultTime = testTimer.Time();
	resultDeltaMicro = testTimer.DeltaTimeMicro();
	resultDeltaMilli = testTimer.DeltaTimeMilli();
	resultDeltaSecs = testTimer.DeltaTimeSecs();

	// Duration should be the same as timer stopped
	EXPECT_EQ((resultTime >= (expectedDurationSecs - (errorMarginMilli * 0.001)))
		&& (resultTime <= (expectedDurationSecs + (errorMarginMilli * 0.001))), true) << "Unexpected time duration";

	EXPECT_EQ(resultDeltaMicro, 0) << "Delta microseconds should be 0";
	EXPECT_EQ(resultDeltaMilli, 0) << "Delta milliseconds should be 0";
	EXPECT_EQ(resultDeltaSecs, 0) << "Delta seconds should be 0";

	// Resume timer
	testTimer.Start();
	Sleep(static_cast<DWORD>(expectedDurationMilli));
	testTimer.Tick();

	// Test after resuming timer
	resultTime = testTimer.Time();
	resultDeltaMicro = testTimer.DeltaTimeMicro();
	resultDeltaMilli = testTimer.DeltaTimeMilli();
	resultDeltaSecs = testTimer.DeltaTimeSecs();

	EXPECT_EQ((resultTime >= (expectedDurationSecs * 2 - (errorMarginMilli * 0.001)))
		&& (resultTime <= (expectedDurationSecs * 2 + (errorMarginMilli * 0.001))), true) << "Unexpected time duration";

	// Deltas
	EXPECT_EQ((resultDeltaMicro >= (expectedDurationMicro - (errorMarginMilli * 1000)))
		&& (resultDeltaMicro <= (expectedDurationMicro + (errorMarginMilli * 1000))), true) << "Unexpected delta time micro";

	EXPECT_EQ((resultDeltaMilli >= (resultDeltaMilli - (errorMarginMilli)))
		&& (resultDeltaMilli <= (resultDeltaMilli + (errorMarginMilli))), true) << "Unexpected delta time milli";

	EXPECT_EQ((resultDeltaSecs >= (expectedDurationSecs - (errorMarginMilli * 0.001)))
		&& (resultDeltaSecs <= (expectedDurationSecs + (errorMarginMilli * 0.001))), true) << "Unexpected delta time seconds";
	
	// Test reset
	testTimer.Stop();
	testTimer.Reset();

	resultTime = testTimer.Time();
	resultDeltaMicro = testTimer.DeltaTimeMicro();
	resultDeltaMilli = testTimer.DeltaTimeMilli();
	resultDeltaSecs = testTimer.DeltaTimeSecs();

	// Should be 0
	EXPECT_EQ(resultTime, 0) << "Current duration should be 0";
	// Deltas should be the same as before since tick not called
	EXPECT_EQ((resultDeltaMicro >= (expectedDurationMicro - (errorMarginMilli * 1000)))
		&& (resultDeltaMicro <= (expectedDurationMicro + (errorMarginMilli * 1000))), true) << "Unexpected delta time micro";

	EXPECT_EQ((resultDeltaMilli >= (resultDeltaMilli - (errorMarginMilli)))
		&& (resultDeltaMilli <= (resultDeltaMilli + (errorMarginMilli))), true) << "Unexpected delta time milli";

	EXPECT_EQ((resultDeltaSecs >= (expectedDurationSecs - (errorMarginMilli * 0.001)))
		&& (resultDeltaSecs <= (expectedDurationSecs + (errorMarginMilli * 0.001))), true) << "Unexpected delta time seconds";

	// Test call Tick after reset
	testTimer.Tick();
	resultTime = testTimer.Time();
	resultDeltaMicro = testTimer.DeltaTimeMicro();
	resultDeltaMilli = testTimer.DeltaTimeMilli();
	resultDeltaSecs = testTimer.DeltaTimeSecs();
	EXPECT_EQ(resultTime, 0) << "Current duration should be 0";
	EXPECT_EQ(resultDeltaMicro, 0) << "Delta microseconds should be 0";
	EXPECT_EQ(resultDeltaMilli, 0) << "Delta milliseconds should be 0";
	EXPECT_EQ(resultDeltaSecs, 0) << "Delta seconds should be 0";
}
