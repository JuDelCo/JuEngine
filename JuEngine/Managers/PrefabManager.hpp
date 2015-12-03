// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"
#include "../Math.hpp"
#include <typeindex>
#include <unordered_map>

namespace JuEngine
{
class Prefab;
class Entity;

using std::type_index;
using std::unordered_map;

class JUENGINEAPI PrefabManager : public IObject
{
public:
	PrefabManager();
	virtual ~PrefabManager();

	void Unload();
	template <typename T> static void Add();
	template <typename T> static auto Instantiate() -> Entity*;
	template <typename T> static auto Instantiate(const string& name) -> Entity*;
	template <typename T> static auto Instantiate(const vec3 position, const quat orientation) -> Entity*;
	static auto Instantiate(const string& prefabName) -> Entity*;
	static auto Instantiate(const string& prefabName, const string& name) -> Entity*;
	static auto Instantiate(const string& prefabName, const vec3 position, const quat orientation) -> Entity*;

private:
	template <typename T> static auto Get() -> T*;
	static void Add(shared_ptr<Prefab> prefab, type_index type);
	static auto Instantiate(Prefab* prefab) -> Entity*;
	static auto Instantiate(Prefab* prefab, const string& name) -> Entity*;
	static auto Instantiate(Prefab* prefab, const vec3 position, const quat orientation) -> Entity*;
	static auto Get(type_index type) -> Prefab*;

	unordered_map<type_index, shared_ptr<Prefab>> mPrefabs;

	// Sigleton
	static PrefabManager* mInstance;
};

template <typename T>
void PrefabManager::Add()
{
	Add(std::make_shared<T>(), typeid(T));
}

template<typename T>
auto PrefabManager::Instantiate() -> Entity*
{
	return Instantiate(Get(typeid(T)));
}

template <typename T>
auto PrefabManager::Instantiate(const string& name) -> Entity*
{
	return Instantiate(Get(typeid(T)), name);
}

template <typename T>
auto PrefabManager::Instantiate(const vec3 position, const quat orientation) -> Entity*
{
	return Instantiate(Get(typeid(T)), position, orientation);
}

template<typename T>
auto PrefabManager::Get() -> T*
{
	return std::static_pointer_cast<T>(Get(typeid(T)));
}
}
