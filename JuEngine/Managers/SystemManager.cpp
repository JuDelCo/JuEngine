// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "SystemManager.hpp"
#include "../Resources/DebugLog.hpp"

namespace JuEngine
{
SystemManager* SystemManager::mInstance = nullptr;

SystemManager::SystemManager() : IObject("systemManager")
{
	if(SystemManager::mInstance != nullptr)
	{
		ThrowRuntimeError("Error, there are a SystemManager instance created already");
	}

	SystemManager::mInstance = this;

	Reset();
}

SystemManager::~SystemManager()
{
	SystemManager::mInstance = nullptr;
}

void SystemManager::Add(std::shared_ptr<ISystem> system)
{
	SystemManager::mInstance->mSystemContainer->Add(system);
}

void SystemManager::Initialize()
{
	SystemManager::mInstance->mSystemContainer->Initialize();
}

void SystemManager::Execute()
{
	SystemManager::mInstance->mSystemContainer->Execute();
}

void SystemManager::FixedExecute()
{
	SystemManager::mInstance->mSystemContainer->FixedExecute();
}

void SystemManager::Reset()
{
	SystemManager::mInstance->mSystemContainer = std::shared_ptr<SystemContainer>(new SystemContainer());
}
}
