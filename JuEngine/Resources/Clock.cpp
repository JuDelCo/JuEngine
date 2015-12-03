// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Clock.hpp"

namespace JuEngine
{
using std::chrono::microseconds;

Clock::Clock() : IObject("clock")
{
	Reset();
}

Time Clock::Reset()
{
	auto timeElapsed = GetTimeElapsed();

	mStartTime = high_resolution_clock::now();

	return timeElapsed;
}

Time Clock::GetTimeElapsed() const
{
	return Time::Microseconds((std::chrono::duration_cast<microseconds>(high_resolution_clock::now() - mStartTime)).count());
}
}
