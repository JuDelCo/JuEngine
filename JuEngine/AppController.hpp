// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "Services/IAppController.hpp"
#include "Resources/Time.hpp"
#include <memory>

namespace JuEngine
{
class JUENGINEAPI AppController : public IAppController
{
	public:
		AppController();
		virtual ~AppController();

		void Run();
		void Stop();

	protected:
		void BootstrapServices();

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
};
}
