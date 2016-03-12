// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../Resources/Time.hpp"

namespace JuEngine
{
class JUENGINEAPI ITimeService : public IObject
{
	friend class AppController;

	public:
		virtual void Update() = 0;
		virtual void FixedUpdate() = 0;
		virtual Time GetAppRunTime() = 0;
		virtual auto GetDeltaTime() -> const Time& = 0;
		virtual void SetMaxDeltaTime(const Time maxDeltaTime) = 0;
		virtual auto GetFrameCount() -> const unsigned long& = 0;
		virtual auto GetFPS() -> const unsigned int& = 0;
		virtual auto GetCurrentDate() -> std::string = 0;
		virtual auto GetCurrentTime() -> std::string = 0;

	protected:
		virtual void Sleep(Time ms) = 0;
};
}
