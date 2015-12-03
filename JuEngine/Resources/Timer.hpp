// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"
#include "Clock.hpp"

namespace JuEngine
{
class JUENGINEAPI Timer : public IObject
{
public:
	Timer(const bool autoStart = true);
	virtual ~Timer() = default;

	void Start();
	void Reset();
	void Pause();
	void Resume();
	void Stop();
	void AddTime(const float seconds);
	void AddTime(const Time time);
	auto IsStarted() const -> const bool&;
	auto IsPaused() const -> const bool&;
	Time GetTimeElapsed() const;

private:
	Clock mCounter;
	Time mPauseStartTime;
	Time mPausedTime;
	bool mStarted{false};
	bool mPaused{false};
};
}
