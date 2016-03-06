// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../Resources/Math.hpp"
#include "../Entity/Entity.hpp"
#include <typeindex>
#include <unordered_map>
#include <memory>

namespace JuEngine
{
class Prefab;
class Pool;

class JUENGINEAPI PrefabManager : public IObject
{
	public:
		PrefabManager();
		~PrefabManager();

		void Unload();
		template <typename T> static void Add();
		template <typename T> static auto Instantiate(Pool* pool) -> EntityPtr;
		template <typename T> static auto Instantiate(Pool* pool, const vec3 position, const quat orientation) -> EntityPtr;
		static auto Instantiate(Pool* pool, const Identifier& id) -> EntityPtr;
		static auto Instantiate(Pool* pool, const Identifier& id, const vec3 position, const quat orientation) -> EntityPtr;

	private:
		template <typename T> static auto Get() -> T*;
		static void Add(std::shared_ptr<Prefab> prefab, std::type_index type);
		static auto Instantiate(Pool* pool, Prefab* prefab) -> EntityPtr;
		static auto Instantiate(Pool* pool, Prefab* prefab, const vec3 position, const quat orientation) -> EntityPtr;
		static auto Get(std::type_index type) -> Prefab*;

		std::unordered_map<std::type_index, std::shared_ptr<Prefab>> mPrefabs;

		// Sigleton
		static PrefabManager* mInstance;
};

template <typename T>
void PrefabManager::Add()
{
	Add(std::make_shared<T>(), typeid(T));
}

template<typename T>
auto PrefabManager::Instantiate(Pool* pool) -> EntityPtr
{
	return Instantiate(pool, Get(typeid(T)));
}

template <typename T>
auto PrefabManager::Instantiate(Pool* pool, const vec3 position, const quat orientation) -> EntityPtr
{
	return Instantiate(pool, Get(typeid(T)), position, orientation);
}

template<typename T>
auto PrefabManager::Get() -> T*
{
	return std::static_pointer_cast<T>(Get(typeid(T)));
}
}
