// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Timer.hpp"

namespace JuEngine
{
Timer::Timer(const bool autoStart) : IObject("timer")
{
	if(autoStart)
	{
		Start();
	}
}

void Timer::Start()
{
	if(IsStarted())
	{
		return;
	}

	mStarted = true;
	mPaused = false;

	Reset();
}

void Timer::Reset()
{
	if(! IsStarted())
	{
		return;
	}

	mCounter.Reset();
	mPauseStartTime = Time::Zero;
	mPausedTime = Time::Zero;
}

void Timer::Pause()
{
	if(! IsStarted() || IsPaused())
	{
		return;
	}

	mPauseStartTime = mCounter.GetTimeElapsed();
	mPaused = true;
}

void Timer::Resume()
{
	if(! IsPaused())
	{
		return;
	}

	mPausedTime += mCounter.GetTimeElapsed() - mPauseStartTime;
	mPaused = false;
}

void Timer::Stop()
{
	if(! IsStarted())
	{
		return;
	}

	mStarted = false;
	mPaused = false;
}

void Timer::AddTime(const float seconds)
{
	AddTime(Time::Seconds(seconds));
}

void Timer::AddTime(const Time time)
{
	mPausedTime -= time;
}

auto Timer::IsStarted() const -> const bool&
{
	return mStarted;
}

auto Timer::IsPaused() const -> const bool&
{
	return mPaused;
}

Time Timer::GetTimeElapsed() const
{
	if(IsStarted())
	{
		auto elapsedTime = mCounter.GetTimeElapsed();

		if(IsPaused())
		{
			return (elapsedTime - mPausedTime - (elapsedTime - mPauseStartTime));
		}
		else
		{
			return (elapsedTime - mPausedTime);
		}
	}

	return Time::Zero;
}
}
