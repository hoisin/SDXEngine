//--------------------------------------------------------------------------
//
// Simple Log class
//
//--------------------------------------------------------------------------
#pragma once

#include <iostream>
#include <fstream>
#include "SDXErrors.h"

namespace SDXEngine
{
	class SDXLog
	{
	public:
		SDXLog();
		~SDXLog();

		SDXErrorId Setup(const std::string& fileName);
		void SetUseTimeStamp(bool use);

		SDXErrorId WriteLog(const std::string& text);

		static SDXLog* GetInstance();
		void Release();

	private:
		void WriteLogHeader();

		std::string GetDateTime() const;
		std::string GetLogLineNumber();

	private:
		std::ofstream m_log;
		static SDXLog* __instance;

		int m_currentLine;
		bool m_useTimeStamp;
	};

}

#define SDXLOG SDXLog::GetInstance()