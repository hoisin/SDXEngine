#include "stdafx.h"
#include "SDXUtility.h"

using namespace SDXEngine;

std::wstring SDXEngine::StringToWideString(const std::string& s)
{
	int len;
	int sLength = static_cast<int>(s.length() + 1);
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), sLength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), sLength, buf, len);
	std::wstring returnString(buf);
	delete[] buf;
	return returnString;
}
