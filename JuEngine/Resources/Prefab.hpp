// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "Math.hpp"
#include <memory>

namespace JuEngine
{
class Pool;
class Entity;
typedef std::shared_ptr<Entity> EntityPtr;

class JUENGINEAPI Prefab : public IObject
{
	public:
		Prefab();

		virtual auto Create(Pool* pool) -> EntityPtr = 0;

		auto Create(Pool* pool, const Identifier& id) -> EntityPtr;
		auto Create(Pool* pool, const Identifier& id, const vec3 position, const quat orientation) -> EntityPtr;
};
}
