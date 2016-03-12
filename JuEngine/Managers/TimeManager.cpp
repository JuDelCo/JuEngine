// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "TimeManager.hpp"
#include <thread>
#include <sstream>
#include <iomanip>

namespace JuEngine
{
TimeManager::TimeManager()
{
	SetId("timeManager");
}

void TimeManager::Update()
{
	mFrameCount++;
	mFpsCounter++;

	if(mAppFrameTime.GetTimeElapsed().AsSeconds() > 1.f)
	{
		mAppFrameTime.AddTime(-1.f);
		mFps = mFpsCounter;
		mFpsCounter = 0;
	}
}

void TimeManager::FixedUpdate()
{
	mDeltaTime = mAppFixedTime.Reset();
}

Time TimeManager::GetAppRunTime()
{
	return mAppRunTime.GetTimeElapsed();
}

auto TimeManager::GetDeltaTime() -> const Time&
{
	if(mDeltaTime > mMaxDeltaTime)
	{
		return mMaxDeltaTime;
	}

	return mDeltaTime;
}

void TimeManager::SetMaxDeltaTime(const Time maxDeltaTime)
{
	mMaxDeltaTime = maxDeltaTime;
}

auto TimeManager::GetFrameCount() -> const unsigned long&
{
	return mFrameCount;
}

auto TimeManager::GetFPS() -> const unsigned int&
{
	return mFps;
}

auto TimeManager::GetCurrentDate() -> std::string
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

auto TimeManager::GetCurrentTime() -> std::string
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

void TimeManager::Sleep(Time ms)
{
	if(ms.AsMicroseconds() > 100) // 0.1ms ignore
	{
		std::this_thread::sleep_for(std::chrono::microseconds(ms.AsMicroseconds()));
	}
}
}
