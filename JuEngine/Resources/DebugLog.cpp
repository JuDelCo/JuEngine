// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "DebugLog.hpp"
#include <fstream>
#include <stdarg.h>
#include <iostream>

#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time

namespace JuEngine
{
DebugLog::DebugLog() : IObject("debugLog")
{
    stdout = freopen("log.log", "w", stdout);
    stderr = freopen("err.log", "w", stderr);

	// Set buffers to NULL, so fflush() is not needed even if the program crashes
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
}

DebugLog::~DebugLog()
{
	fclose(stdout);
	fclose(stderr);

    stdout = freopen("CON", "w", stdout);
    stderr = freopen("CON", "w", stderr);

	RemoveLogIfEmpty("log.log");
	RemoveLogIfEmpty("err.log");
}

void DebugLog::Write(const string message, ...)
{
	va_list args;
	va_start(args, message);
	char buffer[1024];
	vsprintf(buffer, message.c_str(), args);

	std::cout << DebugLog::GetCurrentTime() << " " << buffer << std::endl;

	va_end(args);
}

void DebugLog::Error(const string function, int line)
{
	throw std::runtime_error(function);
}

string DebugLog::GetCurrentDate()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);
	struct tm *parts = std::localtime(&in_time_t);

	std::stringstream ss;

	ss << (1900 + parts->tm_year);
	ss << "-";
	ss << std::setw(2) << std::setfill('0') << (parts->tm_mon + 1);
	ss << "-";
	ss << std::setw(2) << std::setfill('0') << parts->tm_mday;

	return ss.str();
}

string DebugLog::GetCurrentTime()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);
	struct tm *parts = std::localtime(&in_time_t);

	std::stringstream ss;

	ss << std::setw(2) << std::setfill('0') << parts->tm_hour;
	ss << ":";
	ss << std::setw(2) << std::setfill('0') << parts->tm_min;
	ss << ":";
	ss << std::setw(2) << std::setfill('0') << parts->tm_sec;

	return ss.str();
}

void DebugLog::RemoveLogIfEmpty(const string& fileName)
{
	std::ifstream fs{fileName};

	if(!fs.is_open())
	{
		return;
	}

	if(fs.peek() != std::ifstream::traits_type::eof())
	{
		return;
	}

	fs.close();

	std::remove(fileName.c_str());
}
}
