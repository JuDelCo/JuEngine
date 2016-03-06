// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../Resources/Timer.hpp"

namespace JuEngine
{
class JUENGINEAPI AppTimeManager : public IObject
{
	friend class Application;

	public:
		AppTimeManager();
		~AppTimeManager();

		void Update();
		void FixedUpdate();
		static Time GetAppRunTime();
		static auto GetDeltaTime() -> const Time&;
		void SetMaxDeltaTime(const Time maxDeltaTime);
		static auto GetFrameCount() -> const unsigned long&;
		static auto GetFPS() -> const unsigned int&;

	protected:
		static void Sleep(Time ms);

	private:
		unsigned int mFps{0};
		unsigned int mFpsCounter{0};
		unsigned long mFrameCount{0L};
		Clock mAppRunTime;
		Timer mAppFrameTime;
		Clock mAppFixedTime;
		Time mDeltaTime;
		Time mMaxDeltaTime{Time::Milliseconds(20)};

		// Singleton
		static AppTimeManager* mInstance;
};
}
