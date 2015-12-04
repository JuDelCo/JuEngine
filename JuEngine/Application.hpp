// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "IObject.hpp"
#include "Resources/Time.hpp"
#include "Resources/DebugLog.hpp"

namespace JuEngine
{
class Time;
class AppTimeManager;
class EntityManager;
class InputManager;
class LevelManager;
class MaterialManager;
class MeshManager;
class PrefabManager;
class ShaderManager;
class TimerManager;
class TimerCallbackManager;
class WindowManager;
class DebugLog;

class JUENGINEAPI Application : public IObject
{
public:
	Application();
	virtual ~Application();

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
	unique_ptr<AppTimeManager> mAppTimeManager;
	unique_ptr<EntityManager> mEntityManager;
	unique_ptr<InputManager> mInputManager;
	unique_ptr<LevelManager> mLevelManager;
	unique_ptr<MaterialManager> mMaterialManager;
	unique_ptr<MeshManager> mMeshManager;
	unique_ptr<PrefabManager> mPrefabManager;
	unique_ptr<ShaderManager> mShaderManager;
	unique_ptr<TimerManager> mTimerManager;
	unique_ptr<TimerCallbackManager> mTimerCallbackManager;
	unique_ptr<WindowManager> mWindowManager;
#ifndef DEBUG_ON
	DebugLog mDebugLog;
#endif

	// Singleton
	static Application* mInstance;
};
}
