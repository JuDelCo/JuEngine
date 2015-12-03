// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Collider.hpp"
#include "../Resources/Entity.hpp"
#include "RigidBody.hpp"
#include <btBulletDynamicsCommon.h>

namespace JuEngine
{
Collider::Collider()
{
	mCollisionShape = new btSphereShape(1.f);
}

Collider::~Collider()
{
	delete mCollisionShape;

	// TODO: Collider: Notify rigidbody component (when deleting components be implemented)
}

void Collider::Init()
{
	if(mEntity->HasComponent<RigidBody>())
	{
		mEntity->GetComponent<RigidBody>()->Rebuild();
	}
}

auto Collider::GetCollisionShape() -> btCollisionShape*
{
	return mCollisionShape;
}
}
