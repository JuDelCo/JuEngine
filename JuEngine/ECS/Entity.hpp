// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"
#include "EntityManager.hpp"
#include "Component.hpp"
#include <vector>
#include <unordered_map>

namespace JuEngine
{
using std::vector;
using std::unordered_map;

class JUENGINEAPI Entity : public IObject
{
public:
	Entity(EntityManager* manager);
	virtual ~Entity() = default;

	auto GetId() const -> const unsigned int;
	void Update();
	void FixedUpdate();
	auto IsAlive() const -> const bool&;
	void Destroy();
	auto GetComponents() -> vector<shared_ptr<Component>>&;
	template <typename T, typename... TArgs> inline auto CreateComponent(TArgs&&... args) -> T*;
	template <typename T> inline bool HasComponent() const;
	template <typename T> inline auto GetComponent() const -> T*;
	template <typename T> inline auto GetComponents() -> vector<Component*>;

private:
	auto AddComponent(Component* component, const type_index type) -> Component*;
	bool HasComponent(type_index type) const;
	auto GetComponent(type_index type) const -> Component*;
	auto GetComponents(type_index type) -> vector<Component*>;

	bool mAlive{true};
	EntityManager* mEntityManager;
	vector<shared_ptr<Component>> mComponents;
	unordered_map<type_index, vector<Component*>> mComponentArray;
	unsigned int mId;

	shared_ptr<Entity> mSelf;
};

template <typename T, typename... TArgs>
auto Entity::CreateComponent(TArgs&&... args) -> T*
{
	return (T*)Entity::AddComponent(new T(std::forward<TArgs>(args)...), typeid(T));
}

template <typename T>
bool Entity::HasComponent() const
{
	return HasComponent(typeid(T));
}

template<typename T>
auto Entity::GetComponent() const -> T*
{
	return (T*)GetComponent(typeid(T));
}

template <typename T>
auto Entity::GetComponents() -> vector<Component*>
{
	return GetComponents(typeid(T));
}
}
