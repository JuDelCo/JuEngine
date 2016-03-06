// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Application.hpp"
#include "Managers/AppTimeManager.hpp"
#include "Managers/InputManager.hpp"
#include "Managers/LevelManager.hpp"
#include "Managers/MaterialManager.hpp"
#include "Managers/MeshManager.hpp"
#include "Managers/PoolManager.hpp"
#include "Managers/PrefabManager.hpp"
#include "Managers/ShaderManager.hpp"
#include "Managers/SystemManager.hpp"
#include "Managers/TimerManager.hpp"
#include "Managers/WindowManager.hpp"
#include "Resources/ForwardRenderer.hpp"
#include <thread>

namespace JuEngine
{
Application* Application::mInstance = nullptr;

Application::Application() : IObject("application")
{
	if(Application::mInstance != nullptr)
	{
		ThrowRuntimeError("Error, there are an Application instance created already");
	}

	Application::mInstance = this;

	SetFixedInterval(50.f);
	SetFrameInterval(60.f);
	SetEventInterval(30.f);

	mWindowManager = std::unique_ptr<WindowManager>(new WindowManager());
	mSystemManager = std::unique_ptr<SystemManager>(new SystemManager());
	mAppTimeManager = std::unique_ptr<AppTimeManager>(new AppTimeManager());
	mInputManager = std::unique_ptr<InputManager>(new InputManager());
	mTimerManager = std::unique_ptr<TimerManager>(new TimerManager());
	mPoolManager = std::unique_ptr<PoolManager>(new PoolManager());
	mPrefabManager = std::unique_ptr<PrefabManager>(new PrefabManager());
	mMaterialManager = std::unique_ptr<MaterialManager>(new MaterialManager());
	mMeshManager = std::unique_ptr<MeshManager>(new MeshManager());
	mShaderManager = std::unique_ptr<ShaderManager>(new ShaderManager());
	mLevelManager = std::unique_ptr<LevelManager>(new LevelManager());

	SystemInit();
}

Application::~Application()
{
	SystemEnd();

	Application::mInstance = nullptr;
}

void Application::Run()
{
	mIsRunning = true;

	mWindowManager->SetActiveInThisThread(false);

	std::thread runThread([&]()
	{
		mWindowManager->SetActiveInThisThread(true);
		mSystemManager->Initialize();

		Timer fixedTimer;
		Timer frameTimer;
		Time fixedTimeRemaining;
		Time frameTimeRemaining;

		while(mIsRunning)
		{
if(!mIsRunning) DebugLog::Write("RunThread: Starting loop");

			mLevelManager->Update();

if(!mIsRunning) DebugLog::Write("RunThread: Post-update");

			if(fixedTimer.GetTimeElapsed() >= mFixedInterval)
			{
				fixedTimer.Reset();

				mAppTimeManager->FixedUpdate();
				mSystemManager->FixedExecute();
			}

if(!mIsRunning) DebugLog::Write("RunThread: Post-fixedUpdate");

			if(frameTimer.GetTimeElapsed() >= mFrameInterval)
			{
				frameTimer.Reset();

				mAppTimeManager->Update();
				mInputManager->Update();
				mSystemManager->Execute();
				mWindowManager->Render();
			}

if(!mIsRunning) DebugLog::Write("RunThread: Post-update");

			fixedTimeRemaining = mFixedInterval - fixedTimer.GetTimeElapsed();
			frameTimeRemaining = mFrameInterval - frameTimer.GetTimeElapsed();
			AppTimeManager::Sleep(fixedTimeRemaining < frameTimeRemaining ? fixedTimeRemaining : frameTimeRemaining);

if(!mIsRunning) DebugLog::Write("RunThread: Ending loop");
		}

DebugLog::Write("RunThread: finished");
	});

	Timer eventTimer;

	while(mIsRunning)
	{
		if(eventTimer.GetTimeElapsed() >= mEventInterval)
		{
			eventTimer.Reset();

			mWindowManager->PollEvents();
		}

		AppTimeManager::Sleep(mEventInterval - eventTimer.GetTimeElapsed());
	}

DebugLog::Write("MainThread: finished, waiting for RunThread to finish");
	runThread.join();
DebugLog::Write("MainThread: finished");
}

void Application::Stop()
{
	Application::mInstance->mIsRunning = false;
}

void Application::SetFixedInterval(const float interval)
{
	if(interval > 1.f)
	{
		mFixedInterval = Time::Seconds(1.f / interval);
	}
}

void Application::SetFrameInterval(const float interval)
{
	if(interval > 1.f)
	{
		mFrameInterval = Time::Seconds(1.f / interval);
	}
}

void Application::SetEventInterval(const float interval)
{
	if(interval > 1.f)
	{
		mEventInterval = Time::Seconds(1.f / interval);
	}
}

void Application::SystemInit()
{
	mWindowManager->Load();
	mWindowManager->SetRenderer(std::shared_ptr<Renderer>(new ForwardRenderer()));
}

void Application::SystemEnd()
{
	mTimerManager->Unload();
	mLevelManager->Unload();
	mPrefabManager->Unload();
	mPoolManager->Unload();
	mMaterialManager->Unload();
	mShaderManager->Unload();
	mMeshManager->Unload();
}
}
