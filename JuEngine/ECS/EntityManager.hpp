// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"
#include <vector>
#include <unordered_map>
#include <typeindex>

namespace JuEngine
{
class Entity;

namespace World
{
class Data;
}

using std::vector;
using std::unordered_map;
using std::function;
using std::type_index;

class JUENGINEAPI EntityManager : public IObject
{
friend class Entity;

public:
	EntityManager();
	virtual ~EntityManager();

	void Update();
	void FixedUpdate();
	void Refresh();
	static void Unload();
	static auto Create() -> Entity*;
	static auto Create(const string& name) -> Entity*;
	static auto Get(const string& name) -> Entity*;
	static auto GetAll() -> vector<shared_ptr<Entity>>&;
	template <typename T> static bool HasEntities();
	template <typename T> static auto GetEntities() -> vector<Entity*>&;
	static void ForEach(const function<void(Entity*)> function);
	template <typename T> static void ForEach(const function<void(Entity*)> function);
	static auto GetWorld() -> World::Data*;

protected:
	void AddEntityByComponentType(Entity* entity, const type_index type);
	auto GenerateId() -> const unsigned int;

private:
	vector<shared_ptr<Entity>> mEntities;
	unordered_map<type_index, vector<Entity*>> mEntityComponentContainer;
	static bool HasEntities(type_index type);
	static auto GetEntities(type_index type) -> vector<Entity*>&;

	World::Data* world;
	shared_ptr<EntityManager> mSelf;
	unsigned int mIdCounter{1};

	// Singleton
	static EntityManager* mInstance;
};

template <typename T>
bool EntityManager::HasEntities()
{
	return HasEntities(typeid(T));
}

template <typename T>
auto EntityManager::GetEntities() -> vector<Entity*>&
{
	return GetEntities(typeid(T));
}

template <typename T>
void EntityManager::ForEach(const function<void(Entity*)> function)
{
	for(auto& iEntity : GetEntities(typeid(T)))
	{
		function(iEntity);
	}
}
}
