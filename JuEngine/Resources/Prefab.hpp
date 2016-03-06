// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../Entity/Entity.hpp"

namespace JuEngine
{
class Pool;

class JUENGINEAPI Prefab : public IObject
{
	public:
		Prefab() : IObject("prefab") {}

		virtual auto Create(Pool* pool) -> EntityPtr = 0;
};
}
