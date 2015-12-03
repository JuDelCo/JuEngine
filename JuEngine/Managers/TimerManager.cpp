// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "TimerManager.hpp"
#include "../Resources/DebugLog.hpp"

namespace JuEngine
{
TimerManager* TimerManager::mInstance = nullptr;

TimerManager::TimerManager() : IObject("timerManager")
{
	if(TimerManager::mInstance != nullptr)
	{
		ThrowRuntimeError("Error, there are a TimerManager instance created already");
	}

	TimerManager::mInstance = this;
}

TimerManager::~TimerManager()
{
	TimerManager::mInstance = nullptr;
}

auto TimerManager::Create(const string& name, const bool autoStart) -> Timer*
{
	if(TimerManager::mInstance->mTimers.count(name) != 0)
	{
		DebugLog::Write("Warning, attempted to create a timer with name '%s'. The timer name is already being used", name.c_str());

		return Get(name);
	}

	TimerManager::mInstance->mTimers[name] = std::make_shared<Timer>(autoStart);
	TimerManager::mInstance->mTimers.at(name)->SetName(name);

	return TimerManager::mInstance->mTimers.at(name).get();
}

auto TimerManager::Get(const string& name) -> Timer*
{
	if(TimerManager::mInstance->mTimers.count(name) != 0)
	{
		return TimerManager::mInstance->mTimers.at(name).get();
	}

	DebugLog::Write("Warning: TimerManager.Get: No timer found with name %s", name.c_str());

	return nullptr;
}

void TimerManager::Unload()
{
	TimerManager::mInstance->mTimers.clear();
}
}
