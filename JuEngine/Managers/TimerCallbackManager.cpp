// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "TimerCallbackManager.hpp"
#include "../Resources/DebugLog.hpp"
#include <algorithm>

namespace JuEngine
{
TimerCallbackManager* TimerCallbackManager::mInstance = nullptr;

TimerCallbackManager::TimerCallbackManager() : IObject("timerCallbackManager")
{
	if(TimerCallbackManager::mInstance != nullptr)
	{
		ThrowRuntimeError("Error, there are a TimerCallbackManager instance created already");
	}

	TimerCallbackManager::mInstance = this;
}

TimerCallbackManager::~TimerCallbackManager()
{
	TimerCallbackManager::mInstance = nullptr;
}

void TimerCallbackManager::Update()
{
	for(auto& iTimer : mCallbackTimers)
	{
		iTimer->Update();
	}

	mCallbackTimers.erase(
		std::remove_if(std::begin(mCallbackTimers), std::end(mCallbackTimers),
		[](const shared_ptr<TimerCallback>& timerCallback)
		{
			return (! timerCallback->IsLooping() && timerCallback->WasTriggered());
		}),
		std::end(mCallbackTimers));
}

auto TimerCallbackManager::Create(const Time time, const Callback function, const bool loop) -> TimerCallback*
{
	shared_ptr<TimerCallback> psTimer(new TimerCallback(time, function, loop));
	TimerCallback* pwTimer{psTimer.get()};

	TimerCallbackManager::mInstance->mCallbackTimers.emplace_back(std::move(psTimer));

	return pwTimer;
}

void TimerCallbackManager::Unload()
{
	TimerCallbackManager::mInstance->mCallbackTimers.clear();
}
}
