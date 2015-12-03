// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"
#include "../Resources/TimerCallback.hpp"
#include <vector>

namespace JuEngine
{
using std::vector;

class JUENGINEAPI TimerCallbackManager : public IObject
{
public:
	TimerCallbackManager();
	virtual ~TimerCallbackManager();

	void Update();
	static auto Create(const Time time, const Callback function, const bool loop = false) -> TimerCallback*;
	static void Unload();

private:
	vector<shared_ptr<TimerCallback>> mCallbackTimers;

	// Singleton
	static TimerCallbackManager* mInstance;
};
}
