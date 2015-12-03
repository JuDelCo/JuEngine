// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "Component.hpp"

class btCollisionShape;

namespace JuEngine
{
class JUENGINEAPI Collider : public Component
{
friend class RigidBody;
friend class Entity;

public:
	Collider();
	~Collider();

protected:
	void Init();
	auto GetCollisionShape() -> btCollisionShape*;

private:
	btCollisionShape* mCollisionShape;
};
}
