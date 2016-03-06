// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../DllExport.hpp"
#include "ISystem.hpp"
#include <vector>

namespace JuEngine
{
class JUENGINEAPI SystemContainer : public IInitializeSystem, public IExecuteSystem, public IFixedExecuteSystem
{
	public:
		SystemContainer() = default;

		auto Add(std::shared_ptr<ISystem> system) -> SystemContainer*;
		template <typename T> inline auto Add() -> SystemContainer*;

		void Initialize();
		void Execute();
		void FixedExecute();

		void ActivateReactiveSystems();
		void DeactivateReactiveSystems();
		void ClearReactiveSystems();

	private:
		std::vector<std::shared_ptr<IInitializeSystem>> mInitializeSystems;
		std::vector<std::shared_ptr<IExecuteSystem>> mExecuteSystems;
		std::vector<std::shared_ptr<IFixedExecuteSystem>> mFixedExecuteSystems;
		std::vector<std::shared_ptr<ITimedExecuteSystem>> mTimedExecuteSystems;
};

template <typename T>
auto SystemContainer::Add() -> SystemContainer*
{
	return Add(std::shared_ptr<T>(new T()));
}
}
