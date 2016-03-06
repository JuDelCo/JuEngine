// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "PrefabManager.hpp"
#include "../Resources/Prefab.hpp"
#include "../Components/Transform.hpp"
#include "../Resources/DebugLog.hpp"

namespace JuEngine
{
PrefabManager* PrefabManager::mInstance = nullptr;

PrefabManager::PrefabManager() : IObject("prefabManager")
{
	if(PrefabManager::mInstance != nullptr)
	{
		ThrowRuntimeError("Error, there are a PrefabManager instance created already");
	}

	PrefabManager::mInstance = this;
}

PrefabManager::~PrefabManager()
{
	PrefabManager::mInstance = nullptr;
}

void PrefabManager::Unload()
{
	mPrefabs.clear();
}

auto PrefabManager::Instantiate(Pool* pool, const Identifier& id) -> EntityPtr
{
	for(const auto &iPrefab : PrefabManager::mInstance->mPrefabs)
	{
		if(iPrefab.second->GetId() == id)
		{
			return iPrefab.second->Create(pool);
		}
	}

	DebugLog::Write("Warning: PrefabManager.Instantiate: No prefab found with id %s", id.GetStringRef().c_str());

	return nullptr;
}

auto PrefabManager::Instantiate(Pool* pool, const Identifier& id, const vec3 position, const quat orientation) -> EntityPtr
{
	auto entity = Instantiate(pool, id);

	if(entity)
	{
		entity->GetTransform()->SetLocalPosition(position);
		entity->GetTransform()->SetLocalRotation(orientation);
	}

	return entity;
}

void PrefabManager::Add(std::shared_ptr<Prefab> prefab, std::type_index type)
{
	if(PrefabManager::mInstance->mPrefabs.count(type) != 0)
	{
		DebugLog::Write("Warning: PrefabManager.Add: Prefab of type %s exists already", type.name());

		return;
	}

	PrefabManager::mInstance->mPrefabs[type] = prefab;
}

auto PrefabManager::Instantiate(Pool* pool, Prefab* prefab) -> EntityPtr
{
	if(prefab)
	{
		return prefab->Create(pool);
	}

	return nullptr;
}

auto PrefabManager::Instantiate(Pool* pool, Prefab* prefab, const vec3 position, const quat orientation) -> EntityPtr
{
	if(prefab)
	{
		auto entity = prefab->Create(pool);

		if(entity)
		{
			entity->GetTransform()->SetLocalPosition(position);
			entity->GetTransform()->SetLocalRotation(orientation);
		}

		return entity;
	}

	return nullptr;
}

auto PrefabManager::Get(std::type_index type) -> Prefab*
{
	if(PrefabManager::mInstance->mPrefabs.count(type) != 0)
	{
		return &*PrefabManager::mInstance->mPrefabs.at(type);
	}

	DebugLog::Write("Warning: PrefabManager.Get: No prefab found of type %s", type.name());

	return nullptr;
}
}
