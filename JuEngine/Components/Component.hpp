// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"

namespace JuEngine
{
class Entity;

class JUENGINEAPI Component : public IObject
{
friend class Entity;

public:
	Component() : IObject("component") {}
	virtual ~Component() = default;

	virtual void Init() {}
	virtual void Update() {}
	virtual void FixedUpdate() {}
	auto GetEntity() const -> Entity* { return mEntity; }

protected:
	// TODO: Component: AllowDuplicates{true}
	// TODO: Component: Enabled{true}
	Entity* mEntity;
};
}
