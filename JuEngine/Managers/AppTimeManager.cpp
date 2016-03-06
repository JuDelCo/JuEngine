// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "AppTimeManager.hpp"
#include "../Resources/DebugLog.hpp"
#include <thread>

namespace JuEngine
{
AppTimeManager* AppTimeManager::mInstance = nullptr;

AppTimeManager::AppTimeManager() : IObject("appTimeManager")
{
	if(AppTimeManager::mInstance != nullptr)
	{
		ThrowRuntimeError("Error, there are an AppTimeManager instance created already");
	}

	AppTimeManager::mInstance = this;
}

AppTimeManager::~AppTimeManager()
{
	AppTimeManager::mInstance = nullptr;
}

void AppTimeManager::Update()
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

void AppTimeManager::FixedUpdate()
{
	mDeltaTime = mAppFixedTime.Reset();
}

Time AppTimeManager::GetAppRunTime()
{
	return AppTimeManager::mInstance->mAppRunTime.GetTimeElapsed();
}

auto AppTimeManager::GetDeltaTime() -> const Time&
{
	if(AppTimeManager::mInstance->mDeltaTime > AppTimeManager::mInstance->mMaxDeltaTime)
	{
		return AppTimeManager::mInstance->mMaxDeltaTime;
	}

	return AppTimeManager::mInstance->mDeltaTime;
}

void AppTimeManager::SetMaxDeltaTime(const Time maxDeltaTime)
{
	mMaxDeltaTime = maxDeltaTime;
}

auto AppTimeManager::GetFrameCount() -> const unsigned long&
{
	return AppTimeManager::mInstance->mFrameCount;
}

auto AppTimeManager::GetFPS() -> const unsigned int&
{
	return AppTimeManager::mInstance->mFps;
}

void AppTimeManager::Sleep(Time ms)
{
	if(ms.AsMicroseconds() > 100) // 0.1ms ignore
	{
		std::this_thread::sleep_for(std::chrono::microseconds(ms.AsMicroseconds()));
	}
}
}
