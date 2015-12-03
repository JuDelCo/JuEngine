// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"

namespace JuEngine
{
class Entity;

class JUENGINEAPI Prefab : public IObject
{
public:
	Prefab() : IObject("prefab") {}
	virtual ~Prefab() = default;

	virtual auto Create() -> Entity* = 0;
};
}
