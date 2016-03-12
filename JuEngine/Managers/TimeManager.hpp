// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Services/ITimeService.hpp"
#include "../Resources/Timer.hpp"

namespace JuEngine
{
class JUENGINEAPI TimeManager : public ITimeService
{
	public:
		TimeManager();

		void Update();
		void FixedUpdate();
		Time GetAppRunTime();
		auto GetDeltaTime() -> const Time&;
		void SetMaxDeltaTime(const Time maxDeltaTime);
		auto GetFrameCount() -> const unsigned long&;
		auto GetFPS() -> const unsigned int&;
		auto GetCurrentDate() -> std::string;
		auto GetCurrentTime() -> std::string;

	protected:
		void Sleep(Time ms);

	private:
		unsigned int mFps{0};
		unsigned int mFpsCounter{0};
		unsigned long mFrameCount{0L};
		Clock mAppRunTime;
		Timer mAppFrameTime;
		Clock mAppFixedTime;
		Time mDeltaTime;
		Time mMaxDeltaTime{Time::Milliseconds(20)};
};
}
