#include "stdafx.h"
#include <chrono>
#include <sstream>
#include "SDXLog.h"

using namespace SDXEngine;

SDXLog* SDXLog::__instance = nullptr;

SDXLog::SDXLog() : m_useTimeStamp(false), m_currentLine(1)
{
}

SDXLog::~SDXLog()
{
	if(m_log.is_open())
		m_log.close();
}

SDXErrorId SDXEngine::SDXLog::Setup(const std::string& fileName)
{
	// Existing open file check
	if (m_log.is_open())
		return SDX_ERROR_LOG_FILE_ALREADY_OPEN;

	// Attempt to open the file
	m_log.open(fileName, std::ofstream::out | std::ofstream::app);
	// File open failure check
	if (!m_log.is_open())
		return SDX_ERROR_LOG_OPEN_FAILED;

	WriteLogHeader();
	return SDX_ERROR_NONE;
}

void SDXEngine::SDXLog::SetUseTimeStamp(bool use)
{
	m_useTimeStamp = use;
}

SDXErrorId SDXEngine::SDXLog::WriteLog(const std::string & text)
{
	if (!m_log.is_open())
		return SDX_ERROR_LOG_NO_FILE_OPEN;

	if (m_useTimeStamp)
	{
		m_log << GetLogLineNumber() << " " << GetDateTime() << ":-  " << text << "\n";
	}
	else
	{
		m_log <<GetLogLineNumber() << ":-  " << text << "\n";
	}
	m_log.flush();
	return SDX_ERROR_NONE;
}

SDXLog * SDXEngine::SDXLog::GetInstance()
{
	if (__instance)
		return __instance;

	__instance = new SDXLog;
	return __instance;
}

void SDXEngine::SDXLog::Release()
{
	if (__instance)
	{
		delete __instance;
		__instance = nullptr;
	}
}

void SDXLog::WriteLogHeader()
{
	if (m_log.is_open())
	{
		m_log << "=================================================================\n";
		m_log << "\n";
		m_log << "\n";
		m_log << "                       SDX Log Entry\n";
		m_log << "                    " + GetDateTime() + "\n";
		m_log << "\n";
		m_log << "=================================================================\n";
		m_log.flush();
	}
}

std::string SDXEngine::SDXLog::GetDateTime() const
{
	auto now = std::chrono::system_clock::now();
	auto logTime = std::chrono::system_clock::to_time_t(now);
	struct tm buf;
	localtime_s(&buf, &logTime);
	std::string timeStamp;
	timeStamp = std::to_string(buf.tm_mday) + "/" + std::to_string(buf.tm_mon) + "/" + std::to_string(buf.tm_year + 1900);
	timeStamp += "-";
	timeStamp += std::to_string(buf.tm_hour) + ":" + std::to_string(buf.tm_min) + ":" + std::to_string(buf.tm_sec);
	return timeStamp;
}

std::string SDXEngine::SDXLog::GetLogLineNumber()
{
	char buffer[32];
	sprintf_s(buffer, "%06d", m_currentLine);
	std::string returnString(buffer);
	// Increment log line number
	m_currentLine++;
	return returnString;
}
