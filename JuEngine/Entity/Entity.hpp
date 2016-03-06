// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "ComponentTypeId.hpp"
#include "../Resources/Delegate.hpp"
#include <stack>
#include <map>

namespace JuEngine
{
class Entity;
class Transform;
typedef std::shared_ptr<Entity> EntityPtr;

class JUENGINEAPI Entity
{
	friend class Pool;

	public:
		Entity(std::map<ComponentId, std::stack<IComponent*>>* componentPools);

		template <typename T, typename... TArgs> inline auto Add(TArgs&&... args) -> EntityPtr;
		template <typename T> inline auto Remove() -> EntityPtr;
		template <typename T, typename... TArgs> inline auto Replace(TArgs&&... args) -> EntityPtr;
		template <typename T> inline auto Refresh() -> EntityPtr;
		template <typename T> inline auto Get() const -> T*;
		template <typename T> inline auto Use() -> T*;
		template <typename T> inline bool Has() const;

		auto GetTransform() const -> Transform*;
		auto UseTransform() -> Transform*;
		bool HasComponents(const std::vector<ComponentId>& indices) const;
		bool HasAnyComponent(const std::vector<ComponentId>& indices) const;
		auto GetComponentsCount() const -> unsigned int;
		void RemoveAllComponents();
		auto GetUuid() const -> const unsigned int;
		bool IsEnabled();

		bool operator ==(const EntityPtr& right) const;
		bool operator ==(const Entity right) const;

		using EntityChanged = Delegate<void(EntityPtr entity, ComponentId index, IComponent* component)>;
		using ComponentReplaced = Delegate<void(EntityPtr entity, ComponentId index, IComponent* previousComponent, IComponent* newComponent)>;
		using EntityReleased = Delegate<void(Entity* entity)>;

		EntityChanged OnComponentAdded;
		ComponentReplaced OnComponentReplaced;
		EntityChanged OnComponentRemoved;
		EntityReleased OnEntityReleased;

	protected:
		void SetInstance(EntityPtr instance);
		auto AddComponent(const ComponentId index, IComponent* component) -> EntityPtr;
		auto RemoveComponent(const ComponentId index) -> EntityPtr;
		auto ReplaceComponent(const ComponentId index, IComponent* component) -> EntityPtr;
		auto GetComponent(const ComponentId index) const -> IComponent*;
		bool HasComponent(const ComponentId index) const;
		void Destroy();

		template <typename T, typename... TArgs> inline auto CreateComponent(TArgs&&... args) -> IComponent*;

		unsigned int mUuid{0};
		bool mIsEnabled = true;

	private:
		auto GetComponentPool(const ComponentId index) const -> std::stack<IComponent*>*;
		void Replace(const ComponentId index, IComponent* replacement);

		std::weak_ptr<Entity> mInstance;
		std::map<ComponentId, IComponent*> mComponents;
		std::map<ComponentId, std::stack<IComponent*>>* mComponentPools;
};

template <typename T, typename... TArgs>
auto Entity::CreateComponent(TArgs&&... args) -> IComponent*
{
	std::stack<IComponent*>* componentPool = GetComponentPool(ComponentTypeId::Get<T>());
	IComponent* component = nullptr;

	if(componentPool->size() > 0)
	{
		component = componentPool->top();
		componentPool->pop();
	}
	else
	{
		component = new T();
	}

	((T*)component)->Reset(std::forward<TArgs>(args)...);

	return component;
}

template <typename T, typename... TArgs>
auto Entity::Add(TArgs&&... args) -> EntityPtr
{
	return AddComponent(ComponentTypeId::Get<T>(), CreateComponent<T>(std::forward<TArgs>(args)...));
}

template <typename T>
auto Entity::Remove() -> EntityPtr
{
	return RemoveComponent(ComponentTypeId::Get<T>());
}

template <typename T, typename... TArgs>
auto Entity::Replace(TArgs&&... args) -> EntityPtr
{
	return ReplaceComponent(ComponentTypeId::Get<T>(), CreateComponent<T>(std::forward<TArgs>(args)...));
}

template <typename T>
auto Entity::Refresh() -> EntityPtr
{
	return ReplaceComponent(ComponentTypeId::Get<T>(), Get<T>());
}

template<typename T>
auto Entity::Get() const -> T*
{
	return (T*)GetComponent(ComponentTypeId::Get<T>());
}

template<typename T>
auto Entity::Use() -> T*
{
	Refresh<T>();
	return (T*)GetComponent(ComponentTypeId::Get<T>());
}

template <typename T>
bool Entity::Has() const
{
	return HasComponent(ComponentTypeId::Get<T>());
}
}

namespace std
{
template <>
struct hash<weak_ptr<JuEngine::Entity>>
{
	std::size_t operator()(const weak_ptr<JuEngine::Entity>& ptr) const
	{
		return hash<unsigned int>()(ptr.lock()->GetUuid());
	}
};

bool operator ==(weak_ptr<JuEngine::Entity> left, weak_ptr<JuEngine::Entity> right);
}
