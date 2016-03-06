// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/INonCopyable.hpp"
#include <string>

namespace JuEngine
{
class JUENGINEAPI DebugLog : public INonCopyable
{
	public:
		DebugLog();
		~DebugLog();

		static void Write(const std::string& message, ...);
		static void Error(const std::string& function, int line);
		static std::string GetCurrentDate();
		static std::string GetCurrentTime();

	private:
		void RemoveLogIfEmpty(const std::string& fileName);
};

#define ThrowRuntimeError(message, ...) \
do { \
	DebugLog::Write(message, ##__VA_ARGS__); \
	DebugLog::Error(std::string(__PRETTY_FUNCTION__), __LINE__); \
} while(0);
}
