// Copyright (c) 2016 Juan Delgado (JuDelCo)
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

auto TimerManager::Create(const Identifier& id, const bool autoStart) -> Timer*
{
	if(TimerManager::mInstance->mTimers.count(id) != 0)
	{
		DebugLog::Write("Warning, attempted to create a timer with id '%s'. The timer id is already being used", id.GetStringRef().c_str());

		return Get(id);
	}

	TimerManager::mInstance->mTimers[id] = std::make_shared<Timer>(autoStart);
	TimerManager::mInstance->mTimers.at(id)->SetId(id);

	return TimerManager::mInstance->mTimers.at(id).get();
}

auto TimerManager::Get(const Identifier& id) -> Timer*
{
	if(TimerManager::mInstance->mTimers.count(id) != 0)
	{
		return TimerManager::mInstance->mTimers.at(id).get();
	}

	DebugLog::Write("Warning: TimerManager.Get: No timer found with id %s", id.GetStringRef().c_str());

	return nullptr;
}

void TimerManager::Unload()
{
	TimerManager::mInstance->mTimers.clear();
}
}
