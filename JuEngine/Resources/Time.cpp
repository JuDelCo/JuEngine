// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Time.hpp"

namespace JuEngine
{
const Time Time::Zero;

Time::Time() : mMicroseconds(std::chrono::microseconds::zero())
{
}

auto Time::AsSeconds() const -> float
{
	return mMicroseconds.count() / 1000000.f;
}

auto Time::AsMilliseconds() const -> int32_t
{
	return static_cast<int32_t>(mMicroseconds.count() / 1000);
}

auto Time::AsMicroseconds() const -> int64_t
{
	return mMicroseconds.count();
}

Time Time::Seconds(const float amount)
{
	return Time(static_cast<int64_t>(amount * 1000000));
}

Time Time::Milliseconds(const int32_t amount)
{
	return Time(static_cast<int64_t>(amount) * 1000);
}

Time Time::Microseconds(const int64_t amount)
{
	return Time(amount);
}

Time::Time(const int64_t microseconds) : mMicroseconds(microseconds)
{
}

bool operator ==(Time left, Time right)
{
	return left.AsMicroseconds() == right.AsMicroseconds();
}

bool operator !=(Time left, Time right)
{
	return left.AsMicroseconds() != right.AsMicroseconds();
}

bool operator <(Time left, Time right)
{
	return left.AsMicroseconds() < right.AsMicroseconds();
}

bool operator >(Time left, Time right)
{
	return left.AsMicroseconds() > right.AsMicroseconds();
}

bool operator <=(Time left, Time right)
{
	return left.AsMicroseconds() <= right.AsMicroseconds();
}

bool operator >=(Time left, Time right)
{
	return left.AsMicroseconds() >= right.AsMicroseconds();
}

Time operator -(Time right)
{
	return Time::Microseconds(-right.AsMicroseconds());
}

Time operator +(Time left, Time right)
{
	return Time::Microseconds(left.AsMicroseconds() + right.AsMicroseconds());
}

Time& operator +=(Time& left, Time right)
{
	return left = left + right;
}

Time operator -(Time left, Time right)
{
	return Time::Microseconds(left.AsMicroseconds() - right.AsMicroseconds());
}

Time& operator -=(Time& left, Time right)
{
	return left = left - right;
}

Time operator *(Time left, float right)
{
	return Time::Seconds(left.AsSeconds() * right);
}

Time operator *(Time left, int64_t right)
{
	return Time::Microseconds(left.AsMicroseconds() * right);
}

Time operator *(float left, Time right)
{
	return right * left;
}

Time operator *(int64_t left, Time right)
{
	return right * left;
}

Time& operator *=(Time& left, float right)
{
	return left = left * right;
}

Time& operator *=(Time& left, int64_t right)
{
	return left = left * right;
}

Time operator /(Time left, float right)
{
	return Time::Seconds(left.AsSeconds() / right);
}

Time operator /(Time left, int64_t right)
{
	return Time::Microseconds(left.AsMicroseconds() / right);
}

Time& operator /=(Time& left, float right)
{
	return left = left / right;
}

Time& operator /=(Time& left, int64_t right)
{
	return left = left / right;
}

float operator /(Time left, Time right)
{
	return left.AsSeconds() / right.AsSeconds();
}

Time operator %(Time left, Time right)
{
	return Time::Microseconds(left.AsMicroseconds() % right.AsMicroseconds());
}

Time& operator %=(Time& left, Time right)
{
	return left = left % right;
}
}
