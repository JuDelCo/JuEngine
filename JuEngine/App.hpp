// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "DllExport.hpp"
#include "Resources/Identifier.hpp"
#include "Services/ILogService.hpp"
#include <typeindex>

namespace JuEngine
{
class IAppController;
class IDataService;
class IInputService;
class ILevelService;
class ILogService;
class ISystemService;
class ITimeService;
class IWindowService;

struct JUENGINEAPI App
{
	static auto Controller() -> IAppController*;
	static auto Data() -> IDataService*;
	static auto Input() -> IInputService*;
	static auto Level() -> ILevelService*;
	static auto Log() -> ILogService*;
	static auto System() -> ISystemService*;
	static auto Time() -> ITimeService*;
	static auto Window() -> IWindowService*;

	static void Provide(IAppController* appController);
	static void Provide(IDataService* dataService);
	static void Provide(IInputService* inputService);
	static void Provide(ILevelService* levelService);
	static void Provide(ILogService* logService);
	static void Provide(ISystemService* systemService);
	static void Provide(ITimeService* timeService);
	static void Provide(IWindowService* windowService);

	template <typename T> inline static auto Get(const Identifier& id) -> T*;
	template <typename T> inline static void Provide(const Identifier& id, T* service);

	private:
		static auto Get(const std::type_index type, const Identifier& id) -> void*;
		static void Provide(const std::type_index type, const Identifier& id, void* service);
};

template <typename T>
auto App::Get(const Identifier& id) -> T*
{
	return static_cast<T*>(Get(typeid(T), id));
}

template <typename T>
void App::Provide(const Identifier& id, T* service)
{
	Provide(typeid(T), id, service);
}

#define ThrowRuntimeError(message, ...) \
do { \
	App::Log()->Error(message, ##__VA_ARGS__); \
	App::Log()->RunTimeError(std::string(__PRETTY_FUNCTION__), __LINE__); \
} while(0);
}
