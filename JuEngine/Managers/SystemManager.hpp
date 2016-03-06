// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../Entity/SystemContainer.hpp"
#include <memory>

namespace JuEngine
{
class JUENGINEAPI SystemManager : public IObject
{
	friend class Application;
	friend class LevelManager;

	public:
		SystemManager();
		~SystemManager();

		static void Add(std::shared_ptr<ISystem> system);
		template <typename T> static inline void Add();

	protected:
		static void Initialize();
		static void Execute();
		static void FixedExecute();

		static void Reset();

	private:
		std::shared_ptr<SystemContainer> mSystemContainer;

		// Singleton
		static SystemManager* mInstance;
};

template <typename T>
void SystemManager::Add()
{
	SystemManager::Add(std::shared_ptr<T>(new T()));
}
}
