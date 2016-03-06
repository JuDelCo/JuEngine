// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include <typeindex>
#include <unordered_map>
#include <memory>

namespace JuEngine
{
class Level;

class JUENGINEAPI LevelManager : public IObject
{
	public:
		LevelManager();
		~LevelManager();

		void Update();
		void Unload();
		template <typename T> static void Add();
		template <typename T> static void LoadLevel();
		template <typename T> static void LoadLevelAdditive();
		static void LoadLevel(const Identifier& id);
		static void LoadLevelAdditive(const Identifier& id);
		static void UnloadLevel();

	private:
		template <typename T> static auto Get() -> T*;
		static void Add(std::shared_ptr<Level> level, std::type_index type);
		static void LoadLevel(Level* level);
		static void LoadLevelAdditive(Level* level);
		static auto Get(std::type_index type) -> Level*;

		std::unordered_map<std::type_index, std::shared_ptr<Level>> mLevels;
		std::type_index mRequestedLoadLevelType{typeid(void)};
		Identifier* mRequestedLoadLevelName{nullptr};
		bool mRequestedLoadLevel{false};
		bool mLoadAdditive{false};

		// Singleton
		static LevelManager* mInstance;
};

template <typename T>
void LevelManager::Add()
{
	Add(std::make_shared<T>(), typeid(T));
}

template <typename T>
void LevelManager::LoadLevel()
{
	LoadLevel(Get(typeid(T)));
}

template <typename T>
void LevelManager::LoadLevelAdditive()
{
	LoadLevelAdditive(Get(typeid(T)));
}

template<typename T>
auto LevelManager::Get() -> T*
{
	return (T*)Get(typeid(T));
}
}
