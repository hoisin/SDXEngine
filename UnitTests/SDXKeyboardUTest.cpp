#include "pch.h"
#include "SDXKeyboardUTest.h"
#include "..\SDXEngine\SDXKeyboard.h"
#include <string>

using namespace SDXEngine;

SDXKeyboardUTest::SDXKeyboardUTest()
{
}


SDXKeyboardUTest::~SDXKeyboardUTest()
{
}

void SDXKeyboardUTest::SetUp()
{
}

void SDXKeyboardUTest::TearDown()
{
}

TEST_F(SDXKeyboardUTest, DefaultState)
{
	SDXKeyboard testKeyboard;

	// Testing defaults for all ASCII keys
	for (int i = 0; i < 256; i++)
	{
		EXPECT_EQ(testKeyboard.IsKeyUp(i), true) << "Expected ASCII key " + std::to_string(i) + " to be up by default";
		EXPECT_EQ(testKeyboard.IsKeyDown(i), false) << "Expected ASCII key " + std::to_string(i) + " to not be down by default";
		EXPECT_EQ(testKeyboard.IsKeyHeld(i), false) << "Expected ASCII key " + std::to_string(i) + " to not be held by default";
	}
}

TEST_F(SDXKeyboardUTest, SetGetKeys)
{
	int key = 0;
	int totalKeys = 256;

	SDXKeyboard testKeyboard;

	for (key = 0; key < totalKeys; key++)
	{
		// Test down first since default is up
		testKeyboard.SetKeyDown(key);
		EXPECT_EQ(testKeyboard.IsKeyDown(key), true) << "Unexpected get for ASCII key " + std::to_string(key);

		// Now test the up
		testKeyboard.SetKeyUp(key);
		EXPECT_EQ(testKeyboard.IsKeyUp(key), true) << "Unexpected get for ASCII key " + std::to_string(key);
	}
}

TEST_F(SDXKeyboardUTest, UpdateHeldTest)
{
	int key = 0;
	int totalKeys = 256;
	double heldThreshold = 100;

	SDXKeyboard testKeyboard;

	// Test default state
	for (key = 0; key < totalKeys; key++)
	{
		EXPECT_EQ(testKeyboard.IsKeyUp(key), true) << "Expected 'up' state for key: " + std::to_string(key);
	}

	// Test after update with no set state, keys should stay in 'up' state
	testKeyboard.Update(heldThreshold);
	for (key = 0; key < totalKeys; key++)
	{
		EXPECT_EQ(testKeyboard.IsKeyUp(key), true) << "After update expected 'up' state for key: " + std::to_string(key);
	}

	// Test set down state in default (should be 0)
	for (key = 0; key < totalKeys; key++)
	{
		testKeyboard.SetKeyDown(key);
	}
	testKeyboard.Update(1);
	for (key = 0; key < totalKeys; key++)
	{
		EXPECT_EQ(testKeyboard.IsKeyHeld(key), true) << "Expected held state for default update for key: " + std::to_string(key);
	}

	// Reset all keys to 'up'
	for (key = 0; key < totalKeys; key++)
	{
		testKeyboard.SetKeyUp(key);
	}

	// Test updating threshold
	testKeyboard.SetHeldThreshold(heldThreshold);
	for (key = 0; key < totalKeys; key++)
	{
		testKeyboard.SetKeyDown(key);
	}
	// Update test not exceed threshold
	testKeyboard.Update(1);
	for (key = 0; key < totalKeys; key++)
	{
		EXPECT_EQ(testKeyboard.IsKeyHeld(key), false) << "Expected down state for under threshold update for key: " + std::to_string(key);
	}
	// Update test exceed threshold
	testKeyboard.Update(heldThreshold * 2);
	for (key = 0; key < totalKeys; key++)
	{
		EXPECT_EQ(testKeyboard.IsKeyHeld(key), true) << "Expected held state for exceed threshold update for key: " + std::to_string(key);
	}
}