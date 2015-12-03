// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"
#include "Time.hpp"

namespace JuEngine
{
using std::chrono::high_resolution_clock;

class JUENGINEAPI Clock : public IObject
{
public:
	Clock();
	virtual ~Clock() = default;

	Time Reset();
	Time GetTimeElapsed() const;

private:
	high_resolution_clock::time_point mStartTime;
};
}
