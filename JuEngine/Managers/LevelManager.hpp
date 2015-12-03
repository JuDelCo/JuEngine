// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"
#include <typeindex>
#include <unordered_map>

namespace JuEngine
{
class Level;

using std::type_index;
using std::unordered_map;

class JUENGINEAPI LevelManager : public IObject
{
public:
	LevelManager();
	virtual ~LevelManager();

	void Update();
	void Unload();
	template <typename T> static void Add();
	template <typename T> static void LoadLevel();
	template <typename T> static void LoadLevelAdditive();
	static void LoadLevel(const string& name);
	static void LoadLevelAdditive(const string& name);
	static void UnloadLevel();

private:
	template <typename T> static auto Get() -> T*;
	static void Add(shared_ptr<Level> level, type_index type);
	static void LoadLevel(Level* level);
	static void LoadLevelAdditive(Level* level);
	static auto Get(type_index type) -> Level*;

	unordered_map<type_index, shared_ptr<Level>> mLevels;
	type_index mRequestedLoadLevelType{typeid(void)};
	string mRequestedLoadLevelName;
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
