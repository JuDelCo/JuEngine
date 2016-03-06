// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "Entity.hpp"
#include "Matcher.hpp"
#include "TriggerOnEvent.hpp"
#include "../Resources/Clock.hpp"
#include "../DllExport.hpp"
#include <vector>

namespace JuEngine
{
class Pool;

class JUENGINEAPI ISystem
{
	protected:
		ISystem() = default;

	public:
		virtual ~ISystem() = default;
};

class JUENGINEAPI ISetPoolSystem
{
	protected:
		ISetPoolSystem() = default;

	public:
		virtual ~ISetPoolSystem() = default;

		virtual void SetPool(Pool* pool) = 0;
};

class JUENGINEAPI IInitializeSystem
{
	protected:
		IInitializeSystem() = default;

	public:
		virtual ~IInitializeSystem() = default;

		virtual void Initialize() = 0;
};

class JUENGINEAPI IExecuteSystem : public ISystem
{
	protected:
		IExecuteSystem() = default;

	public:
		virtual ~IExecuteSystem() = default;

		virtual void Execute() = 0;
};

class JUENGINEAPI IFixedExecuteSystem : public ISystem
{
	protected:
		IFixedExecuteSystem() = default;

	public:
		virtual ~IFixedExecuteSystem() = default;

		virtual void FixedExecute() = 0;
};

class JUENGINEAPI ITimedExecuteSystem : public IInitializeSystem
{
	friend class SystemContainer;

	public:
		ITimedExecuteSystem(const float timeDelay) : ITimedExecuteSystem(Time::Seconds(timeDelay)) {}
		ITimedExecuteSystem(const Time timeDelay) : mTimeDelay(timeDelay) {}
		virtual ~ITimedExecuteSystem() = default;

		void Initialize()
		{
			mClock.Reset();
		}

		virtual void TimedExecute() = 0;

	protected:
		Time mTimeDelay;
		Clock mClock;
};

class JUENGINEAPI IReactiveExecuteSystem : public ISystem
{
	protected:
		IReactiveExecuteSystem() = default;

	public:
		virtual ~IReactiveExecuteSystem() = default;

		virtual void Execute(std::vector<EntityPtr> entities) = 0;
};

class JUENGINEAPI IReactiveSystem : public IReactiveExecuteSystem
{
	public:
		virtual ~IReactiveSystem() = default;

		TriggerOnEvent trigger;
};

class JUENGINEAPI IMultiReactiveSystem : public IReactiveExecuteSystem
{
	public:
		virtual ~IMultiReactiveSystem() = default;

		std::vector<TriggerOnEvent> triggers;
};

class JUENGINEAPI IEnsureComponents
{
	protected:
		IEnsureComponents() = default;

	public:
		Matcher ensureComponents;
};

class JUENGINEAPI IExcludeComponents
{
	protected:
		IExcludeComponents() = default;

	public:
		Matcher excludeComponents;
};

class JUENGINEAPI IClearReactiveSystem
{
	protected:
		IClearReactiveSystem() = default;
};
}
