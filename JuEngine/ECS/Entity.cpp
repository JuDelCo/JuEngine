// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Entity.hpp"
#include "../Components/Transform.hpp"
#include "../Resources/DebugLog.hpp"

namespace JuEngine
{
Entity::Entity(EntityManager* manager) : IObject("entityManager"),
	mEntityManager(manager)
{
	mSelf = shared_ptr<Entity>(this, [](void*){});

	mId = mEntityManager->GenerateId();

	this->CreateComponent<Transform>();
}

auto Entity::GetId() const -> const unsigned int
{
	return mId;
}

void Entity::Update()
{
	for(auto& iComponent : mComponents)
	{
		iComponent->Update();
	}
}

void Entity::FixedUpdate()
{
	for(auto& iComponent : mComponents)
	{
		iComponent->FixedUpdate();
	}
}

auto Entity::IsAlive() const -> const bool&
{
	return mAlive;
}

void Entity::Destroy()
{
	mAlive = false;
}

auto Entity::GetComponents() -> vector<shared_ptr<Component>>&
{
	return mComponents;
}

auto Entity::AddComponent(Component* component, const type_index type) -> Component*
{
	shared_ptr<Component> psComponent(component);
	Component* pwComponentArray{psComponent.get()};
	Component* pwComponent{psComponent.get()};

	psComponent->mEntity = mSelf.get();
	mComponents.emplace_back(std::move(psComponent));

	mComponentArray[type].emplace_back(std::move(pwComponentArray));
	mEntityManager->AddEntityByComponentType(mSelf.get(), type);

	pwComponent->Init();

	return pwComponent;
}

bool Entity::HasComponent(type_index type) const
{
	if(mComponentArray.count(type))
	{
		return (! mComponentArray.at(type).empty());
	}

	return false;
}

auto Entity::GetComponent(type_index type) const -> Component*
{
	if(HasComponent(type))
	{
		return mComponentArray.at(type).front();
	}

	DebugLog::Write("Warning: Entity.HasComponent: Not component of type %s found in this entity (%s)", type.name(), this->GetName().c_str());

	return nullptr;
}

auto Entity::GetComponents(type_index type) -> vector<Component*>
{
	if(HasComponent(type))
	{
		return mComponentArray.at(type);
	}

	DebugLog::Write("Warning: Entity.HasComponent: Not components of type %s found in this entity (%s)", type.name(), this->GetName().c_str());

	return vector<Component*>();
}
}
