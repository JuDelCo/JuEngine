// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "AppController.hpp"
#include "Managers/DataManager.hpp"
#include "Managers/InputManager.hpp"
#include "Managers/LevelManager.hpp"
#include "Managers/LogManager.hpp"
#include "Managers/SystemManager.hpp"
#include "Managers/TimeManager.hpp"
#include "Managers/WindowManager.hpp"
#include "Resources/ForwardRenderer.hpp"
#include "App.hpp"
#include <thread>

namespace JuEngine
{
AppController::AppController()
{
	SetId("appController");

	SetFixedInterval(50.f);
	SetFrameInterval(60.f);
	SetEventInterval(30.f);

	BootstrapServices();
	SystemInit();
}

AppController::~AppController()
{
	SystemEnd();
}

void AppController::Run()
{
	mIsRunning = true;

	App::Window()->SetActiveInThisThread(false);

	std::thread runThread([&]()
	{
		App::Window()->SetActiveInThisThread(true);
		App::System()->Initialize();

		Timer fixedTimer;
		Timer frameTimer;
		Time fixedTimeRemaining;
		Time frameTimeRemaining;

		while(mIsRunning)
		{
			if(!mIsRunning)
				App::Log()->Debug("RunThread: Starting loop");

			App::Level()->Update();

			if(!mIsRunning)
				App::Log()->Debug("RunThread: Post-update");

			if(fixedTimer.GetTimeElapsed() >= mFixedInterval)
			{
				fixedTimer.Reset();

				App::Time()->FixedUpdate();
				App::System()->FixedExecute();
			}

			if(!mIsRunning)
				App::Log()->Debug("RunThread: Post-fixedUpdate");

			if(frameTimer.GetTimeElapsed() >= mFrameInterval)
			{
				frameTimer.Reset();

				App::Time()->Update();
				App::Input()->Update();
				App::System()->Execute();
				App::Window()->Render();
			}

			if(!mIsRunning)
				App::Log()->Debug("RunThread: Post-update");

			fixedTimeRemaining = mFixedInterval - fixedTimer.GetTimeElapsed();
			frameTimeRemaining = mFrameInterval - frameTimer.GetTimeElapsed();
			App::Time()->Sleep(fixedTimeRemaining < frameTimeRemaining ? fixedTimeRemaining : frameTimeRemaining);

			if(!mIsRunning)
				App::Log()->Debug("RunThread: Ending loop");
		}

		App::Log()->Debug("RunThread: finished");
	});

	Timer eventTimer;

	while(mIsRunning)
	{
		if(eventTimer.GetTimeElapsed() >= mEventInterval)
		{
			eventTimer.Reset();

			App::Window()->PollEvents();
		}

		App::Time()->Sleep(mEventInterval - eventTimer.GetTimeElapsed());
	}

	App::Log()->Debug("MainThread: finished, waiting for RunThread to finish");

	runThread.join();

	App::Log()->Debug("MainThread: finished");
}

void AppController::Stop()
{
	mIsRunning = false;
}

void AppController::BootstrapServices()
{
	App::Provide(this);
	App::Provide(new TimeManager());
	App::Provide(new LogManager());
	App::Provide(new WindowManager());
	App::Provide(new SystemManager());
	App::Provide(new InputManager());
	App::Provide(new DataManager());
	App::Provide(new LevelManager());
}

void AppController::SetFixedInterval(const float interval)
{
	if(interval > 1.f)
	{
		mFixedInterval = Time::Seconds(1.f / interval);
	}
}

void AppController::SetFrameInterval(const float interval)
{
	if(interval > 1.f)
	{
		mFrameInterval = Time::Seconds(1.f / interval);
	}
}

void AppController::SetEventInterval(const float interval)
{
	if(interval > 1.f)
	{
		mEventInterval = Time::Seconds(1.f / interval);
	}
}

void AppController::SystemInit()
{
	App::Window()->Load();
	App::Window()->SetRenderer(std::shared_ptr<Renderer>(new ForwardRenderer()));
}

void AppController::SystemEnd()
{
	App::Level()->DeleteAll();
	App::Data()->DeleteAll();
}
}
