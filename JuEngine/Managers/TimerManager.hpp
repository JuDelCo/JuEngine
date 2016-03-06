// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../Resources/Timer.hpp"
#include <unordered_map>
#include <memory>

namespace JuEngine
{
class JUENGINEAPI TimerManager : public IObject
{
	public:
		TimerManager();
		~TimerManager();

		static auto Create(const Identifier& id, const bool autoStart = true) -> Timer*;
		static auto Get(const Identifier& id) -> Timer*;
		static void Unload();

	private:
		std::unordered_map<Identifier, std::shared_ptr<Timer>> mTimers;

		// Singleton
		static TimerManager* mInstance;
};
}
