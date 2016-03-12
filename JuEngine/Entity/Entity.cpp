// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Entity.hpp"
#include "../App.hpp"

namespace JuEngine
{
Entity::Entity(std::map<ComponentId, std::stack<IComponent*>>* componentPools) : IObject("e")
{
	mComponentPools = componentPools;
}

auto Entity::AddComponent(const ComponentId index, IComponent* component) -> EntityPtr
{
	if (! mIsEnabled)
	{
		ThrowRuntimeError("Error, cannot add component to entity (%u), entity has already been destroyed.", mUuid);
	}

	if (HasComponent(index))
	{
		ThrowRuntimeError("Error, cannot add component to entity (%u), component already exists at index %u", mUuid, index);
	}

	mComponents[index] = component;

	OnComponentAdded(mInstance.lock(), index, component);

	return mInstance.lock();
}

auto Entity::RemoveComponent(const ComponentId index) -> EntityPtr
{
	if (! mIsEnabled)
	{
		ThrowRuntimeError("Error, cannot remove component to entity (%u), entity has already been destroyed.", mUuid);
	}

	if (! HasComponent(index))
	{
		ThrowRuntimeError("Error, cannot remove component to entity (%u), component not exists at index %u", mUuid, index);
	}

	Replace(index, nullptr);

	return mInstance.lock();
}

auto Entity::ReplaceComponent(const ComponentId index, IComponent* component) -> EntityPtr
{
	if (! mIsEnabled)
	{
		ThrowRuntimeError("Error, cannot replace component to entity (%u), entity has already been destroyed.", mUuid);
	}

	if (HasComponent(index))
	{
		Replace(index, component);
	}
	else if (component != nullptr)
	{
		AddComponent(index, component);
	}

	return mInstance.lock();
}

auto Entity::GetComponent(const ComponentId index) const -> IComponent*
{
	if (! HasComponent(index))
	{
		ThrowRuntimeError("Error, cannot get component from entity (%u), component not exists at index %u", mUuid, index);
	}

	return mComponents.at(index);
}

bool Entity::HasComponent(const ComponentId index) const
{
	return (mComponents.find(index) != mComponents.end());
}

bool Entity::HasComponents(const std::vector<ComponentId>& indices) const
{
	for(const ComponentId &index : indices)
	{
		if (! HasComponent(index))
		{
			return false;
		}
	}

	return true;
}

bool Entity::HasAnyComponent(const std::vector<ComponentId>& indices) const
{
	for(const ComponentId &index : indices)
	{
		if (HasComponent(index))
		{
			return true;
		}
	}

	return false;
}

auto Entity::GetComponentsCount() const -> unsigned int
{
	return mComponents.size();
}

void Entity::RemoveAllComponents()
{
	{
		auto componentsIdTemp = std::vector<ComponentId>(mComponents.size());

		for(const auto &pair : mComponents)
		{
			componentsIdTemp.push_back(pair.first);
		}

		while(! mComponents.empty())
		{
			Replace(componentsIdTemp.back(), nullptr);
			componentsIdTemp.pop_back();
		}
	}
}

auto Entity::GetUuid() const -> const unsigned int
{
	return mUuid;
}

bool Entity::IsEnabled()
{
	return mIsEnabled;
}

bool Entity::operator ==(const EntityPtr& right) const
{
	return this->GetUuid() == right->GetUuid();
}

bool Entity::operator ==(const Entity right) const
{
	return this->GetUuid() == right.GetUuid();
}

void Entity::SetInstance(EntityPtr instance)
{
	mInstance = std::weak_ptr<Entity>(instance);
}

void Entity::Destroy()
{
	RemoveAllComponents();
	OnComponentAdded.Clear();
	OnComponentReplaced.Clear();
	OnComponentRemoved.Clear();
	mIsEnabled = false;
}

auto Entity::GetComponentPool(const ComponentId index) const -> std::stack<IComponent*>*
{
	return &((*mComponentPools)[index]);
}

void Entity::Replace(const ComponentId index, IComponent* replacement)
{
	auto previousComponent = GetComponent(index);

	if(previousComponent == replacement)
	{
		OnComponentReplaced(mInstance.lock(), index, previousComponent, replacement);
	}
	else
	{
		GetComponentPool(index)->push(previousComponent);

		if(replacement == nullptr)
		{
			mComponents.erase(index);
			OnComponentRemoved(mInstance.lock(), index, previousComponent);
		}
		else
		{
			mComponents[index] = replacement;
			OnComponentReplaced(mInstance.lock(), index, previousComponent, replacement);
		}
	}
}
}

namespace std
{
bool operator ==(weak_ptr<JuEngine::Entity> left, weak_ptr<JuEngine::Entity> right)
{
	return left.lock().get() == right.lock().get();
}
}
