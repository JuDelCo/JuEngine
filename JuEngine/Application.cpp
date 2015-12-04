// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Application.hpp"
#include "Includes.hpp"
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

	SetFixedInterval(50.0f);
	SetFrameInterval(60.0f);
	SetEventInterval(30.0f);

	mWindowManager = unique_ptr<WindowManager>(new WindowManager());
	mAppTimeManager = unique_ptr<AppTimeManager>(new AppTimeManager());
	mInputManager = unique_ptr<InputManager>(new InputManager());
	mEntityManager = unique_ptr<EntityManager>(new EntityManager());
	mTimerManager = unique_ptr<TimerManager>(new TimerManager());
	mTimerCallbackManager = unique_ptr<TimerCallbackManager>(new TimerCallbackManager());
	mPrefabManager = unique_ptr<PrefabManager>(new PrefabManager());
	mMaterialManager = unique_ptr<MaterialManager>(new MaterialManager());
	mMeshManager = unique_ptr<MeshManager>(new MeshManager());
	mShaderManager = unique_ptr<ShaderManager>(new ShaderManager());
	mLevelManager = unique_ptr<LevelManager>(new LevelManager());

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
				mEntityManager->FixedUpdate();
			}

if(!mIsRunning) DebugLog::Write("RunThread: Post-fixedUpdate");

			if(frameTimer.GetTimeElapsed() >= mFrameInterval)
			{
				frameTimer.Reset();

				mAppTimeManager->Update();
				mInputManager->Update();
				mEntityManager->Update();
				mTimerCallbackManager->Update();
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
	if(interval > 1.0f)
	{
		mFixedInterval = Time::Seconds(1.0f / interval);
	}
}

void Application::SetFrameInterval(const float interval)
{
	if(interval > 1.0f)
	{
		mFrameInterval = Time::Seconds(1.0f / interval);
	}
}

void Application::SetEventInterval(const float interval)
{
	if(interval > 1.0f)
	{
		mEventInterval = Time::Seconds(1.0f / interval);
	}
}

void Application::SystemInit()
{
	mWindowManager->Load();
	mWindowManager->SetRenderer(new ForwardRenderer());
}

void Application::SystemEnd()
{
	mEntityManager->Unload();
	mTimerCallbackManager->Unload();
	mTimerManager->Unload();
	mLevelManager->Unload();
	mPrefabManager->Unload();
	mMaterialManager->Unload();
	mShaderManager->Unload();
	mMeshManager->Unload();
}
}
