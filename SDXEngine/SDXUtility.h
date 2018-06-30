//--------------------------------------------------------------------------
//
//	Utiltiy methods
//
//--------------------------------------------------------------------------
#pragma once

#include <string>
#include <Windows.h>

namespace SDXEngine
{
	// Convert string to wide string
	std::wstring StringToWideString(const std::string& s);
}