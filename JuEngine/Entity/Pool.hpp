// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../DllExport.hpp"
#include "Entity.hpp"
#include "Matcher.hpp"
#include <unordered_map>
#include <unordered_set>

namespace JuEngine
{
class ISystem;
class Group;

class JUENGINEAPI Pool
{
	public:
		Pool(const unsigned int startCreationIndex = 1);
		~Pool();

		auto CreateEntity() -> EntityPtr;
		bool HasEntity(const EntityPtr& entity) const;
		void DestroyEntity(EntityPtr entity);
		void DestroyAllEntities();

		auto GetEntities() -> std::vector<EntityPtr>;
		auto GetEntities(const Matcher matcher) -> std::vector<EntityPtr>;
		auto GetGroup(Matcher matcher) -> std::shared_ptr<Group>;

		void ClearGroups();
		void ResetCreationIndex();
		void ClearComponentPool(const ComponentId index);
		void ClearComponentPools();
		void Reset();

		auto GetEntityCount() const -> unsigned int;
		auto GetReusableEntitiesCount() const -> unsigned int;
		auto GetRetainedEntitiesCount() const -> unsigned int;

		auto CreateSystem(std::shared_ptr<ISystem> system) -> std::shared_ptr<ISystem>;
		template <typename T> inline auto CreateSystem() -> std::shared_ptr<ISystem>;

		using PoolChanged = Delegate<void(Pool* pool, EntityPtr entity)>;
		using GroupChanged = Delegate<void(Pool* pool, std::shared_ptr<Group> group)>;

		PoolChanged OnEntityCreated;
		PoolChanged OnEntityWillBeDestroyed;
		PoolChanged OnEntityDestroyed;
		GroupChanged OnGroupCreated;
		GroupChanged OnGroupCleared;

	private:
		void UpdateGroupsComponentAddedOrRemoved(EntityPtr entity, ComponentId index, IComponent* component);
		void UpdateGroupsComponentReplaced(EntityPtr entity, ComponentId index, IComponent* previousComponent, IComponent* newComponent);
		void OnEntityReleased(Entity* entity);

		unsigned int mCreationIndex;
		std::unordered_set<EntityPtr> mEntities;
		std::unordered_map<Matcher, std::shared_ptr<Group>> mGroups;
		std::stack<Entity*> mReusableEntities;
		std::unordered_set<Entity*> mRetainedEntities;

		std::map<ComponentId, std::stack<IComponent*>> mComponentPools;
		std::map<ComponentId, std::vector<std::weak_ptr<Group>>> mGroupsForIndex;

		std::vector<EntityPtr> mEntitiesCache;
		std::function<void(Entity*)> mOnEntityReleasedCache;
};

template <typename T>
auto Pool::CreateSystem() -> std::shared_ptr<ISystem>
{
	return CreateSystem(std::dynamic_pointer_cast<ISystem>(std::shared_ptr<T>(new T())));
}
}
