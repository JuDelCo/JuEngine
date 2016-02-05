// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "EntityManager.hpp"
#include "Entity.hpp"
#include "../Components/World.hpp"
#include "../Resources/DebugLog.hpp"
#include <algorithm>

namespace JuEngine
{
EntityManager* EntityManager::mInstance = nullptr;

EntityManager::EntityManager() : IObject("entityManager")
{
	if(EntityManager::mInstance != nullptr)
	{
		ThrowRuntimeError("Error, there are an EntityManager instance created already");
	}

	EntityManager::world = new World::Data();

	EntityManager::mInstance = this;

	mSelf = shared_ptr<EntityManager>(this, [](void*){});
}

EntityManager::~EntityManager()
{
	EntityManager::mInstance = nullptr;
}

void EntityManager::Update()
{
	for(auto& iEntity : mEntities)
	{
		iEntity->Update();
	}

	Refresh();
}

void EntityManager::FixedUpdate()
{
	for(auto& iEntity : mEntities)
	{
		iEntity->FixedUpdate();
	}

	Refresh();
}

void EntityManager::Refresh()
{
	mEntities.erase(
		std::remove_if(std::begin(mEntities), std::end(mEntities),
		[](const shared_ptr<Entity>& entity)
		{
			return (! entity->IsAlive());
		}),
		std::end(mEntities));

	// TODO: EntityManager: Refresh mEntityComponentContainer (BEFORE start using e->Destroy() !!!)
}

void EntityManager::Unload()
{
	EntityManager::mInstance->mEntities.clear();
	EntityManager::mInstance->mEntityComponentContainer.clear();
}

auto EntityManager::Create() -> Entity*
{
	shared_ptr<Entity> psEntity(new Entity(EntityManager::mInstance->mSelf.get()));
	Entity* pwEntity{psEntity.get()};

	EntityManager::mInstance->mEntities.emplace_back(std::move(psEntity));

	return pwEntity;
}

auto EntityManager::Create(const string& name) -> Entity*
{
	auto entity = Create();
	entity->SetName(name);

	return entity;
}

auto EntityManager::Get(const string& name) -> Entity*
{
	for(auto& iEntity : EntityManager::mInstance->mEntities)
	{
		if(iEntity->GetName() == name)
		{
			return iEntity.get();
		}
	}

	DebugLog::Write("Warning: EntityManager.Get: No entity found with name %s", name.c_str());

	return nullptr;
}

auto EntityManager::GetAll() -> vector<shared_ptr<Entity>>&
{
	return EntityManager::mInstance->mEntities;
}

void EntityManager::ForEach(const function<void(Entity*)> function)
{
	for(auto& iEntity : EntityManager::mInstance->mEntities)
	{
		function(iEntity.get());
	}
}

auto EntityManager::GetWorld() -> World::Data*
{
	return EntityManager::mInstance->world;
}

void EntityManager::AddEntityByComponentType(Entity* entity, const type_index type)
{
	mEntityComponentContainer[type].emplace_back(entity);
}

auto EntityManager::GenerateId() -> const unsigned int
{
	return mIdCounter++;
}

bool EntityManager::HasEntities(type_index type)
{
	if(EntityManager::mInstance->mEntityComponentContainer.count(type))
	{
		return (! EntityManager::mInstance->mEntityComponentContainer.at(type).empty());
	}

	return false;
}

auto EntityManager::GetEntities(type_index type) -> vector<Entity*>&
{
	if(HasEntities(type))
	{
		return EntityManager::mInstance->mEntityComponentContainer.at(type);
	}

	DebugLog::Write("Warning: EntityManager.GetEntities: No entities found with component of type %s", type.name());

	return EntityManager::mInstance->mEntityComponentContainer[type];
}
}
