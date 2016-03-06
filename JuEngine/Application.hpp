// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "Resources/IObject.hpp"
#include "Resources/Time.hpp"
#include "Resources/DebugLog.hpp"
#include <memory>

namespace JuEngine
{
class Time;
class AppTimeManager;
class InputManager;
class LevelManager;
class MaterialManager;
class MeshManager;
class PoolManager;
class PrefabManager;
class ShaderManager;
class TimerManager;
class SystemManager;
class WindowManager;
class DebugLog;

class JUENGINEAPI Application : public IObject
{
	public:
		Application();
		~Application();

		void Run();
		static void Stop();

	private:
		void SetFixedInterval(const float interval);
		void SetFrameInterval(const float interval);
		void SetEventInterval(const float interval);
		void SystemInit();
		void SystemEnd();

		bool mIsRunning{false};
		Time mFixedInterval;
		Time mFrameInterval;
		Time mEventInterval;
		std::unique_ptr<AppTimeManager> mAppTimeManager;
		std::unique_ptr<InputManager> mInputManager;
		std::unique_ptr<LevelManager> mLevelManager;
		std::unique_ptr<MaterialManager> mMaterialManager;
		std::unique_ptr<MeshManager> mMeshManager;
		std::unique_ptr<PoolManager> mPoolManager;
		std::unique_ptr<PrefabManager> mPrefabManager;
		std::unique_ptr<ShaderManager> mShaderManager;
		std::unique_ptr<TimerManager> mTimerManager;
		std::unique_ptr<SystemManager> mSystemManager;
		std::unique_ptr<WindowManager> mWindowManager;
#ifndef DEBUG_ON
		DebugLog mDebugLog;
#endif

		// Singleton
		static Application* mInstance;
};
}
