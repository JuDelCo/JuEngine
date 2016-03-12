// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "App.hpp"
#include "Services/IAppController.hpp"
#include "Services/IDataService.hpp"
#include "Services/IInputService.hpp"
#include "Services/ILevelService.hpp"
#include "Services/ISystemService.hpp"
#include "Services/ITimeService.hpp"
#include "Services/IWindowService.hpp"
#include <unordered_map>
#include <memory>

namespace JuEngine
{
static const std::string defaultServiceId = "base";
static std::unordered_map<std::type_index, std::unordered_map<Identifier, std::shared_ptr<void>>>* mServices = nullptr;

auto App::Get(const std::type_index type, const Identifier& id) -> void*
{
	if(mServices == nullptr)
	{
		return nullptr;
	}

	if((*mServices)[type].size() >= 1)
	{
		if((*mServices)[type].count(id) == 1)
		{
			return (*mServices)[type].begin()->second.get();
		}

		App::Log()->Warning("Warning: App.Get: No service of type '%u' with id '%s' found", type, id.GetStringRef().c_str());
	}

	App::Log()->Warning("Warning: App.Get: No services of type '%u' found", type);

	return nullptr;
}

void App::Provide(const std::type_index type, const Identifier& id, void* service)
{
	if(mServices == nullptr)
	{
		mServices = new std::unordered_map<std::type_index, std::unordered_map<Identifier, std::shared_ptr<void>>>();
	}

	if((*mServices)[type].count(id) > 0)
	{
		App::Log()->Warning("Warning, provided an existing service of type '%u' with id '%s'. The previous service has been unloaded.", type, id.GetStringRef().c_str());
	}

	(*mServices)[type][id] = std::shared_ptr<void>(service);
}

auto App::Controller() -> IAppController*
{
	return static_cast<IAppController*>(App::Get(typeid(IAppController), defaultServiceId));
}

auto App::Data() -> IDataService*
{
	return static_cast<IDataService*>(App::Get(typeid(IDataService), defaultServiceId));
}

auto App::Input() -> IInputService*
{
	return static_cast<IInputService*>(App::Get(typeid(IInputService), defaultServiceId));
}

auto App::Level() -> ILevelService*
{
	return static_cast<ILevelService*>(App::Get(typeid(ILevelService), defaultServiceId));
}

auto App::Log() -> ILogService*
{
	return static_cast<ILogService*>(App::Get(typeid(ILogService), defaultServiceId));
}

auto App::System() -> ISystemService*
{
	return static_cast<ISystemService*>(App::Get(typeid(ISystemService), defaultServiceId));
}

auto App::Time() -> ITimeService*
{
	return static_cast<ITimeService*>(App::Get(typeid(ITimeService), defaultServiceId));
}

auto App::Window() -> IWindowService*
{
	return static_cast<IWindowService*>(App::Get(typeid(IWindowService), defaultServiceId));
}

void App::Provide(IAppController* appController)
{
	App::Provide(typeid(IAppController), defaultServiceId, appController);
}

void App::Provide(IDataService* dataService)
{
	App::Provide(typeid(IDataService), defaultServiceId, dataService);
}

void App::Provide(IInputService* inputService)
{
	App::Provide(typeid(IInputService), defaultServiceId, inputService);
}

void App::Provide(ILevelService* levelService)
{
	App::Provide(typeid(ILevelService), defaultServiceId, levelService);
}

void App::Provide(ILogService* logService)
{
	App::Provide(typeid(ILogService), defaultServiceId, logService);
}

void App::Provide(ISystemService* systemService)
{
	App::Provide(typeid(ISystemService), defaultServiceId, systemService);
}

void App::Provide(ITimeService* timeService)
{
	App::Provide(typeid(ITimeService), defaultServiceId, timeService);
}

void App::Provide(IWindowService* windowService)
{
	App::Provide(typeid(IWindowService), defaultServiceId, windowService);
}
}
