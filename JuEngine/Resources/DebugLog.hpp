// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"

namespace JuEngine
{
class JUENGINEAPI DebugLog : public IObject
{
	public:
		DebugLog();
		virtual ~DebugLog();

		static void Write(const string message, ...);
		static void Error(const string function, int line);
		static string GetCurrentDate();
		static string GetCurrentTime();

	private:
		void RemoveLogIfEmpty(const string& fileName);
};

#define ThrowRuntimeError(message, ...) \
do { \
	DebugLog::Write(message, ##__VA_ARGS__); \
	DebugLog::Error(string(__PRETTY_FUNCTION__), __LINE__); \
} while(0);
}
