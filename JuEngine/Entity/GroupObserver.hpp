// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "ComponentTypeId.hpp"
#include "Entity.hpp"
#include "GroupEventType.hpp"
#include <vector>
#include <unordered_set>
#include <functional>

namespace JuEngine
{
class Group;

class JUENGINEAPI GroupObserver
{
	public:
		GroupObserver(std::shared_ptr<Group> group, const GroupEventType eventType);
		GroupObserver(std::vector<std::shared_ptr<Group>> groups, std::vector<GroupEventType> eventTypes);
		~GroupObserver();

		void Activate();
		void Deactivate();
		auto GetCollectedEntities() -> std::unordered_set<EntityPtr>;
		void ClearCollectedEntities();

	private:
		void AddEntity(std::shared_ptr<Group> group, EntityPtr entity, ComponentId index, IComponent* component);

		std::unordered_set<EntityPtr> mCollectedEntities;
		std::vector<std::shared_ptr<Group>> mGroups;
		std::vector<GroupEventType> mEventTypes;
		std::function<void(std::shared_ptr<Group>, EntityPtr, ComponentId, IComponent*)> mAddEntityCache;
};
}
