// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "Time.hpp"

namespace JuEngine
{
class JUENGINEAPI Clock : public IObject
{
	public:
		Clock();

		Time Reset();
		Time GetTimeElapsed() const;

	private:
		std::chrono::high_resolution_clock::time_point mStartTime;
};
}
