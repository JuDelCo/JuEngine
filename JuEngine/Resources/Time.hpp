// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../DllExport.hpp"
#include <chrono>

namespace JuEngine
{
class JUENGINEAPI Time
{
	public:
		Time();

		auto AsSeconds() const -> float;
		auto AsMilliseconds() const -> int32_t;
		auto AsMicroseconds() const -> int64_t;

		static Time Seconds(const float amount);
		static Time Milliseconds(const int32_t amount);
		static Time Microseconds(const int64_t amount);

		static const Time Zero;

	private:
		explicit Time(const int64_t microseconds);

		std::chrono::microseconds mMicroseconds;
};

bool  operator ==(Time left, Time right);
bool  operator !=(Time left, Time right);
bool  operator  <(Time left, Time right);
bool  operator  >(Time left, Time right);
bool  operator <=(Time left, Time right);
bool  operator >=(Time left, Time right);
Time  operator  -(Time right);
Time  operator  +(Time left, Time right);
Time& operator +=(Time& left, Time right);
Time  operator  -(Time left, Time right);
Time& operator -=(Time& left, Time right);
Time  operator  *(Time left, float right);
Time  operator  *(Time left, int64_t right);
Time  operator  *(float left, Time right);
Time  operator  *(int64_t left, Time right);
Time& operator *=(Time& left, float right);
Time& operator *=(Time& left, int64_t right);
Time  operator  /(Time left, float right);
Time  operator  /(Time left, int64_t right);
Time& operator /=(Time& left, float right);
Time& operator /=(Time& left, int64_t right);
float operator  /(Time left, Time right);
Time  operator  %(Time left, Time right);
Time& operator %=(Time& left, Time right);
}
