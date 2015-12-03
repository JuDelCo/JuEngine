// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"
#include "../Resources/Timer.hpp"
#include <unordered_map>

namespace JuEngine
{
using std::unordered_map;

class JUENGINEAPI TimerManager : public IObject
{
public:
	TimerManager();
	virtual ~TimerManager();

	static auto Create(const string& name, const bool autoStart = true) -> Timer*;
	static auto Get(const string& name) -> Timer*;
	static void Unload();

private:
	unordered_map<string, shared_ptr<Timer>> mTimers;

	// Singleton
	static TimerManager* mInstance;
};
}
