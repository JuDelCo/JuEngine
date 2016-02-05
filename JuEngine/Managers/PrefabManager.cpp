// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "PrefabManager.hpp"
#include "../Resources/Prefab.hpp"
#include "../ECS/Entity.hpp"
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

auto PrefabManager::Instantiate(const string& prefabName) -> Entity*
{
	for(auto& iPrefab : PrefabManager::mInstance->mPrefabs)
	{
		if(iPrefab.second->GetName() == prefabName)
		{
			return iPrefab.second->Create();
		}
	}

	DebugLog::Write("Warning: PrefabManager.Instantiate: No prefab found with name %s", prefabName.c_str());

	return nullptr;
}

auto PrefabManager::Instantiate(const string& prefabName, const string& name) -> Entity*
{
	auto entity = Instantiate(prefabName);

	if(entity)
	{
		entity->SetName(name);
	}

	return entity;
}

auto PrefabManager::Instantiate(const string& prefabName, const vec3 position, const quat orientation) -> Entity*
{
	auto entity = Instantiate(prefabName);

	if(entity)
	{
		entity->GetComponent<Transform>()->SetLocalPosition(position);
		entity->GetComponent<Transform>()->SetLocalRotation(orientation);
	}

	return entity;
}

void PrefabManager::Add(shared_ptr<Prefab> prefab, type_index type)
{
	if(PrefabManager::mInstance->mPrefabs.count(type) != 0)
	{
		DebugLog::Write("Warning: PrefabManager.Add: Prefab of type %s exists already", type.name());

		return;
	}

	PrefabManager::mInstance->mPrefabs[type] = std::move(prefab);
}

auto PrefabManager::Instantiate(Prefab* prefab) -> Entity*
{
	if(prefab)
	{
		return prefab->Create();
	}

	return nullptr;
}

auto PrefabManager::Instantiate(Prefab* prefab, const string& name) -> Entity*
{
	if(prefab)
	{
		auto entity = prefab->Create();

		if(entity)
		{
			entity->SetName(name);
		}

		return entity;
	}

	return nullptr;
}

auto PrefabManager::Instantiate(Prefab* prefab, const vec3 position, const quat orientation) -> Entity*
{
	if(prefab)
	{
		auto entity = prefab->Create();

		if(entity)
		{
			entity->GetComponent<Transform>()->SetLocalPosition(position);
			entity->GetComponent<Transform>()->SetLocalRotation(orientation);
		}

		return entity;
	}

	return nullptr;
}

auto PrefabManager::Get(type_index type) -> Prefab*
{
	if(PrefabManager::mInstance->mPrefabs.count(type) != 0)
	{
		return &*PrefabManager::mInstance->mPrefabs.at(type);
	}

	DebugLog::Write("Warning: PrefabManager.Get: No prefab found of type %s", type.name());

	return nullptr;
}
}
