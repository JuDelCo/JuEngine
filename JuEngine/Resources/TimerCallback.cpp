// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "TimerCallback.hpp"

namespace JuEngine
{
TimerCallback::TimerCallback(const float timeDelay, const Callback& callback, const bool loop) :
	TimerCallback(Time::Seconds(timeDelay), callback, loop)
{
}

TimerCallback::TimerCallback(const Time timeDelay, const Callback& callback, const bool loop) :
	mLoops(loop), mTimeDelay(timeDelay), mCallback(callback)
{
	this->SetId("timerCallback");
}

void TimerCallback::Update()
{
	if(! Timer::IsPaused())
	{
		if(! WasTriggered() && GetTimeRemaining() <= Time::Zero)
		{
			RunCallback();

			if(mLoops)
			{
				Reset();
			}
		}
	}
}

void TimerCallback::Reset()
{
	Timer::Reset();

	mWasTriggered = false;
}

void TimerCallback::AddTimeDelay(const float seconds)
{
	AddTimeDelay(Time::Seconds(seconds));
}

void TimerCallback::AddTimeDelay(const Time time)
{
	SetTimeDelay(mTimeDelay + time);
}

void TimerCallback::SetTimeDelay(const float seconds)
{
	SetTimeDelay(Time::Seconds(seconds));
}

void TimerCallback::SetTimeDelay(const Time time)
{
	mTimeDelay = time;
}

auto TimerCallback::GetTimeDelay() const -> const Time&
{
	return mTimeDelay;
}

Time TimerCallback::GetTimeRemaining() const
{
	Time timeElapsed = Timer::GetTimeElapsed();
	Time timeRemaining = mTimeDelay - timeElapsed;

	if(timeRemaining.AsSeconds() < 0.f)
	{
		timeRemaining = Time::Zero;
	}

	return timeRemaining;
}

auto TimerCallback::GetTimeRemainingMs() const -> unsigned int
{
	return GetTimeRemaining().AsMilliseconds();
}

void TimerCallback::SetLooping(const bool loop)
{
	if(loop)
	{
		if(mWasTriggered)
		{
			Reset();
		}

		mWasTriggered = false;
	}

	mLoops = loop;
}

auto TimerCallback::IsLooping() const -> const bool&
{
	return mLoops;
}

auto TimerCallback::WasTriggered() const -> const bool&
{
	return mWasTriggered;
}

void TimerCallback::RunCallback()
{
	mWasTriggered = true;
	mCallback();
}
}
