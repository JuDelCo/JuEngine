// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "SystemManager.hpp"
#include "../Entity/SystemContainer.hpp"

namespace JuEngine
{
SystemManager::SystemManager()
{
	SetId("systemManager");

	Reset();
}

void SystemManager::Add(std::shared_ptr<ISystem> system)
{
	mSystemContainer->Add(system);
}

void SystemManager::Initialize()
{
	mSystemContainer->Initialize();
}

void SystemManager::Execute()
{
	mSystemContainer->Execute();
}

void SystemManager::FixedExecute()
{
	mSystemContainer->FixedExecute();
}

void SystemManager::Reset()
{
	mSystemContainer = std::shared_ptr<SystemContainer>(new SystemContainer());
}
}
