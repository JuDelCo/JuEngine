// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Prefab.hpp"
#include "../Components/Transform.hpp"
#include "../Entity/Entity.hpp"

namespace JuEngine
{
Prefab::Prefab() : IObject("prefab")
{
}

auto Prefab::Create(Pool* pool, const Identifier& id) -> EntityPtr
{
	auto entity = Create(pool);

	entity->SetId(id);

	return entity;
}

auto Prefab::Create(Pool* pool, const Identifier& id, const vec3 position, const quat orientation) -> EntityPtr
{
	auto entity = Create(pool, id);

	entity->Get<Transform>()->SetLocalPosition(position);
	entity->Get<Transform>()->SetLocalRotation(orientation);

	return entity;
}
}
