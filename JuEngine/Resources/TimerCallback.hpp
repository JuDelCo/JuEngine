// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "Timer.hpp"
#include <functional>

namespace JuEngine
{
typedef std::function<void()> Callback;

class JUENGINEAPI TimerCallback : public Timer
{
public:
	TimerCallback(const float timeDelay, const Callback callback, const bool loop = false);
	TimerCallback(const Time timeDelay, const Callback callback, const bool loop = false);
	virtual ~TimerCallback() = default;

	void Update();
	void Reset();
	void AddTimeDelay(const float seconds);
	void AddTimeDelay(const Time time);
	void SetTimeDelay(const float seconds);
	void SetTimeDelay(const Time time);
	auto GetTimeDelay() const -> const Time&;
	Time GetTimeRemaining() const;
	auto GetTimeRemainingMs() const -> unsigned int;
	void SetLooping(const bool loop);
	auto IsLooping() const -> const bool&;
	auto WasTriggered() const -> const bool&;

private:
	void RunCallback();

	bool mWasTriggered{false};
	bool mLoops;
	Time mTimeDelay;
	Callback mCallback;
};
}
