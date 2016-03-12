// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "LogManager.hpp"
#include "../App.hpp"
#include "../Services/ITimeService.hpp"

#include <fstream>
#include <stdarg.h>
#include <iostream>
#include <sstream>
#include <stdexcept> // runtime_error

namespace JuEngine
{
LogManager::LogManager()
{
#ifndef DEBUG_ON
	stdout = freopen("log.log", "w", stdout);
	stderr = freopen("err.log", "w", stderr);

	// Set buffers to NULL, so fflush() is not needed even if the program crashes
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
#endif
}

LogManager::~LogManager()
{
#ifndef DEBUG_ON
	fclose(stdout);
	fclose(stderr);

	#if defined(_WIN32)
		freopen("CON", "w", stdout);
		freopen("CON", "w", stderr);
	#endif

	RemoveLogIfEmpty("log.log");
	RemoveLogIfEmpty("err.log");
#endif
}

void LogManager::Log(const std::string& message, ...)
{
	va_list args;
	va_start(args, message);
	char buffer[1024];
	vsprintf(buffer, message.c_str(), args);

	std::cout << App::Time()->GetCurrentTime() << " " << buffer << std::endl;

	va_end(args);
}

void LogManager::Debug(const std::string& message, ...)
{
	va_list args;
	va_start(args, message);
	char buffer[1024];
	vsprintf(buffer, message.c_str(), args);

	std::cout << App::Time()->GetCurrentTime() << " " << buffer << std::endl;

	va_end(args);
}

void LogManager::Info(const std::string& message, ...)
{
	va_list args;
	va_start(args, message);
	char buffer[1024];
	vsprintf(buffer, message.c_str(), args);

	std::cout << App::Time()->GetCurrentTime() << " " << buffer << std::endl;

	va_end(args);
}

void LogManager::Notice(const std::string& message, ...)
{
	va_list args;
	va_start(args, message);
	char buffer[1024];
	vsprintf(buffer, message.c_str(), args);

	std::cout << App::Time()->GetCurrentTime() << " " << buffer << std::endl;

	va_end(args);
}

void LogManager::Warning(const std::string& message, ...)
{
	va_list args;
	va_start(args, message);
	char buffer[1024];
	vsprintf(buffer, message.c_str(), args);

	std::cout << App::Time()->GetCurrentTime() << " " << buffer << std::endl;

	va_end(args);
}

void LogManager::Error(const std::string& message, ...)
{
	va_list args;
	va_start(args, message);
	char buffer[1024];
	vsprintf(buffer, message.c_str(), args);

	std::cerr << App::Time()->GetCurrentTime() << " " << buffer << std::endl;

	va_end(args);
}

void LogManager::RunTimeError(const std::string& function, int line)
{
	std::stringstream ss;
	ss << function << " at line " << line;

	throw std::runtime_error(ss.str());
}

void LogManager::RemoveLogIfEmpty(const std::string& fileName)
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
